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

		if (GetKeyDown() == 32 && walking) {	
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
		system("cls");	
	}

	return 0;
}

void InitConsoleConfig() {
	system("mode con:cols=100 lines=25");
}

void DrawDinosaur(int dinosaurY) {
	static bool legDraw = true;		
	GotoXY(0, dinosaurY);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_RED);

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

	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

int GetKeyDown() {
	if (_kbhit() != 0) {
		return _getch();		
	}
	return 0;
}