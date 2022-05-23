// 202211274 김수현
#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<Windows.h>
#include<conio.h>
void gotoxy(int x, int y);
void gameInit(void);
void printMap(void);
void playGame(void);
void move(void);
void gameRobby(void);
void clearMap(void);
void printstringxy(int x, int y, char s[]);
void updateInfo(void);
int collisionCheck(void);
void gameOver(void);
void printcharxy(int x, int y, char c);
void setConsoleSize(int, int);
void hideCursor();
struct Object {
	int x, y, category;
};
struct DifficultyCon {
	int mapSize, sightSize, objectAmount;
};
int cx, cy, mapSize, sightSize, objectAmount, difficulty;
char playerShow;
struct DifficultyCon difficultyCons[3] = { {51,11,10},{101,21,30},{151,31,50} };
struct Object objects[100];
int main(void) {
	hideCursor();
	while (1) {
		gameRobby();
		gameInit();
		playGame();
	}

	return 0;
}
void gameRobby(void) {
	int n;
	char s[50], ch;
	clearMap();
	printstringxy(2, 5, "고르세요 easy(0) normal(1) hard(2)");
	while (1) {
		ch = _getch();
		if (ch >= '0' && ch <= '2') {
			difficulty = ch - '0';
			break;
		}
	}
}
void gameInit(void) {
	int i;
	sightSize = difficultyCons[difficulty].sightSize;
	mapSize = difficultyCons[difficulty].mapSize;
	objectAmount = difficultyCons[difficulty].objectAmount;

	setConsoleSize(sightSize * 2, sightSize);

	for (i = 0; i < objectAmount; i++) {
		objects[i].category = 0;
		objects[i].x = rand() % (mapSize - 2) + 1;
		objects[i].y = rand() % (mapSize - 2) + 1;

	}
	cx = mapSize / 2;
	cy = mapSize / 2;
	playerShow = '@';
}
void playGame(void) {
	int isCollision;
	printMap();
	while (1) {
		move();
		isCollision = collisionCheck();
		if (isCollision) {
			break;
		}
		printMap();
	}
	gameOver();
}
void printMap(void) {
	int i, j;
	clearMap();
	for (i = 0; i < sightSize; i++) {
		for (j = 0; j < sightSize; j++) {
			if (i == 0 || i == sightSize - 1 || j == 0 || j == sightSize - 1) { //시야 범위 표시
				printcharxy(i, j, '*');
			}
			else {
				if (i == sightSize / 2 && j == sightSize / 2) {       //플레이어 표시
					printcharxy(i, j, playerShow);
				}
			}
		}
	}
	for (i = 0; i < objectAmount; i++) {
		int x = sightSize / 2 + objects[i].x - cx;
		int y = sightSize / 2 + objects[i].y - cy;
		if (x > 0 && x < sightSize - 1 && y>0 && y < sightSize) {
			printcharxy(sightSize / 2 + objects[i].x - cx, sightSize / 2 + objects[i].y - cy, '!');
		}

	}
	updateInfo();
}
void clearMap(void) {
	int i, j;
	for (i = 0; i < 100; i++) {
		COORD Cur = { 0,i };
		SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Cur);
		printf("                                                                                           ");
	}
}
void updateInfo(void) {
	char s[100];
	sprintf(s, "x: %d, y: %d", cx, cy);
	printstringxy(0, sightSize, s);
}
int collisionCheck(void) {
	int i;
	char s[100];
	for (i = 0; i < objectAmount; i++) {
		sprintf(s, "%d %d", objects[i].x, objects[i].y);
		if (objects[i].x == cx && objects[i].y == cy) {

			if (objects[i].category == 0) {
				return 1;
			}
		}
	}
	return 0;
}
void gameOver(void) {
	clearMap();
	printstringxy(2, 5, "게임 오버");
	printstringxy(2, 6, "(로비로 돌아가려면 아무 키나 누르세요)");
	_getch();

}
void gotoxy(int x, int y) {
	COORD Cur = { x * 2,y }; //글자 사이 간격 넓히기
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Cur);
}
void printstringxy(int x, int y, char s[]) {
	gotoxy(x, y);
	printf("%s", s);
	gotoxy(0, 0);
}
void printcharxy(int x, int y, char c) {
	gotoxy(x, y);
	printf("%c", c);
	gotoxy(0, 0);
}
void move(void) {
	char ch;
	int check;
	while (1) {
		check = 1;
		ch = _getch();
		switch (ch) {
		case 'w':
			cy--;
			break;
		case 's':
			cy++;
			break;
		case 'a':
			cx--;
			break;
		case 'd':
			cx++;
			break;
		default:
			check = 0;
			break;
		}
		if (check)break;
	}
}

void setConsoleSize(int x, int y) {
	SMALL_RECT windowSize = { 0 , 0 , x , y };
	SetConsoleWindowInfo(GetStdHandle(STD_OUTPUT_HANDLE), TRUE, &windowSize);
}

void hideCursor() {
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO info;
	info.dwSize = 100;
	info.bVisible = FALSE;
	SetConsoleCursorInfo(consoleHandle, &info);
}
