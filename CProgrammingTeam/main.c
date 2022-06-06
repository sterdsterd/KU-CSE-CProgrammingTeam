#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include "common.h"
#include "console.h"

Object** map = NULL;
Score rank[10];
int difficulty;
const Difficulty difficultyCons[3] = {
	// EASY
	{.mapSize = 41, .sightSize = 11, .bombAmount = 20, .moveCount = 100, .minSight = 7, .maxSight = 19},
	// NORMAL
	{.mapSize = 71, .sightSize = 21, .bombAmount = 30, .moveCount = 150, .minSight = 13, .maxSight = 29},
	//HARD
	{.mapSize = 101, .sightSize = 31, .bombAmount = 50, .moveCount = 200, .minSight = 23, .maxSight = 39}
};
int charX, charY, mapSize, sightSize, bombAmount, moveCount;
int consoleX = 100, consoleY = 50;
int score = 0, rankSize = 0;

int main() {
	hideConsoleCursor();
	setConsoleSize(consoleX, consoleY);
	//initStory();
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

void initStory() {
	char str[] = "2075년, 인간의 무분별한 자원 사용으로 지구의 자원은 점점 고갈되기 시작한다.\n자원의 부족으로 사람들은 점차 죽어나가고\n몇몇 나라에서는 몇 안되는 자원을 약탈하기 위해 전쟁을 일삼게 된다.\n점점 심각해지는 상황 속에서 미 항공 우주국 NASA에서는\n화성 탐사 로봇인 “Perseverance”가 화성 내부에서 정체불명의 원석을 발견했음을 알게된다.\n이는 지구의 석탄, 석유보다 몇 배 이상의 효용성을 가지는 자원으로 판명되었고\nNASA에서는 즉각 이 자원을 대량으로 얻어오기 위해 화성탐사팀을 꾸리게 되는데...";
	printSequence(str);
	_getch();
}

int initLobby() {
	int key;
	int diff = 0;
	system("cls");
	gotoxy(10, 9);
	printSequence("난이도를  선택하세요");
	printString(10, 11, "+-------------------------+");
	printString(10, 12, "| EASY                  < |");
	printString(10, 13, "+-------------------------+");
	printString(10, 15, "+-------------------------+");
	printString(10, 16, "| NORMAL                  |");
	printString(10, 17, "+-------------------------+");
	printString(10, 19, "+-------------------------+");
	printString(10, 20, "| HARD                    |");
	printString(10, 21, "+-------------------------+");
	printString(10, 23, "+-------------------------+");
	printString(10, 24, "| 도움말                  |");
	printString(10, 25, "+-------------------------+");
	printString(10, 27, "+-------------------------+");
	printString(10, 28, "| 종료                    |");
	printString(10, 29, "+-------------------------+");
	while (1) {
		key = _getch();
		if (key == 224) {
			printString(22, 12 + 4 * diff, " ");
			key = _getch();
			if (key == 72) {
				if (--diff < 0) diff = 4;
			} else if (key == 80) {
				if (++diff > 4) diff = 0;
			}
			printString(22, 12 + 4 * diff, "<");
		} else if (key == 13) {
			switch (diff) {
			case 0:
			case 1:
			case 2:
				difficulty = diff;
				return 0;
			case 3:
				return 2;
			case 4:
				return 1;
			}

		}
	}
}

void generateMaze() {
	// 길을 다 막아버리는 작업
	for (int y = 0; y < mapSize; y++) {
		for (int x = 0; x < mapSize; x++) {
			if (x % 2 == 0 || y % 2 == 0)
				map[x][y].category = 'W';
			else
				map[x][y].category = '.';
		}
	}

	// 길을 반반 확률로 뚫는 작업
	for (int y = 0; y < mapSize; y++) {
		int count = 1;  // 연속해서 몇 개의 오른쪽 벽을 길로 뚫었는지
		for (int x = 0; x < mapSize; x++) {
			if (x % 2 == 0 || y % 2 == 0)
				continue;

			if (x == mapSize - 2 && y == mapSize - 2)
				continue;

			if (y == mapSize - 2) {
				map[x + 1][y].category = '.';
				continue;
			}

			if (x == mapSize - 2) {
				map[x][y + 1].category = '.';
				continue;
			}

			if (rand() % 2 == 0) {
				map[x + 1][y].category = '.';
				count++;
			} else {
				int randomIndex = rand() % count;
				map[x - randomIndex * 2][y + 1].category = '.';  // 아래 뚫기
				count = 1;
			}
		}
	}
}

void generateItem(int amount, char category) {
	for (int i = 0; i < amount; i++) {
		int x = rand() % (mapSize - 2) + 1;
		int y = rand() % (mapSize - 2) + 1;
		if (map[x][y].category != '.') {
			i--;
			continue;
		}
		map[x][y].category = category;
		map[x][y].isActive = 1;
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

	generateMaze();

	generateItem(20, 'S');
	generateItem(20, 's');
	generateItem(20, 'M');
	generateItem(20, 'm');
	generateItem(1, 'T');

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
		else continue;

		// 충돌 체크
		int c = collisionCheck(map, dx, dy);
		if (c == -1) continue;
		else if (c) return c;

		printSight();
	}
}

int collisionCheck(Object** map, int dx, int dy) {
	switch (map[charX + dx][charY + dy].category) {
	case 'W':
		return -1;

	case 'T':
		printQuote("알림", "보물을 찾았습니다.");
		return 2;

	case 'B':
		printQuote("알림", "폭탄을 밟았습니다.");
		return 1;

	case 'S':
		if (sightSize + 4 <= difficultyCons[difficulty].maxSight) {
			sightSize += 4;
			printQuote("알림", "시야가 증가되었습니다.");
		} else {
			printQuote("알림", "최대 시야");
		}
		map[charX + dx][charY + dy].category = '.';
		break;

	case 's':
		if (sightSize - 4 >= difficultyCons[difficulty].minSight) {
			clearSight();
			sightSize -= 4;
			printQuote("알림", "시야가 감소되었습니다.");
		} else {
			printQuote("알림", "최소 시야");
		}
		map[charX + dx][charY + dy].category = '.';
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
			else if (x == charX && y == charY) {
				printf("◆");
			}
			// 맵 안 오브젝트 표시
			else categoryToChar(map[x][y].category);
		}
		gotoxy(startX - 1, startY + i);
	}
	gotoxy(startX - 1, startY + sightSize + 1);
	printf("좌표: (%d, %d)   ", charX, charY);
	gotoxy(startX - 1, startY + sightSize + 2);
	printf("점수: %d", score);
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

	if (rankSize == 10 && rank[9].score < score) {
		printString(10, 14, "NAME : ");
		scanf("%s", &rank[9].name);
		rank[9].score = score;
	} else if (rankSize < 10) {
		printString(10, 14, "NAME : ");
		scanf("%s", &rank[rankSize].name);
		rank[rankSize].score = score;
		rankSize++;
	}

	sort(rank, rankSize);
	score = 0;
	system("cls");
	gotoxy(10, 10);
	printf("=== 랭 킹 ===\n");
	for (int i = 0; i < rankSize; i++) {
		printf("                    %s: %d점\n", rank[i].name, rank[i].score);
	}
	_getch();
}