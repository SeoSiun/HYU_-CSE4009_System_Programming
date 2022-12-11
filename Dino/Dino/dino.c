#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#include<Windows.h>
#include<conio.h>

#define DINOSAUR_DISTANCE_FROM_TOP_Y	12
#define TREE_DISTANCE_FROM_LEFT_X		90
#define TREE_DISTANCE_FROM_TOP_Y		20
#define SCORE_DISTANCE_FROM_LEFT_X		85

void InitConsoleConfig();
int GetKeyDown();
void DrawDinosaur(int);
void DrawTree(int);
void GotoXY(int, int);
int score = 0;

int main(int argc, char* argv[]) {
	bool jumping = false;
	bool walking = true;
	int dinosaurY = DINOSAUR_DISTANCE_FROM_TOP_Y;
	int treeX = TREE_DISTANCE_FROM_LEFT_X;
	int gravity = 3;
	int speed = 2;
	int sleepTime = 80;

	InitConsoleConfig();

	while (true) {
		if (treeX <= 8 && treeX >= 3 && dinosaurY > 8) break;

		if (GetKeyDown() == 32 && walking) {	//32==스페이스바
			jumping = true;
			walking = false;
		}
		if (jumping) {
			dinosaurY = dinosaurY - gravity;
		}
		else {
			dinosaurY = dinosaurY + gravity;
		}
		if (dinosaurY <= 0) {
			dinosaurY = 0;
			jumping = false;
			if (treeX <= 10) {
				score++;
			}
			if (score % 4 == 0 && score != 0) {
				if (sleepTime != 0) sleepTime -= 20;
				else {
					speed++;
					gravity++;
				}
			}
		}
		if (dinosaurY >= DINOSAUR_DISTANCE_FROM_TOP_Y) {
			dinosaurY = DINOSAUR_DISTANCE_FROM_TOP_Y;
			walking = true;
		}
		treeX = treeX - speed;
		if (treeX <= 0) {
			treeX = TREE_DISTANCE_FROM_LEFT_X;
		}

		DrawDinosaur(dinosaurY);
		DrawTree(treeX);
		Sleep(sleepTime);
		system("cls");		//shell에서 운영체제가 해석할 명령을 적으면 된다, stdlib에 있음(C에서 제공하는 라이브러리), cls는 클리어스크린, 많이 쓰지 않는게 좋다
	}

	return 0;
}

//콘솔창 사이즈 조절
void InitConsoleConfig() {
	system("mode con:cols=100 lines=25");
}

void DrawDinosaur(int dinosaurY) {
	static bool legDraw = true;		//static: 컴파일 될 때 생겨서 제일 처음 초기화 이후 프로그램이 종료될 때까지 살아있다(함수가 끝나도 사라지지않음)
	//왼쪽 위가 0,0
	GotoXY(0, dinosaurY);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);

	//printf는 라이브러리~
	printf("        &&&&&&& \n");
	printf("       && &&&&&&\n");
	printf("       &&&&&&&&&\n");
	printf("&      &&&      \n");
	printf("&&     &&&&&&&  \n");
	printf("&&&   &&&&&     \n");
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE);
	printf(" &&  &&&&&&&&&& \n");
	printf(" &&&&&&&&&&&    \n");
	printf("  &&&&&&&&&&    \n");
	printf("    &&&&&&&&    \n");
	printf("     &&&&&&     \n");

	//legDraw == switch
	if (legDraw) {
		printf("     &    &&&     \n");
		printf("     &&           ");
		legDraw = false;

	}
	else {
		printf("     &&&  &       \n");
		printf("          &&      ");
		legDraw = true;
	}

	GotoXY(SCORE_DISTANCE_FROM_LEFT_X, 0);
	printf("Score: %d", score);
}

void DrawTree(int treeX) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_GREEN);
	GotoXY(treeX, TREE_DISTANCE_FROM_TOP_Y);
	printf("$$$$");
	GotoXY(treeX, TREE_DISTANCE_FROM_TOP_Y + 1);
	printf(" $$ ");
	GotoXY(treeX, TREE_DISTANCE_FROM_TOP_Y + 2);
	printf(" $$ ");
	GotoXY(treeX, TREE_DISTANCE_FROM_TOP_Y + 3);
	printf(" $$ ");
	GotoXY(treeX, TREE_DISTANCE_FROM_TOP_Y + 4);
	printf(" $$ ");

	GotoXY(SCORE_DISTANCE_FROM_LEFT_X, 0);
	printf("Score: %d", score);
}

void GotoXY(int x, int y) {
	COORD coord;
	coord.X = x;
	coord.Y = y;

	//GetStdHandle(STD_OUTPUT_HANDLE)==stdout
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

//사용자 입력을 scanf로 받으면 입력을 받을 때까지 기다려야함
int GetKeyDown() {
	//_kbhit : Windows에서 제공, 사용자가 어떤 키를 누르면 버퍼에 쌓아둠, 키보드가 눌리면 true(!=0), 시스템 콜
	if (_kbhit() != 0) {
		return _getch();		//버퍼에서 제일 앞에 있는 애를 가지고 옴
	}
	return 0;
}