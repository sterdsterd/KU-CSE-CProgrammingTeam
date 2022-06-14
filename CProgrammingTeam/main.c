#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>
#include <Windows.h>
#include "common.h"
#include "console.h"
#include "game.h"

int main() {
	Game* game = NULL;
	int isClear = 0;
	srand((unsigned int)time(NULL));
	hideConsoleObjects();
	setConsoleSize(CONSOLE_X, CONSOLE_Y);
	initStory();
	for (;;) {
		if (!isClear) {
   			int diff = initLobby();
			// 랭킹 또는 도움말 선택 시
			if (diff == 3 || diff == 4) continue;
			// 게임 종료 선택 시
			else if (diff == 5) break;
			else {
				game = new_Game(diff, 0);
			}
		}
		initGame(&game);
		// 게임 오버 시
		if (playGame(&game) == 1) {
			gameOver(&game);
			isClear = 0;
		// EASY, NORMAL 난이도에서 게임 클리어 시
		} else if (game->difficulty < 2) { //playGame() is always 2
			int difficulty = game->difficulty;
			int score = game->score;
			// 맵 동적할당 해제
			destroyMap(&game, &(difficultyCons[game->difficulty].mapSize));
			isClear = 1;
			cls();
			printQuote("알림", "성배를 찾았습니다. 다음 레벨로 이동합니다.\n │ 계속하려면 아무 키나 누르세요.");
			_getch();

			// 다음 난이도로
			free(game);
			game = new_Game(difficulty + 1, score + 200);
		// HARD 난이도에서 게임 클리어 시
		} else {
			gameClear(&game);
			isClear = 0;
		}
	}
	
	return 0;
}

void initStory() {
	char* str[] = { "제 2차 세계대전에서 독일과 일본이 승리를 하게 되고",
	"전 세계는 두 국가의 지배 아래에 놓이게 된다.",
	"결국 전 세계의 모든 국가들은 한 독재자 아래에 놓여",
	"하나의 커다란 독재국가인 '판엠'이 건국된다.",
	"독재자는 판엠을 12개의 구역으로 나누어 관리하게 하는데",
	"각 구역에서 시민반란이 일어나 판엠의 체제가 주춤하게 된다.",
	"한 번의 거대한 시민 반란 이후 판엠은 시민들이 반란에 대한 경계심을 가질 수 있도록",
	"1년에 한번씩 '헝거게임'이라는 생존게임을 개최하여 시민 반란을 사전에 방지하게 된다.",
	"'헝거게임'의 룰을 간단하다.",
	"1부터 12구역까지 각 구역마다 두명 씩 대표를 추첨으로 대표자를 선발한다.",
	"각 대표자들은 출구를 알 수 없는 함정이 가득 설치된 미로에서",
	"미로 어딘가에 숨겨져있는 성배를 찾아야 하고",
	"성배를 가장 먼저 찾은 사람이 최종 우승자가 된다.",
	"헝거게임에서는 살인이 허용되며",
	"최종적으로 성배를 찾은 1인 이외의 나머지 참가자는 모두 죽게 된다.",
	"제 5구역의 대표로 뽑힌 당신은 헝거게임에 참가하여 생존을 위해 광활한 미로 속을 거닐게 되는데...",
	"과연 살아남을 수 있을까?" };

	setTextColor(COLOR.GREY);
	gotoxy((100 - strlen("스킵하려면 SPACE를 누르세요")) / 4, 1);
	printf("스킵하려면 SPACE를 누르세요");

	setTextColor(COLOR.WHITE);

	// 한 글자씩 출력
	// 스페이스 입력 시 남은 스트링 한 번에 출력
	for (int j = 0; j < sizeof(str) / sizeof(char*); j++) {
		int i;
		gotoxy((100 - strlen(str[j])) / 4, 5 + j * 2);
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

	setTextColor(COLOR.GREY);
	gotoxy((100 - strlen(" 계속하려면 아무 키나 누르세요")) / 4, 7 + sizeof(str) / sizeof(char*) * 2);
	printf("%s", " 계속하려면 아무 키나 누르세요");
	_getch();
}

int initLobby() {
	int key;
	int diff = 0; //현재 선택된 메뉴 인덱스
	cls();
	setTextColor(COLOR.RED);
	printString(13, 5, " __  __                                         ");
	printString(13, 6, "|  \\/  |                                        ");
	printString(13, 7, "| \\  / | __ _ _______                           ");
	printString(13, 8, "| |\\/| |/ _` |_  / _ \\                          ");
	printString(13, 9, "| |  | | (_| |/ /  __/                          ");
	printString(13, 10, "|_|  |_|\\__,_/___\\___|              _           ");
	printString(13, 11, "            / ____|                (_)          ");
	printString(13, 12, "           | (___  _   _ _ ____   _____   _____ ");
	printString(13, 13, "            \\___ \\| | | | '__\\ \\ / / \\ \\ / / _ \\");
	printString(13, 14, "            ____) | |_| | |   \\ V /| |\\ V /  __/");
	printString(13, 15, "           |_____/ \\__,_|_|    \\_/ |_| \\_/ \\___|"); 
	setTextColor(COLOR.RED);
	printString(17, 19, ">");
	printString(32, 19, "<");
	printString(18, 18, "┏━━━━━━━━━━━━━━━━━━━━━━━━━┓");
	printString(18, 19, "┃                         ┃");
	printString(18, 20, "┗━━━━━━━━━━━━━━━━━━━━━━━━━┛");
	setTextColor(COLOR.GREY);
	printString(23, 19, " EASY");
	printString(18, 22, "┌─────────────────────────┐");
	printString(18, 23, "│         NORMAL          │");
	printString(18, 24, "└─────────────────────────┘");
	printString(18, 26, "┌─────────────────────────┐");
	printString(18, 27, "│          HARD           │");
	printString(18, 28, "└─────────────────────────┘");
	printString(18, 30, "┌─────────────────────────┐");
	printString(18, 31, "│          랭킹           │");
	printString(18, 32, "└─────────────────────────┘");
	printString(18, 34, "┌─────────────────────────┐");
	printString(18, 35, "│         도움말          │");
	printString(18, 36, "└─────────────────────────┘");
	printString(18, 38, "┌─────────────────────────┐");
	printString(18, 39, "│          종료           │");
	printString(18, 40, "└─────────────────────────┘");
	printString(18, 42, " ↑/↓, ENTER 키로 메뉴 선택");
	while (1) {
		key = _getch();
		if (key == 224) {
			printString(17, 19 + 4 * diff, " ");
			printString(32, 19 + 4 * diff, " ");
			printString(18, 18 + 4 * diff, "┌─────────────────────────┐");
			printString(18, 19 + 4 * diff, "│ ");
			printString(31, 19 + 4 * diff, "│");
			printString(18, 20 + 4 * diff, "└─────────────────────────┘");
			key = _getch();
			if (key == 72) {
				if (--diff < 0) diff = 5;
			} else if (key == 80) {
				if (++diff > 5) diff = 0;
			}
			setTextColor(COLOR.RED);
			printString(17, 19 + 4 * diff, ">");
			printString(32, 19 + 4 * diff, "<");
			printString(18, 18 + 4 * diff, "┏━━━━━━━━━━━━━━━━━━━━━━━━━┓");
			printString(18, 19 + 4 * diff, "┃");
			printString(31, 19 + 4 * diff, "┃");
			printString(18, 20 + 4 * diff, "┗━━━━━━━━━━━━━━━━━━━━━━━━━┛");
			setTextColor(COLOR.GREY);

		} else if (key == 13) {
			switch (diff) {
			case 3:
				initRank(NULL);
				break;
			case 4:
				initHelp();
				break;
			}
			return diff;
		}
	}
}

void generateMaze(Game** game) {
	int x, y;

	for (y = 0; y < (*game)->mapSize; y++) {
		for (x = 0; x < (*game)->mapSize; x++) {
			if (x % 2 == 0 || y % 2 == 0)
				((*game)->map)[x][y].category = CATEGORY.WALL;
			else
				((*game)->map)[x][y].category = CATEGORY.BLANK;
		}
	}

	for (y = 0; y < (*game)->mapSize; y++) {
		int count = 1;
		for (int x = 0; x < (*game)->mapSize; x++) {
			if (x % 2 == 0 || y % 2 == 0)
				continue;

			if (x == (*game)->mapSize - 2 && y == (*game)->mapSize - 2)
				continue;

			if (y == (*game)->mapSize - 2) {
				((*game)->map)[x + 1][y].category = CATEGORY.BLANK;
				continue;
			}

			if (x == (*game)->mapSize - 2) {
				((*game)->map)[x][y + 1].category = CATEGORY.BLANK;
				continue;
			}

			if (rand() % 2 == 0) {
				((*game)->map)[x + 1][y].category = CATEGORY.BLANK;
				count++;
			}
			 else {
			 int randomIndex = rand() % count;
			 ((*game)->map)[x - randomIndex * 2][y + 1].category = CATEGORY.BLANK;
			 count = 1;
			}
		}
	}
}

void generateItem(Game** game, int amount, char category, int max) {
	int i;
	for (i = 0; i < amount; i++) {
		int x = rand() % ((*game)->mapSize - 2) + 1;
		int y = rand() % ((*game)->mapSize - 2) + 1;
		if (((*game)->map)[x][y].category != CATEGORY.BLANK) {
			i--;
			continue;
		}
		((*game)->map)[x][y].category = category;
		((*game)->map)[x][y].amount = rand() % max + 1;
		if (((*game)->map)[x][y].category == CATEGORY.INCREASE_MOVE && ((*game)->map)[x][y].amount < 10)
			((*game)->map)[x][y].amount = 10;
		if (((*game)->map)[x][y].category == CATEGORY.TREASURE) {
			(*game)->treasure.x = x;
			(*game)->treasure.y = y;
		}
		else if (((*game)->map)[x][y].category == CATEGORY.MONSTER) {
			((*game)->monsters)[i].x = x;
			((*game)->monsters)[i].y = y;
		}
	}
}

void initGame(Game** game) {
	generateMaze(game);

	generateItem(game, (*game)->objectAmount, CATEGORY.INCREASE_MOVE, (*game)->maxMoveAmount);
	generateItem(game, (*game)->objectAmount, CATEGORY.DECREASE_MOVE, (*game)->maxMoveAmount);
	generateItem(game, (*game)->objectAmount, CATEGORY.INCREASE_SIGHT, 2);
	generateItem(game, (*game)->objectAmount, CATEGORY.DECREASE_SIGHT, 2);
	generateItem(game, (*game)->objectAmount, CATEGORY.HINT, 1);
	generateItem(game, 1, CATEGORY.TREASURE, 1);
	generateItem(game, (*game)->monsterAmount, CATEGORY.MONSTER, 1);

	for (int i = 0; i < 1; i++) {
		int x = rand() % ((*game)->mapSize - 2) + 1;
		int y = rand() % ((*game)->mapSize - 2) + 1;
		if (((*game)->map)[x][y].category != CATEGORY.BLANK) {
			i--;
			continue;
		}
		(*game)->character.x = x;
		(*game)->character.y = y;
	}
}

int playGame(Game** game) {
	cls();
	printSight(game);
	char ch = 0;
	clock_t start = clock(), end;
	for (;;) {
		if (_kbhit()) {
			ch = _getch();
			int dx = 0, dy = 0;
			if (ch == 'w') dy--;
			else if (ch == 'a') dx--;
			else if (ch == 's') dy++;
			else if (ch == 'd') dx++;
			else continue;

			// 충돌 체크
			int c = collisionCheck(game, dx, dy);
			if (c == -1) continue;
			else if (c) return c;

			printSight(game);
		}
		
		if ((end = clock()) - start > 100) {
			start = clock();
			for (int i = 0; i < (*game)->monsterAmount; i++) {
				int mDx = 0, mDy = 0;
				int dx[] = { -1, 0, 0, 1 };
				int dy[] = { 0, -1, 1, 0 };
				int v[4];
				int sum = 0;
				for (int j = 0; j < 4; j++) {
					if (((*game)->map)[((*game)->monsters)[i].x + dx[j]][((*game)->monsters)[i].y + dy[j]].category == CATEGORY.WALL) {
						v[j] = 0;
					}
					else {
						v[j] = (((*game)->monsters)[i].visitedCount)[((*game)->monsters)[i].x + dx[j]][((*game)->monsters)[i].y + dy[j]];
					}
				}
				int lcmResult = lcm(v);
				for (int j = 0; j < 4; j++) {
					if (v[j] == 0) continue;
					v[j] = lcmResult / v[j];
						
					sum += v[j];
					if (j > 0) v[j] += v[j - 1];
				}

				int r = rand() % sum;
				for (int j = 0; j < 4; j++) {
					if (r < v[j]) {
						mDx = dx[j];
						mDy = dy[j];
						break;
					}
				}
				((*game)->map)[((*game)->monsters)[i].x][((*game)->monsters)[i].y].category = CATEGORY.BLANK;
				((*game)->monsters)[i].x += mDx;
				((*game)->monsters)[i].y += mDy;
				((*game)->monsters)[i].visitedCount[((*game)->monsters)[i].x][((*game)->monsters)[i].y]++;
				
				// 플레이어를 만났을 때
				if (((*game)->monsters)[i].x == (*game)->character.x && ((*game)->monsters)[i].y == (*game)->character.y) {
					(*game)->gameOverStr[0] = "다른 생존자와 대치했습니다.";
					(*game)->gameOverStr[1] = "당신은 있는 힘을 다해 상대방과 싸웠지만 결국 치명상을 얻고 사망하였습니다...";
					return 1;
				}

				// 다른 플레이어가 성배를 찾았을 때
				if (((*game)->monsters)[i].x == (*game)->treasure.x && ((*game)->monsters)[i].y == (*game)->treasure.y) {
					(*game)->gameOverStr[0] = "다른 생존자가 성배를 먼저 찾았습니다.";
					(*game)->gameOverStr[1] = "당신은 최종 승리자가 되지 못하여 사망하였습니다...";
					return 1;
				}
				for (int i = 0; i < (*game)->monsterAmount; i++) {
					((*game)->map)[((*game)->monsters)[i].x][((*game)->monsters)[i].y].category = CATEGORY.MONSTER;
				}
			}
			printSight(game);
		}
	}
}

int collisionCheck(Game** game, int dx, int dy) {
	char category = ((*game)->map)[(*game)->character.x + dx][(*game)->character.y + dy].category;
	int amount = ((*game)->map)[(*game)->character.x + dx][(*game)->character.y + dy].amount;

	if (category == CATEGORY.WALL) {
		return -1;

	} else if (category == CATEGORY.MONSTER) {
		(*game)->gameOverStr[0] = "다른 생존자와 대치했습니다.";
		(*game)->gameOverStr[1] = "당신은 있는 힘을 다해 상대방과 싸웠지만 결국 치명상을 얻고 사망하였습니다...";
		return 1;

	} else if (category == CATEGORY.TREASURE) {
		return 2;

	} else if (category == CATEGORY.INCREASE_SIGHT) {
		if ((*game)->sightSize + amount * 2 <= (*game)->maxSight) {
			(*game)->sightSize += amount * 2;
		} else (*game)->sightSize = (*game)->maxSight;
		printQuote("시야 증가", "");
		printf("보급품으로 플래시 라이트를 얻었습니다. 시야가 ");
		setTextColor(COLOR.GREEN);
		printf("%d만큼 증가", amount * 2);
		setTextColor(COLOR.GREY);
		printf("합니다!");
		((*game)->map)[(*game)->character.x + dx][(*game)->character.y + dy].category = CATEGORY.BLANK;

	} else if (category == CATEGORY.DECREASE_SIGHT) {
		cls();
		if ((*game)->sightSize - amount * 2 >= (*game)->minSight) {
			(*game)->sightSize -= amount * 2;
		} else (*game)->sightSize = (*game)->minSight;
		printQuote("시야 감소", "");
		printf("밤이 되었습니다... 시야가 ");
		setTextColor(COLOR.RED);
		printf("%d만큼 감소", amount * 2);
		setTextColor(COLOR.GREY);
		printf("합니다.");
		((*game)->map)[(*game)->character.x + dx][(*game)->character.y + dy].category = CATEGORY.BLANK;

	} else if (category == CATEGORY.INCREASE_MOVE) {
		system("color 20");
		Sleep(50);
		system("color 07");
		system("color 07");
		printQuote("체력 증가", "");
		printf("보급품으로 기초 식량을 얻었습니다. 기력이 회복하여 체력이 ");
		setTextColor(COLOR.GREEN);
		printf("%d만큼 증가", amount);
		setTextColor(COLOR.GREY);
		printf("합니다!");
		((*game)->map)[(*game)->character.x + dx][(*game)->character.y + dy].category = CATEGORY.BLANK;
		(*game)->moveCount += amount;

	} else if (category == CATEGORY.DECREASE_MOVE) {
		system("color 40");
		Sleep(50);
		system("color 07");
		system("color 07");
		printQuote("체력 감소", "");
		printf("미로 속 함정에 걸렸습니다. 주변에서 화살 세례가 이루어집니다.\n │ 다행히 목숨은 건졌지만 출혈이 발생하여 체력이 ");
		setTextColor(COLOR.RED);
		printf("%d만큼 감소", amount);
		setTextColor(COLOR.GREY);
		printf("합니다.");
		((*game)->map)[(*game)->character.x + dx][(*game)->character.y + dy].category = CATEGORY.BLANK;
		(*game)->moveCount -= amount;

	} else if (category == CATEGORY.HINT) {
		if (rand() % 2) {
			printQuote("힌트 발견", "");
			printf("성배의 x좌표는 %d 입니다.", (*game)->treasure.x);
			((*game)->map)[(*game)->character.x + dx][(*game)->character.y + dy].category = CATEGORY.BLANK;

		} else {
			printQuote("힌트 발견", "");
			printf("성배의 y좌표는 %d 입니다.", (*game)->treasure.y);
			((*game)->map)[(*game)->character.x + dx][(*game)->character.y + dy].category = CATEGORY.BLANK;
			
		}
	}

	(*game)->character.x += dx;
	(*game)->character.y += dy;
	(*game)->score++;
	// 이동횟수 감소 및 없으면 게임 오버
	if (--((*game)->moveCount) < 0) {
		(*game)->gameOverStr[0] = "음식의 섭취 없이 계속 걷다 보니 눈 앞이 흐려집니다...";
		(*game)->gameOverStr[1] = "탈수 증세로 당신이 쓰러졌습니다.";
		return 1;
	}

	return 0;
}

void printSight(Game** game) {
	gotoxy(0, 0);
	printf("난이도: %s", (*game)->difficulty == 0 ? "EASY" : (*game)->difficulty == 1 ? "NORMAL" : "HARD");
	gotoxy(0, 1);
	printf("체  력: %d  ", (*game)->moveCount);

	// startX, startY = 맵 프린트 시작 위치
	int startX = (CONSOLE_X / 2 - (*game)->sightSize) / 2, startY = (CONSOLE_Y - (*game)->sightSize) / 2;
	gotoxy(startX - 1, startY - 1);

	for (int y = (*game)->character.y - (*game)->sightSize / 2 - 1, i = 0; y <= (*game)->character.y + (*game)->sightSize / 2 + 1; y++, i++) {
		for (int x = (*game)->character.x - (*game)->sightSize / 2 - 1; x <= (*game)->character.x + (*game)->sightSize / 2 + 1; x++) {
			// 시야 테두리 표시
			if (x == (*game)->character.x - (*game)->sightSize / 2 - 1 && y == (*game)->character.y - (*game)->sightSize / 2 - 1)
				printf("┏");
			else if (x == (*game)->character.x + (*game)->sightSize / 2 + 1 && y == (*game)->character.y - (*game)->sightSize / 2 - 1)
				printf("┓");
			else if (x == (*game)->character.x - (*game)->sightSize / 2 - 1 && y == (*game)->character.y + (*game)->sightSize / 2 + 1)
				printf("┗");
			else if (x == (*game)->character.x + (*game)->sightSize / 2 + 1 && y == (*game)->character.y + (*game)->sightSize / 2 + 1)
				printf("┛");
			else if (x == (*game)->character.x - (*game)->sightSize / 2 - 1 || x == (*game)->character.x + (*game)->sightSize / 2 + 1)
				printf("┃");
			else if (y == (*game)->character.y - (*game)->sightSize / 2 - 1 || y == (*game)->character.y + (*game)->sightSize / 2 + 1)
				printf("━━");
			// 맵 밖 표시
			else if (x < 0 || x > (*game)->mapSize - 1 || y < 0 || y > (*game)->mapSize - 1)
				printf("  ");
			// 플레이어 표시
			else if (x == (*game)->character.x && y == (*game)->character.y) {
				setTextColor(COLOR.RED);
				printf("♥");
				setTextColor(COLOR.GREY);
			}
			// 맵 안 오브젝트 표시
			else categoryToChar(((*game)->map)[x][y].category);
		}
		gotoxy(startX - 1, startY + i);
	}
	gotoxy(0, 2);
	printf("좌  표: (%d, %d)   ", (*game)->character.x, (*game)->character.y);
	gotoxy(44, 0);
	printf("점  수: %5d", (*game)->score);
}

void initRank(Game** game) {
	static Score rank[10];
	static int rankSize = 0;
	
	if (game != NULL) {
		gotoxy(10, 20);
		printf("SCORE: %5d", (*game)->score);

		if (rankSize == 10 && rank[9].score < (*game)->score) {
			printString(10, 21, "NAME : ");
			scanf("%9s", &rank[9].name);
			rank[9].score = (*game)->score;
		}
		else if (rankSize < 10) {
			printString(10, 21, "NAME : ");
			scanf("%9s", &rank[rankSize].name);
			rank[rankSize].score = (*game)->score;
			rankSize++;
		}

		sort(rank, rankSize);
		(*game)->score = 0;
		free(*game);
	}
	cls();
	printString(15, 10, " _____             _    _             ");
	printString(15, 11, "|  __ \\           | |  (_)            ");
	printString(15, 12, "| |__) |__ _ _ __ | | ___ _ __   __ _ ");
	printString(15, 13, "|  _  // _\\ | '_ \\| |/ / | '_ \\ / _\\ |");
	printString(15, 14, "| | \\ \\ (_| | | | |   <| | | | | (_| |");
	printString(15, 15, "|_|  \\_\\__,_|_| |_|_|\\_\\_|_| |_|\\__, |");
	printString(15, 16, "                                 __/ |");
	printString(15, 17, "                                |___/ ");

	if (rank[0].score != NULL) {
		for (int i = 0; i < rankSize; i++) {
			gotoxy(15, 19 + i);
			printf("%10s: %5d점\n", rank[i].name, rank[i].score);
		}
	} else {
		gotoxy(15, 19);
		printf("플레이 기록이 존재하지 않습니다.");
	}
	_getch();
}

void gameClear(Game** game) {
	cls();

	destroyMap(game, (*game)->mapSize);

	printString(15, 10, "          _ _    _____ _                 ");
	printString(15, 11, "    /\\   | | |  / ____| |                ");
	printString(15, 12, "   /  \\  | | | | |    | | ___  __ _ _ __ ");
	printString(15, 13, "  / /\\ \\ | | | | |    | |/ _ \\/ _\\ | '__|");
	printString(15, 14, " / ____ \\| | | | |____| |  __/ (_| | |   ");
	printString(15, 15, "/_/    \\_\\_|_|  \\_____|_|\\___|\\__,_|_|   ");

	printString(14, 17, "성배를 찾았습니다. 당신이 최종 우승자입니다!");

	(*game)->score += 500;
	initRank(game);
                                
}

// 동적할당 해제
void destroyMap(Game** game, int mapSize) {
	free(((*game)->map)[0]);
	free(((*game)->map));
	for (int i = 0; i < (*game)->monsterAmount; i++) {
		free((((*game)->monsters)[i].visitedCount)[0]);
		free(((*game)->monsters)[i].visitedCount);
	}
	//free((*game)->monsters);
}

void gameOver(Game** game) {
	cls();

	destroyMap(game, (*game)->mapSize);
	
	printString(11, 10, "  _____                         ____");
	printString(11, 11, " / ____|                       / __ \\");
	printString(11, 12, "| |  __  __ _ _ __ ___   ___  | |  | |_   _____ _ __");
	printString(11, 13, "| | |_ |/ _\\ | '_ \\ _ \\ / _ \\ | |  | \\ \\ / / _ \\ '__|");
	printString(11, 14, "| |__| | (_| | | | | | |  __/ | |__| |\\ V /  __/ |");
	printString(11, 15, " \\_____|\\__,_|_| |_| |_|\\___|  \\____/  \\_/ \\___|_|");

	printString((100 - strlen((*game)->gameOverStr[0])) / 4, 17, (*game)->gameOverStr[0]);
	printString((100 - strlen((*game)->gameOverStr[1])) / 4, 18, (*game)->gameOverStr[1]);
	
	initRank(game);
}

void initHelp() {
	cls();
	printString(19, 10, " _    _      _");
	printString(19, 11, "| |  | |    | |");
	printString(19, 12, "| |__| | ___| |_ __");
	printString(19, 13, "|  __  |/ _ \\ | '_ \\ ");
	printString(19, 14, "| |  | |  __/ | |_) |");
	printString(19, 15, "|_|  |_|\\___|_| .__/");
	printString(19, 16, "              | |");
	printString(19, 17, "              |_|    ");

	printString(10, 20, "WASD 키를 이용하여 캐릭터를 움직일 수 있습니다.");
	printString(10, 21, "W : 위로 이동");
	printString(10, 22, "A : 왼쪽으로 이동");
	printString(10, 23, "S : 아래로 이동");
	printString(10, 24, "D : 오른쪽으로 이동");

	printString(10, 26, "맵 속에 숨겨진 성배를 찾아 게임을 클리어하세요.");
	printString(10, 27, "체력이 0이 되면 게임 오버됩니다.");
	printString(10, 28, "아이템을 획득하여 시야와 체력을 줄이거나 늘릴 수 있습니다.");
	printString(10, 29, "성배의 위치를 알 수 있는 힌트를 맵 곳곳에서 발견할 수 있습니다.");

	printString(10, 31, "계속하려면 아무 키나 누르세요.");

	_getch();
}