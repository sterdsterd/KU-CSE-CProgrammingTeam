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
	int isClear = 0;
	for (;;) {
		if (!isClear) {
			int k = initLobby();
			if (k == 2) continue;
			else if (k) break;
		}
		initGame();
		if (playGame() == 1) {
			gameOver();
			isClear = 0;
		} else if (difficulty <= 2) { //playGame() is always 2
			difficulty++;
			score += 200;
			for (int i = 0; i < mapSize; i++) {
				free(map[i]);
			}
			free(map);
			isClear = 1;
		} else {
			gameClear();
			isClear = 0;
		}
	}

	return 0;
}

void initStory() {
	gotoxy((100 - strlen("스킵하려면 SPACE를 누르세요")) / 4, 1);
	printf("스킵하려면 SPACE를 누르세요");

	char* str[] = { "제 2차 세계대전에서 독일과 일본이 승리를 하게 되고",
		"전 세계는 두 국가의 지배 아래에 놓이게 된다.",
		"결국 전 세계의 모든 국가들은 한 독재자 아래에 놓여",
		"하나의 커다란 독재국가인 ‘판엠’이 건국된다.",
		"독재자는 판엠을 12개의 구역으로 나누어 관리하게 하는데",
		"각 구역에서 시민반란이 일어나 판엠의 체제가 주춤하게 된다.",
		"한 번의 거대한 시민 반란 이후 판엠은 시민들이 반란에 대한 경계심을 가질 수 있도록",
		"1년에 한번씩 ‘헝거게임’이라는 생존게임을 개최하여 시민 반란을 사전에 방지하게 된다.",
		"‘헝거게임’의 룰을 간단하다.",
		"1부터 12구역까지 각 구역마다 두명 씩 대표를 추첨으로 대표자를 선발한다.",
		"각 대표자들은 출구를 알 수 없는 함정이 가득 설치된 미로에서",
		"미로 어딘가에 숨겨져있는 성배를 찾아야 하고",
		"성배를 가장 먼저 찾은 사람이 최종 우승자가 된다.",
		"헝거게임에서는 살인이 허용되며",
		"최종적으로 성배를 찾은 1인 이외의 나머지 참가자는 모두 죽게 된다.",
		"제 5구역의 대표로 뽑힌 당신은 헝거게임에 참가하여 생존을 위해 광활한 미로 속을 거닐게 되는데...",
		"과연 살아남을 수 있을까?" };

	for (int j = 0; j < sizeof(str) / sizeof(char*); j++) {
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
	gotoxy((100 - strlen(con)) / 4, 6 + sizeof(str) / sizeof(char*) * 2);
	printf("%s", con);
	_getch();
}

int initLobby() {
	int key;
	int diff = 0;
	cls();
	setTextColor(COLOR.WHITE);
	printString(13, 5, " __  __                                         ");
	printString(13, 6, "|  `/  |                                        ");
	printString(13, 7, "| `  / | __ _ _______                           ");
	printString(13, 8, "| |`/| |/ _` |_  / _ `                          ");
	printString(13, 9, "| |  | | (_| |/ /  __/                          ");
	printString(13, 10, "|_|  |_|`__,_/___`___|              _           ");
	printString(13, 11, "            / ____|                (_)          ");
	printString(13, 12, "           | (___  _   _ _ ____   _____   _____ ");
	printString(13, 13, "            `___ `| | | | '__` ` / / ` ` / / _ `");
	printString(13, 14, "            ____) | |_| | |   ` V /| |` V /  __/");
	printString(13, 15, "           |_____/ `__,_|_|    `_/ |_| `_/ `___|");
	setTextColor(COLOR.YELLOW);
	printString(18, 18, "+-------------------------+");
	printString(17, 19, "▶ |                         | ◀");
	printString(18, 20, "+-------------------------+");
	setTextColor(COLOR.GREY);
	printString(19, 19, "EASY");
	printString(18, 22, "+-------------------------+");
	printString(18, 23, "| NORMAL                  |");
	printString(18, 24, "+-------------------------+");
	printString(18, 26, "+-------------------------+");
	printString(18, 27, "| HARD                    |");
	printString(18, 28, "+-------------------------+");
	printString(18, 30, "+-------------------------+");
	printString(18, 31, "| 도움말                  |");
	printString(18, 32, "+-------------------------+");
	printString(18, 34, "+-------------------------+");
	printString(18, 35, "| 종료                    |");
	printString(18, 36, "+-------------------------+");
	printString(18, 38, "↑/↓, ENTER 키로 메뉴 선택");
	while (1) {
		key = _getch();
		if (key == 224) {
			printString(17, 19 + 4 * diff, " ");
			printString(32, 19 + 4 * diff, " ");
			printString(18, 18 + 4 * diff, "+-------------------------+");
			printString(18, 19 + 4 * diff, "|");
			printString(31, 19 + 4 * diff, "|");
			printString(18, 20 + 4 * diff, "+-------------------------+");
			key = _getch();
			if (key == 72) {
				if (--diff < 0) diff = 4;
			} else if (key == 80) {
				if (++diff > 4) diff = 0;
			}
			setTextColor(COLOR.YELLOW);
			printString(17, 19 + 4 * diff, "▶");
			printString(32, 19 + 4 * diff, "◀");
			printString(18, 18 + 4 * diff, "+-------------------------+");
			printString(18, 19 + 4 * diff, "|");
			printString(31, 19 + 4 * diff, "|");
			printString(18, 20 + 4 * diff, "+-------------------------+");
			setTextColor(COLOR.GREY);

		} else if (key == 13) {
			switch (diff) {
			case 0:
			case 1:
			case 2:
				difficulty = diff;
				return 0;
			case 3:
				initHelp();
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

	for (int i = 0; i < 1; i++) {
		int x = rand() % (mapSize - 2) + 1;
		int y = rand() % (mapSize - 2) + 1;
		if (map[x][y].category != CATEGORY.BLANK) {
			i--;
			continue;
		}
		charX = x;
		charY = y;
	}
}

int playGame() {
	cls();
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
		return 2;
	} else if (category == CATEGORY.INCREASE_SIGHT) {
		if (sightSize + 4 <= difficultyCons[difficulty].maxSight) {
			sightSize += 4;
		}
		printQuote("시야 증가", "보급품으로 플래시 라이트를 얻었습니다. 시야가 증가합니다.");
		map[charX + dx][charY + dy].category = CATEGORY.BLANK;
	} else if (category == CATEGORY.DECREASE_SIGHT) {
		if (sightSize - 4 >= difficultyCons[difficulty].minSight) {
			clearSight();
			sightSize -= 4;
		}
		printQuote("시야 감소", "밤이 되었습니다... 시야가 감소합니다.");
		map[charX + dx][charY + dy].category = CATEGORY.BLANK;
	} else if (category == CATEGORY.INCREASE_MOVE) {
		system("color 20");
		printQuote("체력 증가", "보급품으로 기초 식량을 얻었습니다. 기력이 회복하여 체력이 증가합니다!");
		map[charX + dx][charY + dy].category = CATEGORY.BLANK;
		moveCount += 15;
		Sleep(70);
		system("color 07");
	} else if (category == CATEGORY.DECREASE_MOVE) {
		system("color 40");
		printQuote("체력 감소", "미로 속 함정에 걸렸습니다. 주변에서 화살 세례가 이루어집니다.\n| 다행히 목숨은 건졌지만 출혈이 발생하여 체력이 감소합니다.");
		map[charX + dx][charY + dy].category = CATEGORY.BLANK;
		moveCount -= 15;
		Sleep(70);
		system("color 07");
	} else if (category == CATEGORY.HINT) {
		if (rand() % 2) {
			printQuote("힌트 발견", "");
			gotoxy(1, 45);
			printf("성배의 x좌표는 %d 입니다.", treasureX);
			map[charX + dx][charY + dy].category = CATEGORY.BLANK;
		} else {
			printQuote("힌트 발견", "");
			gotoxy(1, 45);
			printf("성배의 y좌표는 %d 입니다.", treasureY);
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
	printf("남은 체력: %d  ", moveCount);
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
	gotoxy(startX - 1, startY - 3);
	printf("좌표: (%d, %d)   ", charX, charY);
	gotoxy(startX - 1, startY - 2);
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

void initRank() {
	gotoxy(10, 20);
	printf("SCORE: %d", score);

	if (rankSize == 10 && rank[9].score < score) {
		printString(10, 21, "NAME : ");
		scanf("%s", &rank[9].name);
		rank[9].score = score;
	}
	else if (rankSize < 10) {
		printString(10, 21, "NAME : ");
		scanf("%s", &rank[rankSize].name);
		rank[rankSize].score = score;
		rankSize++;
	}

	sort(rank, rankSize);
	score = 0;
	cls();
	printString(15, 10, " _____             _    _             ");
	printString(15, 11, "|  __ `           | |  (_)            ");
	printString(15, 12, "| |__) |__ _ _ __ | | ___ _ __   __ _ ");
	printString(15, 13, "|  _  // _` | '_ `| |/ / | '_ ` / _` |");
	printString(15, 14, "| | ` ` (_| | | | |   <| | | | | (_| |");
	printString(15, 15, "|_|  `_`__,_|_| |_|_|`_`_|_| |_|`__, |");
	printString(15, 16, "                                 __/ |");
	printString(15, 17, "                                |___/ ");

	for (int i = 0; i < rankSize; i++) {
		gotoxy(15, 19 + i);
		printf("%s: %d점\n", rank[i].name, rank[i].score);
	}
	_getch();
}

void gameClear() {
	cls();
	
	for (int i = 0; i < mapSize; i++) {
		free(map[i]);
	}
	free(map);

	printString(15, 10, "          _ _    _____ _                 ");
	printString(15, 11, "    /\   | | |  / ____| |                ");
	printString(15, 12, "   /  \  | | | | |    | | ___  __ _ _ __ ");
	printString(15, 13, "  / /\ \ | | | | |    | |/ _ \/ _` | '__|");
	printString(15, 14, " / ____ \| | | | |____| |  __/ (_| | |   ");
	printString(15, 15, "/_/    \_\_|_|  \_____|_|\___|\__,_|_|   ");

	initRank();
                                
}

void gameOver() {

	// 동적할당 해제
	for (int i = 0; i < mapSize; i++) {
		free(map[i]);
	}
	free(map);

	cls();
	
	printString(11, 10, "  _____                         ____");
	printString(11, 11, " / ____|                       / __ `");
	printString(11, 12, "| |  __  __ _ _ __ ___   ___  | |  | |_   _____ _ __");
	printString(11, 13, "| | |_ |/ _` | '_ ` _ ` / _ ` | |  | ` ` / / _ ` '__|");
	printString(11, 14, "| |__| | (_| | | | | | |  __/ | |__| |` V /  __/ |");
	printString(11, 15, " `_____|`__,_|_| |_| |_|`___|  `____/  `_/ `___|_|");

	printString(11, 17, "음식의 섭취 없이 계속 걷다 보니 눈 앞이 흐려집니다...");
	printString(17, 18, "탈수 증세로 당신이 쓰러졌습니다.");

	initRank();
}

void initHelp() {
	cls();
	printString(19, 10, " _    _      _");
	printString(19, 11, "| |  | |    | |");
	printString(19, 12, "| |__| | ___| |_ __");
	printString(19, 13, "|  __  |/ _ ` | '_ ` ");
	printString(19, 14, "| |  | |  __/ | |_) |");
	printString(19, 15, "|_|  |_|`___|_| .__/");
	printString(19, 16, "              | |");
	printString(19, 17, "              |_|    ");


	_getch();
}