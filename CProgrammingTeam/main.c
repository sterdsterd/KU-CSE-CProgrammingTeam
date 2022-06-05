#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include "common.h"
#include "console.h"

Object** map = NULL;
Score rank[10];
int difficulty;
const Difficulty difficultyCons[3] = {{51, 11, 20, 50}, {101, 21, 30, 100}, {151, 31, 50, 150}};
int charX, charY, mapSize, sightSize, bombAmount, moveCount;
int consoleX = 100, consoleY = 50;
int score = 0, rankSize = 0;

int main() {
	hideConsoleCursor();
	setConsoleSize(consoleX, consoleY);
	if (initLobby()) return 0;
	for (;;) {
		initGame();
		if (playGame() == 1) {
			gameOver();
			if (initLobby()) break;
		} else {
			difficulty++;
			score += 200;
			for (int i = 0; i < mapSize; i++) {
				free(map[i]);
			}
			free(map);
		}
	}

	return 0;
}

int initLobby() {
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
	printString(10, 23, "+-------------------------+");
	printString(10, 24, "| 3. 종료                 |");
	printString(10, 25, "+-------------------------+");
	while (1) {
		ch = _getch();
		if (ch >= '0' && ch <= '2') {
			difficulty = ch - '0';
			return 0;
		} else if (ch == '3') return 1;
	}
}

void initGame() {

	mapSize = difficultyCons[difficulty].mapSize;
	sightSize = difficultyCons[difficulty].sightSize;
	bombAmount = difficultyCons[difficulty].bombAmount;
	moveCount = difficultyCons[difficulty].moveCount;

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

	// 보물 생성
	int x = rand() % (mapSize - 2) + 1;
	int y = rand() % (mapSize - 2) + 1;
	map[x][y].category = 'T';
	map[x][y].isActive = 1;

	// 폭탄 생성
	for (int i = 0; i < bombAmount; i++) {
		int x = rand() % (mapSize - 2) + 1;
		int y = rand() % (mapSize - 2) + 1;
		map[x][y].category = 'B';
		map[x][y].isActive = 1;
	}

	// 시야 증가
	for (int i = 0; i < 10; i++) {
		int x = rand() % (mapSize - 2) + 1;
		int y = rand() % (mapSize - 2) + 1;
		map[x][y].category = 'S';
		map[x][y].isActive = 1;
	}

	// 시야 감소
	for (int i = 0; i < 10; i++) {
		int x = rand() % (mapSize - 2) + 1;
		int y = rand() % (mapSize - 2) + 1;
		map[x][y].category = 's';
		map[x][y].isActive = 1;
	}

	// 이동 횟수 증가
	for (int i = 0; i < 10; i++) {
		int x = rand() % (mapSize - 2) + 1;
		int y = rand() % (mapSize - 2) + 1;
		map[x][y].category = 'M';
		map[x][y].isActive = 1;
	}

	// 이동 횟수 증가
	for (int i = 0; i < 10; i++) {
		int x = rand() % (mapSize - 2) + 1;
		int y = rand() % (mapSize - 2) + 1;
		map[x][y].category = 'm';
		map[x][y].isActive = 1;
	}

}

int playGame() {
	system("cls");
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

		// 충돌 체크
		int c = collisionCheck(map, dx, dy);
		if (c) return c;

		printSight();
	}
}

int collisionCheck(Object** map, int dx, int dy) {
	switch (map[charX + dx][charY + dy].category) {
	case 'T':
		printQuote("알림", "보물을 찾았습니다.");
		return 2;

	case 'B':
		printQuote("알림", "폭탄을 밟았습니다.");
		return 1;

	case 'S':
		printQuote("알림", "시야가 증가되었습니다.");
		sightSize += 10;
		map[charX + dx][charY + dy].category = '.';
		break;

	case 's':
		printQuote("알림", "시야가 감소되었습니다.");
		clearSight();
		map[charX + dx][charY + dy].category = '.';
		sightSize -= 10;
		break;

	case 'M':
		printQuote("알림", "이동 횟수가 증가되었습니다.");
		map[charX + dx][charY + dy].category = '.';
		moveCount += 15;
		break;

	case 'm':
		printQuote("알림", "이동 횟수가 감소되었습니다.");
		map[charX + dx][charY + dy].category = '.';
		moveCount -= 15;
		break;

	}

	charX += dx;
	charY += dy;
	score++;
	// 이동횟수 감소 및 없으면 게임 오버
	if (--moveCount < 0) return 1;

	return 0;
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
	gotoxy(startX - 1, startY + sightSize + 1);
	printf("좌표: (%d,%d)", charX, charY);
	gotoxy(startX - 1, startY + sightSize + 2);
	printf("점수: %d", score);
	// DEBUG
	// gotoxy(0, 0);
	// printf("x: %d; y: %d        ", charX, charY);
}

void clearSight() {
	int startX = (consoleX / 2 - sightSize) / 2, startY = (consoleY - sightSize) / 2;
	gotoxy(startX - 1, startY - 1);
	for (int y = charY - sightSize / 2 - 1, i = 0; y <= charY + sightSize / 2 + 3; y++, i++) {
		for (int x = charX - sightSize / 2 - 1; x <= charX + sightSize / 2 + 1; x++) {
			printf("  ");
		}
		gotoxy(startX - 1, startY + i);
	}
}

void gameOver() {

	// 동적할당 해제
	for (int i = 0; i < mapSize; i++) {
		free(map[i]);
	}
	free(map);

	system("cls");
	printString(10, 10, "GAME OVER");
	gotoxy(10, 12);
	printf("SCORE: %d", score);
	printString(10, 14, "NAME : ");
	scanf("%s", &rank[rankSize].name);
	rank[rankSize].score = score;
	rankSize++;
	score = 0;
	system("cls");
	gotoxy(10, 10);
	printf("=== 랭 킹 ===\n");
	for (int i = 0; i < rankSize; i++) {
		printf("                    %s: %d점\n", rank[i].name, rank[i].score);
	}
	_getch();
}
