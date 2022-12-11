#define _WINSOCK_DEPRECATED_NO_WARNINGS		//소켓 버전을 업데이트함에 따라 이전에 사용되던 함수를 금지하는 것을 무시

#include<stdio.h>
#include<stdlib.h>
#include<WinSock2.h>	//소켓, 윈도우에서 통신을 관장하는 헤더
#include<process.h>

#pragma comment(lib, "ws2_32.lib")

#define PORT		7777
#define MAX_CLIENT	2

void InitServer();
void RunServer();
void CloseServer();
void ErrorHandling(const char*);		//예외처리

unsigned _stdcall Client1RecvThread(void*);		//시스템에서 제공해주는 쓰레드

WSADATA wsaData;
//1대1 대전을 위해 client 2개씩
SOCKET servSock, clientSock[2];		//디스크립터, 지정을 해준다, 상수값
SOCKADDR_IN servAddr, clientAddr[2];	//주소 관련 구조체 (port, ip 등)

int clientAddrLen = 0;	//클라이언트 주소 정보를 가지고 있는 데이터 길이, ipv4인지 이런거에 따라 길이가 다름, 클라이언트를 구분하는데 필요
char client1Cmd[10];	//클라이언트1로부터 받는 명령
char client2Cmd[10];	//클라이언트2로부터 받는 명령

int main(int argc, char* argv) {

	system("mode con cols=37 lines=24");	//콘솔크기 조절

	InitServer();

	RunServer();

	CloseServer();

	return 0;
}

void RunServer() {

	printf("START GAME!\n");

	//1번 palyer 수신 쓰레드
	_beginthreadex(NULL, 0, Client1RecvThread, 0, 0, NULL);

	//2번 palyer의 요청을 처리하는 로직
	while (1) {
		int res = 0;
		memset(client2Cmd, 0x00, sizeof(client2Cmd));

		// 2번으로부터 오는 데이터를 올 때까지 한없이 대기, 한 방향만 receive할 수 있다. (recv로 플레이어 두 명을 동시에 받을수는 없다)
		res = recv(clientSock[1], client2Cmd, sizeof(client2Cmd) - 1, 0);

		if (-1 == res) {
			ErrorHandling("recv() error!");
		}

		if (0 == strcmp(client2Cmd, "exit")) {
			break;
		}

		if (0 == client2Cmd[0] && 0 == client2Cmd[1]) {
			continue;
		}

		send(clientSock[0], client2Cmd, sizeof(client2Cmd) - 1, 0);

		printf("[2P 이동] x : %d, y : %d \n", client2Cmd[0], client2Cmd[1]);
	}
}

unsigned _stdcall Client1RecvThread(void* args) {

	while (1) {
		int res = 0;
		memset(client1Cmd, 0x00, sizeof(client1Cmd));

		res = recv(clientSock[0], client1Cmd, sizeof(client1Cmd) - 1, 0);	//1번으로부터 오는 데이터를 올 때까지 한없이 대기

		if (-1 == res) {
			ErrorHandling("recv() error!");
		}

		if (0 == strcmp(client1Cmd, "exit")) {
			break;
		}

		if (0 == client1Cmd[0] && 0 == client1Cmd[1]) {
			continue;
		}

		send(clientSock[1], client1Cmd, sizeof(client1Cmd) - 1, 0);		//1번으로부터 받은 것을 2번으로 바로 보내준다. (바이패스 - 받은걸 그대로 보내주는 것)

		printf("[1P 이동] x : %d, y : %d \n", client1Cmd[0], client1Cmd[1]);
	}

	printf("Client1RecvThread Close.\n");

	return 0;
}

void InitServer() {
	//STEP1~6은 다른 언어에서도, 모든 소켓 통신에서 일어나는 과정이다. 


	//소켓 라이브러리 초기화 (버전, IP형식(V4,V6))				////////////////////////////////STEP1
	if (0 != WSAStartup(MAKEWORD(2, 2), &wsaData)) {		
		ErrorHandling("WASSatartup() error!");
	}

	// 소켓을 전화기에 비유할 때, 전화기를 설치하는 과정		/////////////////////////////////STEP2
	//첫 번째 argument는 어떤 프로토콜을 쓸건지(프로토콜 그룹이름, AF_INET은 TCP/IP)
	servSock = socket(AF_INET, SOCK_STREAM, 0);		

	if (INVALID_SOCKET == servSock) {
		ErrorHandling("socket() error!");
	}

	//소켓 정보 초기화														//////////////////////SEPT3
	memset(&servAddr, 0x00, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);	//서버의 IP 초기화
	servAddr.sin_port = PORT;						// 포트 초기화 (전화기에 비유할 때 전화번호)

	//소켓에 IP주소와 PORT번호 결합 (전화기와 전화번호를 결합)
	if (SOCKET_ERROR == bind(servSock, (SOCKADDR*)&servAddr, sizeof(servAddr))) {	///////////////SEPT4
		ErrorHandling("bind() error");
	}

	// 클라이언트 연결 대기
	if (SOCKET_ERROR == listen(servSock, MAX_CLIENT)) {							////////////////////SETP5
		ErrorHandling("listen() error");
	}

	fputs("유저 접속 대기중...", stdout);

	clientAddrLen = sizeof(clientAddr[0]);	//클라이언트 주소정보의 길이는 버전마다 상이하기 떄문에 중요한 요소이다.

	for (int idx = 0; idx < MAX_CLIENT; idx++) {	
		//클라이언트 연결 수락
		clientSock[idx] = accept(servSock, (SOCKADDR*)&(clientAddr[idx]), &clientAddrLen);		////STEP6

		if (INVALID_SOCKET == clientSock[idx]) {
			ErrorHandling("accecpt() error");
		}
		else {
			printf("%s Connection Comlete! \n", inet_ntoa(clientAddr[idx].sin_addr));
		}
	}
}

void CloseServer() {
	//close해야 새로운 소켓을 만들 수 있다.

	for (int idx = 0; idx < MAX_CLIENT; idx++) {
		closesocket(clientSock[idx]);
	}
	closesocket(servSock);

	WSACleanup();
}

void ErrorHandling(const char* message) {

	fputs(message, stderr);		//stderr로 출력하면 에러넘버, 에러사유, 에러메세지가 출력된다
	fputc('\n', stdout);
	exit(-1);
}