#include <stdio.h>
//#include "common.h"

// 난이도 조절
const Difficulty difficultyCons[3] = {
	// EASY
	{.mapSize = 41, .sightSize = 11, .moveCount = 150, .minSight = 7, .maxSight = 19, .objectAmount = 15, .maxMoveAmount = 15},
	// NORMAL
	{.mapSize = 71, .sightSize = 21, .moveCount = 200, .minSight = 13, .maxSight = 29, .objectAmount = 45, .maxMoveAmount = 25},
	// HARD
	{.mapSize = 101, .sightSize = 37, .moveCount = 250, .minSight = 23, .maxSight = 37, .objectAmount = 100, .maxMoveAmount = 35}
};


typedef struct Game {
	// Constants
	int difficulty;
	int mapSize;
	int maxSight, minSight;
	int objectAmount;
	int maxMoveAmount;

	// Variables
	int score, moveCount;
	int sightSize;
	int charX, charY;
	int treasureX, treasureY;

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
	_this->charX = difficultyCons[difficulty].mapSize / 2;
	_this->charY = difficultyCons[difficulty].mapSize / 2;
	_this->score = score;
	_this->treasureX = 0;
	_this->treasureY = 0;

	return _this;
}