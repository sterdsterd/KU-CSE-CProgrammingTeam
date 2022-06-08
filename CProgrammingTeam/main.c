#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include "common.h"

Object** map = NULL;
Score rank[10];
int difficulty;
const Difficulty difficultyCons[3] = {
	// EASY
	{.mapSize = 41, .sightSize = 11, .moveCount = 100, .minSight = 7, .maxSight = 19},
	// NORMAL
	{.mapSize = 71, .sightSize = 21, .moveCount = 150, .minSight = 13, .maxSight = 29},
	// HARD
	{.mapSize = 101, .sightSize = 31, .moveCount = 200, .minSight = 23, .maxSight = 39}
};
int charX, charY, mapSize, sightSize, moveCount;
int consoleX = 100, consoleY = 50;
int score = 0, rankSize = 0;
int treasureX, treasureY;

int main() {
	srand((unsigned int)time(NULL));
	hideConsoleCursor();
	setConsoleSize(consoleX, consoleY);
	initStory();
	if (initLobby()) return 0;
	for (;;) {
		initGame();
		if (playGame() == 1) {
			gameOver();
			if (initLobby()) break;
		} else if (difficulty <= 2) {
			difficulty++;
			score += 200;
			for (int i = 0; i < mapSize; i++) {
				free(map[i]);
			}
			free(map);
		} else {
			//gameClear();
		}
	}

	return 0;
}

void initStory() {
	gotoxy((100 - strlen("스킵하려면 SPACE를 누르세요")) / 4, 1);
	printf("스킵하려면 SPACE를 누르세요");

	char* str[] = { "2075년, 인간의 무분별한 자원 사용으로 지구의 자원은 점점 고갈되기 시작한다.",
		"자원의 부족으로 사람들은 점차 죽어나가고",
		"몇몇 나라에서는 몇 안되는 자원을 약탈하기 위해 전쟁을 일삼게 된다.",
		"점점 심각해지는 상황 속에서 미 항공 우주국 NASA에서는",
		"화성 탐사 로봇인 “Perseverance”가 화성 내부에서 정체불명의 원석을 발견했음을 알게된다.",
		"이는 지구의 석탄, 석유보다 몇 배 이상의 효용성을 가지는 자원으로 판명되었고",
		"NASA에서는 즉각 이 자원을 대량으로 얻어오기 위해 화성탐사팀을 꾸리게 되는데..." };

	for (int j = 0; j < 7; j++) {
		gotoxy((100 - strlen(str[j])) / 4, 5 + j * 2);
		int i;
		for (i = 0; i < strlen(str[j]); i++) {
			if (_kbhit() && _getch() == ' ') {
				break;
			}
			printf("%c", str[j][i]);
			Sleep(50);
		}
		char* substr = (char*)malloc(sizeof(char) * (strlen(str[j]) - i + 2));
		strncpy(substr, str[j] + i, strlen(str[j]) - i + 1);
		substr[strlen(str[j]) - i + 1] = 0;
		printf("%s", substr);
		printf("\n");
	}

	char con[] = "계속하려면 아무 키나 누르세요";
	gotoxy((100 - strlen(con)) / 4, 5 + 8 * 2);
	printf("%s", con);
	_getch();
}

int initLobby() {
	int key;
	int diff = 0;
	system("cls");
	gotoxy(20, 9);
	printSequence("난이도를 선택하세요");
	setTextColor(COLOR.YELLOW);
	printString(18, 11, "+-------------------------+");
	printString(17, 12, "▶ |                         | ◀");
	printString(18, 13, "+-------------------------+");
	setTextColor(COLOR.GREY);
	printString(19, 12, "EASY");
	Sleep(100);
	printString(18, 15, "+-------------------------+");
	printString(18, 16, "| NORMAL                  |");
	printString(18, 17, "+-------------------------+");
	Sleep(100);
	printString(18, 19, "+-------------------------+");
	printString(18, 20, "| HARD                    |");
	printString(18, 21, "+-------------------------+");
	Sleep(100);
	printString(18, 23, "+-------------------------+");
	printString(18, 24, "| 도움말                  |");
	printString(18, 25, "+-------------------------+");
	Sleep(100);
	printString(18, 27, "+-------------------------+");
	printString(18, 28, "| 종료                    |");
	printString(18, 29, "+-------------------------+");
	printString(18, 31, "↑/↓, ENTER 키로 메뉴 선택");
	while (1) {
		key = _getch();
		if (key == 224) {
			printString(17, 12 + 4 * diff, " ");
			printString(32, 12 + 4 * diff, " ");
			printString(18, 11 + 4 * diff, "+-------------------------+");
			printString(18, 12 + 4 * diff, "|");
			printString(31, 12 + 4 * diff, "|");
			printString(18, 13 + 4 * diff, "+-------------------------+");
			key = _getch();
			if (key == 72) {
				if (--diff < 0) diff = 4;
			} else if (key == 80) {
				if (++diff > 4) diff = 0;
			}
			setTextColor(COLOR.YELLOW);
			printString(17, 12 + 4 * diff, "▶");
			printString(32, 12 + 4 * diff, "◀");
			printString(18, 11 + 4 * diff, "+-------------------------+");
			printString(18, 12 + 4 * diff, "|");
			printString(31, 12 + 4 * diff, "|");
			printString(18, 13 + 4 * diff, "+-------------------------+");
			setTextColor(COLOR.GREY);

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

	for (int y = 0; y < mapSize; y++) {
		for (int x = 0; x < mapSize; x++) {
			if (x % 2 == 0 || y % 2 == 0)
				map[x][y].category = CATEGORY.WALL;
			else
				map[x][y].category = CATEGORY.BLANK;
		}
	}

	for (int y = 0; y < mapSize; y++) {
		int count = 1;
		for (int x = 0; x < mapSize; x++) {
			if (x % 2 == 0 || y % 2 == 0)
				continue;

			if (x == mapSize - 2 && y == mapSize - 2)
				continue;

			if (y == mapSize - 2) {
				map[x + 1][y].category = CATEGORY.BLANK;
				continue;
			}

			if (x == mapSize - 2) {
				map[x][y + 1].category = CATEGORY.BLANK;
				continue;
			}

			if (rand() % 2 == 0) {
				map[x + 1][y].category = CATEGORY.BLANK;
				count++;
			} else {
				int randomIndex = rand() % count;
				map[x - randomIndex * 2][y + 1].category = CATEGORY.BLANK;
				count = 1;
			}
		}
	}
}

void generateItem(int amount, char category) {
	for (int i = 0; i < amount; i++) {
		int x = rand() % (mapSize - 2) + 1;
		int y = rand() % (mapSize - 2) + 1;
		if (map[x][y].category != CATEGORY.BLANK) {
			i--;
			continue;
		}
		map[x][y].category = category;
		map[x][y].isActive = 1;
		if (map[x][y].category == CATEGORY.TREASURE) {
			treasureX = x;
			treasureY = y;
		}
	}
}

void initGame() {

	mapSize = difficultyCons[difficulty].mapSize;
	sightSize = difficultyCons[difficulty].sightSize;
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

	generateItem(20, CATEGORY.INCREASE_MOVE);
	generateItem(20, CATEGORY.DECREASE_MOVE);
	generateItem(20, CATEGORY.INCREASE_SIGHT);
	generateItem(20, CATEGORY.DECREASE_SIGHT);
	generateItem(20, CATEGORY.HINT);
	generateItem(1, CATEGORY.TREASURE);

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
	char category = map[charX + dx][charY + dy].category;

	if (category == CATEGORY.WALL) {
		return -1;
	} else if (category == CATEGORY.TREASURE) {
		printQuote("알림", "보물을 찾았습니다.");
		return 2;
	} else if (category == CATEGORY.INCREASE_SIGHT) {
		if (sightSize + 4 <= difficultyCons[difficulty].maxSight) {
			sightSize += 4;
			printQuote("알림", "시야가 증가되었습니다.");
		}
		else {
			printQuote("알림", "최대 시야");
		}
		map[charX + dx][charY + dy].category = CATEGORY.BLANK;
	} else if (category == CATEGORY.DECREASE_SIGHT) {
		if (sightSize - 4 >= difficultyCons[difficulty].minSight) {
			clearSight();
			sightSize -= 4;
			printQuote("알림", "시야가 감소되었습니다.");
		}
		else {
			printQuote("알림", "최소 시야");
		}
		map[charX + dx][charY + dy].category = CATEGORY.BLANK;
	} else if (category == CATEGORY.INCREASE_MOVE) {
		printQuote("알림", "이동 횟수가 증가되었습니다.");
		map[charX + dx][charY + dy].category = CATEGORY.BLANK;
		moveCount += 15;
	} else if (category == CATEGORY.DECREASE_MOVE) {
		printQuote("알림", "이동 횟수가 감소되었습니다.");
		map[charX + dx][charY + dy].category = CATEGORY.BLANK;
		moveCount -= 15;
	} else if (category == CATEGORY.HINT) {
		if (rand() % 2) {
			printQuote("힌트", "");
			gotoxy(1, 43);
			printf("보물의 x좌표는 %d 입니다.", treasureX);
			map[charX + dx][charY + dy].category = CATEGORY.BLANK;
		} else {
			printQuote("힌트", "");
			gotoxy(1, 43);
			printf("보물의 y좌표는 %d 입니다.", treasureY);
			map[charX + dx][charY + dy].category = CATEGORY.BLANK;
		}
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

void gameClear() {
	system("cls");
	gotoxy(0, 0);
	printf("클리어");
	_getch();
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