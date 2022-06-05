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
} Difficulty;

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
