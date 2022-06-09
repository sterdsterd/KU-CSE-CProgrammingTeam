#include <Windows.h>
#include "console.h"

typedef struct Coord {
	int x;
	int y;
} Coord;

typedef struct Object {
	int isActive;
	char category;
	int amount;
} Object;

typedef struct Score {
	char name[10];
	int score;
} Score;

typedef struct Difficulty {
	int mapSize;
	int sightSize;
	int moveCount;
	int maxSight;
	int minSight;
	int objectAmount;
	int maxMoveAmount;
} Difficulty;

const struct {
	char WALL;
	char BLANK;
	char INCREASE_SIGHT;
	char DECREASE_SIGHT;
	char INCREASE_MOVE;
	char DECREASE_MOVE;
	char TREASURE;
	char HINT;
} CATEGORY = { 0, 1, 2, 3, 4, 5, 6, 7 };

void initStory();
int initLobby();
void generateMaze();
void generateItem(int amount, char category, int max);
void initGame();
void initHelp();
void printSight();
void clearSight();
int playGame();
void gameClear();
void gameOver();
void initRank();
int collisionCheck(Object** map, int dx, int dy);

void categoryToChar(char c) {
	if (c == CATEGORY.BLANK) printf("  ");
	else if (c == CATEGORY.WALL) {
		setTextColor(COLOR.DARK_GREY);
		printf("бс");
		setTextColor(COLOR.GREY);
	} else {
		setTextColor(COLOR.YELLOW);
		printf("б┌");
		setTextColor(COLOR.GREY);
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