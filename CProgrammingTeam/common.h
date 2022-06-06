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
char categoryToChar(char c) {
	if (c == 'B') return '!';
	else if (c == '.') return ' ';
	else if (c == 'W') return '#';
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