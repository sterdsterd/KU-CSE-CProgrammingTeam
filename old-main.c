#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<Windows.h>
#include<conio.h>
void gameInit(void);
void printBase(void);
void printObject(void);
void playGame(void);
void move(void);
void gameRobby(void);
void clearScreen(void);
void printstringxy(int x, int y, char s[]);
void printInfo(void);
char collisionCheck(void);
void gameOver(void);
void printcharxy(int x, int y, char c);
void setConsoleSize(int, int);
void hideCursor();
void clearCoord(int, int);
void gotoxy(int, int);
struct Coord {
	int x, y;
};
struct Coord toRealCoord(int, int);
struct Object {
	int x, y, active;  
	char category;
};
struct DifficultyCon {
	int mapSize, sightSize, bombAmount;
};
struct Coord toRealCoord(int, int);
struct PrintManager {
	int index;
	struct Coord printedCoords[20];
};
int cx, cy, mapSize, sightSize, objectAmount, difficulty, bombAmount;
char playerShow;
const char OBJECTSHOW = '!';
const int consoleSizeX = 100, consoleSizeY = 40, centerX = 50, centerY = 20;
const struct DifficultyCon difficultyCons[3] = { {51,11,20},{101,21,30},{151,31,50} }; 
struct PrintManager printManager = { 0,{{0,0}} };
struct Object objects[100];
int main(void) {
	setConsoleSize(consoleSizeX, consoleSizeY);
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
	clearScreen();
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
	int i,j=0;
	sightSize = difficultyCons[difficulty].sightSize;
	mapSize = difficultyCons[difficulty].mapSize;
	objectAmount = difficultyCons[difficulty].bombAmount; // 각 개수를 더해줘서 총 합을 만들어야 됨

	for (i = 0; i < difficultyCons[difficulty].bombAmount; i++) {
		objects[j].category = 'B';
		objects[j].active = 1;
		objects[j].x = rand() % (mapSize - 2) + 1;
		objects[j].y = rand() % (mapSize - 2) + 1;
		j++;
	}
	printManager.index = 0;
	cx = mapSize / 2;
	cy = mapSize / 2;
	playerShow = 'P';

}
void playGame(void) {
	int isCollision;
	clearScreen();
	printBase();
	printObject();
	printInfo();
	while (1) {
		move();
		printInfo();
		isCollision = collisionCheck();
		if (isCollision=='B') {
			break;
		}
		printObject();
	}
	gameOver();
}
void move(void) {
	char ch;
	int check;
	while (1) {
		check = 1;
		ch = _getch();
		if (ch == 'w')cy--;
		else if (ch == 's')cy++;
		else if (ch == 'a')cx--;
		else if (ch == 'd')cx++;
		else check = 0;
		if (check)break;
	}
}

char collisionCheck(void) {
	int i;
	char s[100];
	for (i = 0; i < objectAmount; i++) {
		sprintf(s, "%d %d", objects[i].x, objects[i].y);
		if (objects[i].x == cx && objects[i].y == cy) {
			objects[i].active = 0;
			return objects[i].category;
		}
	}
	return '-';
}
void gameOver(void) {
	clearScreen();
	printstringxy(2, 5, "게임 오버");
	printstringxy(2, 6, "(로비로 돌아가려면 아무 키나 누르세요)");
	_getch();

}
//메인 출력 부분
void printBase(void) {
	int i, j;
	for (i = 0; i < sightSize; i++) {
		for (j = 0; j < sightSize; j++) {
			if (i == 0 || i == sightSize - 1 || j == 0 || j == sightSize - 1) { //시야 범위 표시
				printcharxy((i + centerX / 2 - sightSize / 2) * 2, j + centerY - sightSize / 2, '*');
			}
			else {
				if (i == sightSize / 2 && j == sightSize / 2) {       //플레이어 표시
					printcharxy((i + centerX / 2 - sightSize / 2) * 2, j + centerY - sightSize / 2, playerShow);
				}
			}
		}
	}
}
void printObject(void) {
	int i, j;
	for (i = 0; i < printManager.index; i++) {
		clearCoord(printManager.printedCoords[i].x, printManager.printedCoords[i].y);
	}
	printManager.index = 0;
	for (i = 0; i < objectAmount; i++) {
		int x = sightSize / 2 + objects[i].x - cx;
		int y = sightSize / 2 + objects[i].y - cy;
		if (x > 0 && x < sightSize - 1 && y>0 && y < sightSize - 1 && objects[i].active) {
			struct Coord coord = toRealCoord(objects[i].x, objects[i].y);
			printcharxy(coord.x, coord.y, OBJECTSHOW);
			printManager.printedCoords[printManager.index].x = coord.x;
			printManager.printedCoords[printManager.index].y = coord.y;
			printManager.index++;
		}
	}
}
void printInfo(void) {
	char s[100];
	sprintf(s, "x:%d y:%d", cx, cy);
	printstringxy(centerX - sightSize + 1, centerY + sightSize / 2 + 1, s);
}

//출력 관련 부분
void gotoxy(int x, int y) {
	COORD Cur = { x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Cur);
}

void printstringxy(int x, int y, char s[]) {
	gotoxy(x, y);
	printf("%s", s);
}
void printcharxy(int x, int y, char c) {
	gotoxy(x, y);
	printf("%c", c);
}
void clearScreen(void) {
	system("cls");
}
void clearCoord(int x, int y) {
	gotoxy(x, y);
	printf(" ");
}

struct Coord toRealCoord(int x, int y) {                                   //오브젝트의 게임 상 좌표를 실제 창에서의 출력 위치로 바꿈
	struct Coord coord = { (x - cx + centerX / 2) * 2, y - cy + centerY };
	return coord;
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
