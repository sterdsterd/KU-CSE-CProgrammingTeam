#include <Windows.h>

void gotoxy(int x, int y) {
	COORD Cur = {x * 2, y};
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Cur);
}

void setConsoleSize(int x, int y) {
	SMALL_RECT windowSize = {0, 0, x, y};
	SetConsoleWindowInfo(GetStdHandle(STD_OUTPUT_HANDLE), TRUE, &windowSize);
}

/*
0 = 검정색
1 = 파란색
 2 = 초록색
3 = 옥색
 4 = 빨간색
5 = 자주색
 6 = 노란색
7 = 흰색
 8 = 회색
9 = 연한 파란색
 10 = 연한 초록색
11 = 연한 옥색
 12 = 연한 빨간색
13 = 연한 자주색
 14 = 연한 노란색
15 = 진한 흰색 */
const struct {
	char RED, YELLOW, GREY, DARK_GREY, WHITE;
} COLOR = {4, 6, 7, 8, 15};

void setTextColor(int code) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), code);
}

void hideConsoleCursor() {
	HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	CONSOLE_CURSOR_INFO info;
	info.dwSize = 100;
	info.bVisible = FALSE;
	SetConsoleCursorInfo(consoleHandle, &info);
}

void printString(int x, int y, char s[]) {
	gotoxy(x, y);
	printf("%s", s);
}

void printQuote(char* name, char* line) {
	gotoxy(0, 42);
	printf("+---------------+\n| %s", name);
	gotoxy(8, 43);
	printf("|\n");
	printf("+--------------------------------------------------------------------------------------------------+\n");
	printf("|                                                                                                  |\n");
	printf("|                                                                                                  |\n");
	printf("|                                                                                                  |\n");
	printf("|                                                                                                  |\n");
	printf("|                                                                                                  |\n");
	printf("+--------------------------------------------------------------------------------------------------+");

	gotoxy(1, 45);
	printf("%s", line);
	//_getch();

}

void printSequence(char* str) {
	for (int i = 0; i < strlen(str); i++) {
		printf("%c", str[i]);
		Sleep(50);
	}
}

void cls() {
	system("cls");
}