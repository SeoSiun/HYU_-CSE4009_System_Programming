#define _WINSOCK_DEPRECATED_NO_WARNINGS		//���� ������ ������Ʈ�Կ� ���� ������ ���Ǵ� �Լ��� �����ϴ� ���� ����

#include<stdio.h>
#include<stdlib.h>
#include<WinSock2.h>	//����, �����쿡�� ����� �����ϴ� ���
#include<process.h>

#pragma comment(lib, "ws2_32.lib")

#define PORT		7777
#define MAX_CLIENT	2

void InitServer();
void RunServer();
void CloseServer();
void ErrorHandling(const char*);		//����ó��

unsigned _stdcall Client1RecvThread(void*);		//�ý��ۿ��� �������ִ� ������

WSADATA wsaData;
//1��1 ������ ���� client 2����
SOCKET servSock, clientSock[2];		//��ũ����, ������ ���ش�, �����
SOCKADDR_IN servAddr, clientAddr[2];	//�ּ� ���� ����ü (port, ip ��)

int clientAddrLen = 0;	//Ŭ���̾�Ʈ �ּ� ������ ������ �ִ� ������ ����, ipv4���� �̷��ſ� ���� ���̰� �ٸ�, Ŭ���̾�Ʈ�� �����ϴµ� �ʿ�
char client1Cmd[10];	//Ŭ���̾�Ʈ1�κ��� �޴� ���
char client2Cmd[10];	//Ŭ���̾�Ʈ2�κ��� �޴� ���

int main(int argc, char* argv) {

	system("mode con cols=37 lines=24");	//�ܼ�ũ�� ����

	InitServer();

	RunServer();

	CloseServer();

	return 0;
}

void RunServer() {

	printf("START GAME!\n");

	//1�� palyer ���� ������
	_beginthreadex(NULL, 0, Client1RecvThread, 0, 0, NULL);

	//2�� palyer�� ��û�� ó���ϴ� ����
	while (1) {
		int res = 0;
		memset(client2Cmd, 0x00, sizeof(client2Cmd));

		// 2�����κ��� ���� �����͸� �� ������ �Ѿ��� ���, �� ���⸸ receive�� �� �ִ�. (recv�� �÷��̾� �� ���� ���ÿ� �������� ����)
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

		printf("[2P �̵�] x : %d, y : %d \n", client2Cmd[0], client2Cmd[1]);
	}
}

unsigned _stdcall Client1RecvThread(void* args) {

	while (1) {
		int res = 0;
		memset(client1Cmd, 0x00, sizeof(client1Cmd));

		res = recv(clientSock[0], client1Cmd, sizeof(client1Cmd) - 1, 0);	//1�����κ��� ���� �����͸� �� ������ �Ѿ��� ���

		if (-1 == res) {
			ErrorHandling("recv() error!");
		}

		if (0 == strcmp(client1Cmd, "exit")) {
			break;
		}

		if (0 == client1Cmd[0] && 0 == client1Cmd[1]) {
			continue;
		}

		send(clientSock[1], client1Cmd, sizeof(client1Cmd) - 1, 0);		//1�����κ��� ���� ���� 2������ �ٷ� �����ش�. (�����н� - ������ �״�� �����ִ� ��)

		printf("[1P �̵�] x : %d, y : %d \n", client1Cmd[0], client1Cmd[1]);
	}

	printf("Client1RecvThread Close.\n");

	return 0;
}

void InitServer() {
	//STEP1~6�� �ٸ� ������, ��� ���� ��ſ��� �Ͼ�� �����̴�. 


	//���� ���̺귯�� �ʱ�ȭ (����, IP����(V4,V6))				////////////////////////////////STEP1
	if (0 != WSAStartup(MAKEWORD(2, 2), &wsaData)) {		
		ErrorHandling("WASSatartup() error!");
	}

	// ������ ��ȭ�⿡ ������ ��, ��ȭ�⸦ ��ġ�ϴ� ����		/////////////////////////////////STEP2
	//ù ��° argument�� � ���������� ������(�������� �׷��̸�, AF_INET�� TCP/IP)
	servSock = socket(AF_INET, SOCK_STREAM, 0);		

	if (INVALID_SOCKET == servSock) {
		ErrorHandling("socket() error!");
	}

	//���� ���� �ʱ�ȭ														//////////////////////SEPT3
	memset(&servAddr, 0x00, sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_addr.s_addr = htonl(INADDR_ANY);	//������ IP �ʱ�ȭ
	servAddr.sin_port = PORT;						// ��Ʈ �ʱ�ȭ (��ȭ�⿡ ������ �� ��ȭ��ȣ)

	//���Ͽ� IP�ּҿ� PORT��ȣ ���� (��ȭ��� ��ȭ��ȣ�� ����)
	if (SOCKET_ERROR == bind(servSock, (SOCKADDR*)&servAddr, sizeof(servAddr))) {	///////////////SEPT4
		ErrorHandling("bind() error");
	}

	// Ŭ���̾�Ʈ ���� ���
	if (SOCKET_ERROR == listen(servSock, MAX_CLIENT)) {							////////////////////SETP5
		ErrorHandling("listen() error");
	}

	fputs("���� ���� �����...", stdout);

	clientAddrLen = sizeof(clientAddr[0]);	//Ŭ���̾�Ʈ �ּ������� ���̴� �������� �����ϱ� ������ �߿��� ����̴�.

	for (int idx = 0; idx < MAX_CLIENT; idx++) {	
		//Ŭ���̾�Ʈ ���� ����
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
	//close�ؾ� ���ο� ������ ���� �� �ִ�.

	for (int idx = 0; idx < MAX_CLIENT; idx++) {
		closesocket(clientSock[idx]);
	}
	closesocket(servSock);

	WSACleanup();
}

void ErrorHandling(const char* message) {

	fputs(message, stderr);		//stderr�� ����ϸ� �����ѹ�, ��������, �����޼����� ��µȴ�
	fputc('\n', stdout);
	exit(-1);
}