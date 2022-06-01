#include <Windows.h>

typedef struct Coord {
	int x;
	int y;
} Coord;

typedef struct Object {
	int isActive;
	char category;
} Object;

typedef struct Difficulty {
	int mapSize;
	int sightSize;
	int bombAmount;
} Difficulty;

/*
void debug() {
	gotoxy(0, 0);
	for (int y = 0; y < mapSize; y++) {
		for (int x = 0; x < mapSize; x++) {
			printf("%c ", map[x][y].category);
		}
		printf("\n");
	}

	gotoxy(charX, charY);
	printf("#");
}
*/

void initLobby();
void initGame();
void printSight();
void move();
void playGame();
void gameOver();
char categoryToChar(char c) {
	if (c == 'B') return '!';
	else if (c == '.') return ' ';
	else if (c == 'W') return '#';
}