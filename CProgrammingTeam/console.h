#include <Windows.h>

void gotoxy(int x, int y) {
	COORD Cur = {x * 2, y};
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), Cur);
}

void setConsoleSize(int x, int y) {
	SMALL_RECT windowSize = {0, 0, x, y};
	SetConsoleWindowInfo(GetStdHandle(STD_OUTPUT_HANDLE), TRUE, &windowSize);
}

const struct {
	char GREY, YELLOW, DARK_GREY;
} COLOR = {7, 6, 8};

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
	gotoxy(0, 40);
	printf("+---------------+\n| %s", name);
	gotoxy(8, 41);
	printf("|\n");
	printf("+--------------------------------------------------------------------------------------------------+\n");
	printf("|                                                                                                  |\n");
	printf("|                                                                                                  |\n");
	printf("|                                                                                                  |\n");
	printf("|                                                                                                  |\n");
	printf("|                                                                                                  |\n");
	printf("|                                                                                                  |\n");
	printf("|                                                                                                  |\n");
	printf("+--------------------------------------------------------------------------------------------------+");

	gotoxy(1, 43);
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