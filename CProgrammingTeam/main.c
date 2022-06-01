#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include "common.h"
#include "console.h"

Object** map = NULL;
int difficulty;
const Difficulty difficultyCons[3] = {{51, 11, 20}, {101, 21, 30}, {151, 31, 50}};
int charX, charY, mapSize, sightSize, bombAmount;
int consoleX = 100, consoleY = 50;
int moveCount = 0;

int main() {
	hideConsoleCursor();
	setConsoleSize(consoleX, consoleY);
	for (;;) {
		initLobby();
		initGame();
		playGame();
		gameOver();
	}

	return 0;
}

void initLobby() {
	char ch;
	system("cls");
	printString(10, 9, "난이도를 선택하세요");
	printString(10, 11, "+-------------------------+");
	printString(10, 12, "| 0. EASY                 |");
	printString(10, 13, "+-------------------------+");
	printString(10, 15, "+-------------------------+");
	printString(10, 16, "| 1. NORMAL               |");
	printString(10, 17, "+-------------------------+");
	printString(10, 19, "+-------------------------+");
	printString(10, 20, "| 2. HARD                 |");
	printString(10, 21, "+-------------------------+");
	while (1) {
		ch = _getch();
		if (ch >= '0' && ch <= '2') {
			difficulty = ch - '0';
			break;
		}
	}
}

void initGame() {

	mapSize = difficultyCons[difficulty].mapSize;
	sightSize = difficultyCons[difficulty].sightSize;
	bombAmount = difficultyCons[difficulty].bombAmount;
	moveCount = 50; // 얘도 난이도 따라 조정 필요

	// 캐릭터의 x, y값 -> 맵 정중앙
	charX = mapSize / 2;
	charY = mapSize / 2;

	// difficulty에 맞게 map 배열 mapSize * mapSize 크기로 동적 할당
	map = (Object**)malloc(sizeof(Object*) * mapSize);
	for (int i = 0; i < mapSize; i++) {
		map[i] = (Object*)malloc(sizeof(Object) * mapSize);
	}
	// map 배열 초기화
	for (int x = 0; x < mapSize; x++) {
		for (int y = 0; y < mapSize; y++) {
			if (x == 0 || x == mapSize - 1 || y == 0 || y == mapSize - 1) {
				map[x][y].category = 'W';
			} else {
				map[x][y].category = '.';
			}
		}
	}

	// 폭탄 생성
	for (int i = 0; i < bombAmount; i++) {
		int x = rand() % (mapSize - 2) + 1;
		int y = rand() % (mapSize - 2) + 1;
		map[x][y].category = 'B';
		map[x][y].isActive = 1;
	}

}

void playGame() {
	system("cls");
	move();
}

void move() {
	printSight();

	char ch;
	for (;;) {
		ch = _getch();
		int dx = 0, dy = 0;

		if (ch == 'w') dy--;
		else if (ch == 'a') dx--;
		else if (ch == 's') dy++;
		else if (ch == 'd') dx++;
		
		// 맵 밖으로 벗어나지 못하게
		if (charX + dx <= 0 || charX + dx >= mapSize - 1 || charY + dy <= 0 || charY + dy >= mapSize - 1)
			continue;

		// 폭탄일 경우 게임 오버
		if (map[charX + dx][charY + dy].category == 'B') {
			break;
		} else {
			charX += dx;
			charY += dy;
			// 이동횟수 감소 및 없으면 게임 오버
			if (--moveCount < 0) break;
		}

		printSight();
	}
}

void printSight() {

	gotoxy(0, 0);
	printf("남은 이동 횟수: %d  ", moveCount);

	// startX, startY = 맵 프린트 시작 위치
	int startX = (consoleX / 2 - sightSize) / 2, startY = (consoleY - sightSize) / 2;
	gotoxy(startX - 1, startY - 1);
	for (int y = charY - sightSize / 2 - 1, i = 0; y <= charY + sightSize / 2 + 1; y++, i++) {
		for (int x = charX - sightSize / 2 - 1; x <= charX + sightSize / 2 + 1; x++) {
			// 시야 테두리 표시
			if (x == charX - sightSize / 2 - 1 || x == charX + sightSize / 2 + 1 || y == charY - sightSize / 2 - 1 || y == charY + sightSize / 2 + 1)
				printf("* ");
			// 맵 밖 표시
			else if (x < 0 || x > mapSize - 1 || y < 0 || y > mapSize - 1)
				printf("  ");
			// 플레이어 표시
			else if (x == charX && y == charY) printf("P ");
			// 맵 안 오브젝트 표시
			else printf("%c ", categoryToChar(map[x][y].category));
		}
		gotoxy(startX - 1, startY + i);
	}

	// DEBUG
	// gotoxy(0, 0);
	// printf("x: %d; y: %d        ", charX, charY);
}

void gameOver() {

	// 동적할당 해제
	for (int i = 0; i < mapSize; i++) {
		free(map[i]);
	}
	free(map);

	system("cls");
	printString(10, 10, "GAME OVER");
	printString(10, 12, "Press any key to continue");
	_getch();
}
