#include <Windows.h>

typedef struct Coord {
	int x;
	int y;
} Coord;

typedef struct Object {
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
	char MONSTER;
} CATEGORY = { 'W', '.', 'S', 's', 'M', 'm', 'T', 'H', 'K'};

void initStory();
int initLobby();
void generateMaze();
void generateItem(int amount, char category, int max);
void initGame();
void destroyMap(int* mapSize);
void initHelp();
void printSight();
void clearSight();
int playGame();
void gameClear();
void gameOver();
void initRank();
int collisionCheck();

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