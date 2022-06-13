#include <stdio.h>
//#include "common.h"

// 난이도 조절
const Difficulty difficultyCons[3] = {
	// EASY
	{.mapSize = 15, .sightSize = 30, .moveCount = 80, .minSight = 5, .maxSight = 15, .objectAmount = 4, .maxMoveAmount = 5},
	// NORMAL
	{.mapSize = 25, .sightSize = 9, .moveCount = 200, .minSight = 13, .maxSight = 17, .objectAmount = 6, .maxMoveAmount = 15},
	// HARD
	{.mapSize = 35, .sightSize = 21, .moveCount = 250, .minSight = 15, .maxSight = 27, .objectAmount = 8, .maxMoveAmount = 35}
};

typedef struct Monster {
	int** visitedCount;
	int x, y;
} Monster;

typedef struct Game {
	// Map
	Object** map;

	// Constants
	int difficulty;
	int mapSize;
	int maxSight, minSight;
	int objectAmount;
	int maxMoveAmount;

	// Variables
	int score, moveCount;
	int sightSize;
	Coord character;
	Coord treasure;
	Monster monster;

} Game;

Game* new_Game(int difficulty, int score) {
	Game* _this = malloc(sizeof(Game));

	_this->difficulty = difficulty;
	_this->mapSize = difficultyCons[difficulty].mapSize;
	_this->maxSight = difficultyCons[difficulty].maxSight;
	_this->minSight = difficultyCons[difficulty].minSight;
	_this->objectAmount = difficultyCons[difficulty].objectAmount;
	_this->maxMoveAmount = difficultyCons[difficulty].maxMoveAmount;

	_this->sightSize = difficultyCons[difficulty].sightSize;
	_this->moveCount = difficultyCons[difficulty].moveCount;
	_this->score = score;
	_this->character.x = 0;
	_this->character.y = 0;
	_this->treasure.x = 0;
	_this->treasure.y = 0;
	_this->monster.x = 0;
	_this->monster.y = 0;

	_this->monster.visitedCount = (int**)malloc(sizeof(int*) * difficultyCons[difficulty].mapSize);
	(_this->monster.visitedCount)[0] = (int*)malloc(sizeof(int) * difficultyCons[difficulty].mapSize * difficultyCons[difficulty].mapSize);

	_this->map = (Object**)malloc(sizeof(Object*) * difficultyCons[difficulty].mapSize);
	(_this->map)[0] = (Object*)malloc(sizeof(Object) * difficultyCons[difficulty].mapSize * difficultyCons[difficulty].mapSize);

	for (int i = 0; i < difficultyCons[difficulty].mapSize; i++) {
		if (i > 0) {
			(_this->map)[i] = (_this->map)[i - 1] + difficultyCons[difficulty].mapSize;
			(_this->monster.visitedCount)[i] = (_this->monster.visitedCount)[i - 1] + difficultyCons[difficulty].mapSize;
		}
		for (int j = 0; j < difficultyCons[difficulty].mapSize; j++) {
			(_this->map)[i][j].amount = 0;
			(_this->map)[i][j].category = 0;

			(_this->monster.visitedCount)[i][j] = 1;
		}

	}
	(_this->monster.visitedCount)[0][0] = 2;
	return _this;
}