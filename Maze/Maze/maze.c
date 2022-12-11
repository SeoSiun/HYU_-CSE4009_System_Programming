#include "maze.h"

void TextColorChange(int);
void GotoXY(int, int);
void DrawMap(int,int);
void RunGame(int, int);
int Checker(int, int);
int isStar(int, int);

int main(int argc, char* argv[]) {
	int circlePosX = 4, circlePosY = 3;

	DrawMap(circlePosX, circlePosY);

	RunGame(circlePosX, circlePosY);

	return 0;
}

void TextColorChange(int colorNumber) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), colorNumber);
}

void GotoXY(int x, int y) {
	COORD pos = { x,y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

void DrawMap(int circlePosX, int circlePosY) {
	system("mode con cols=37 lines=28");

	int idx = 0, idy = 0;

	GotoXY(0, 2);

	for (idx = 0; idx < HEIGHT; idx++) {
		for (idy = 0; idy < WIDTH; idy++) {
			switch (map[idx][idy]) {
				case 0:
					printf("  ");
					break;
				case 1:
					TextColorChange(8);
					printf("■");
					break;
				case 2:
					TextColorChange(12);
					printf("★");
					break;
			}
		}
		printf("\n");
	}

	GotoXY(circlePosX, circlePosY+2);
	TextColorChange(14);
	printf("●");
}

int Checker(int x, int y) {
	switch (map[y][x]) {
		case 0:
		case 2:
			return 1;
			break;
		default:
			return 0;
			break;
	}
}

int isStar(int x, int y) {
	if (map[y][x] == 2) return  1;
	else return 0;
}

void RunGame(int circlePosX, int circlePosY) {
	int cmd = 0;

	while (1) {
		TextColorChange(14);

		cmd = _getch();

		switch (cmd) {
			case UP:
				GotoXY(circlePosX, circlePosY+2);
				printf("  ");

				circlePosY = circlePosY - 1;
				if (!Checker(circlePosX,circlePosY)) {	//이동이 불가능한 경우
					circlePosY = circlePosY + 1;
				}
				break;
			case DOWN:
				GotoXY(circlePosX, circlePosY + 2);
				printf("  ");

				circlePosY = circlePosY + 1;
				if (!Checker(circlePosX, circlePosY)) {	
					circlePosY = circlePosY - 1;
				}
				break;
			case LEFT:
				GotoXY(circlePosX, circlePosY + 2);
				printf("  ");

				circlePosX = circlePosX - 2;	//빈칸이 공백 두개니까 1이 아니라 2
				if (!Checker(circlePosX, circlePosY)) {	
					circlePosX = circlePosX + 2;
				}
				break;
			case RIGHT:
				GotoXY(circlePosX, circlePosY + 2);
				printf("  ");

				circlePosX = circlePosX + 2;
				if (!Checker(circlePosX, circlePosY)) {	
					circlePosX = circlePosX - 2;
				}
				break;
			case EXIT:
				return;

		}
		GotoXY(circlePosX, circlePosY + 2);		//배열과 cmd는 y에서 2만큼의 차이가 있고 x,y가 반대
		printf("●");
		if (isStar(circlePosX, circlePosY)) {
			return;
		}
	}
}