#include <Windows.h>

typedef struct Coord {
	int x;
	int y;
} Coord;

typedef struct Object {
	int isActive;
	char category;
} Object;

typedef struct Score {
	char name[10];
	int score;
} Score;

typedef struct Difficulty {
	int mapSize;
	int sightSize;
	int bombAmount;
	int moveCount;
	int maxSight;
	int minSight;
} Difficulty;

void initStory();
int initLobby();
void initGame();
void printSight();
void clearSight();
int playGame();
void gameOver();
int collisionCheck(Object** map, int dx, int dy);
void categoryToChar(char c) {
	if (c == 'B') printf("! ");
	else if (c == '.') printf("  ");
	else if (c == 'W') {
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 8);
		printf("бс");
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
	}
	else {
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 6);
		printf("б┌");
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
	}
}

void sort(Score* arr, int size) {
	int i, j;
	Score key;

	for (i = 1; i < size; i++) {
		key = arr[i];

		for (j = i - 1; j >= 0 && arr[j].score < key.score; j--)
			arr[j + 1] = arr[j];

		arr[j + 1] = key;
	}
}