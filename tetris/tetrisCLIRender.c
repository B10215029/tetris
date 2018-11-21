#pragma warning(disable : 4996)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tetrisCLIRender.h"

const ScreenPoint boardPosition = { 14, 3 };
const ScreenPoint holdPosition = { 2, 3 };
const ScreenPoint nextPosition = { 38, 3 };
const ScreenPoint scorePosition = { 2, 20 };

const ScreenPoint boardPosition2 = { 74, 3 };
const ScreenPoint holdPosition2 = { 62, 3 };
const ScreenPoint nextPosition2 = { 98, 3 };
const ScreenPoint scorePosition2 = { 62, 20 };

char renderBuffer[RANDER_WIDTH * RENDER_HEIGHT];

void ClearBuffer();
void RenderBoard(GameData* gameData, ScreenPoint position);
void RenderShadow(GameData* gameData, ScreenPoint position);
void RenderHold(GameData* gameData, ScreenPoint position);
void RenderNext(GameData* gameData, ScreenPoint position);
void RenderScore(GameData* gameData, ScreenPoint position);
int ShadowCheck(GameData* gameData, Point *newPosition, Point *oldPosition);

void Render(GameData* gameData) {
	ClearBuffer();

	// render data
	RenderBoard(gameData, boardPosition);
	RenderShadow(gameData, boardPosition);
	RenderHold(gameData, holdPosition);
	RenderNext(gameData, nextPosition);
	RenderScore(gameData, scorePosition);

	// print to screen
	puts(renderBuffer);
}

void Render2P(GameData* gameData1, GameData* gameData2) {
	ClearBuffer();

	// render data
	RenderBoard(gameData1, boardPosition);
	RenderShadow(gameData1, boardPosition);
	RenderHold(gameData1, holdPosition);
	RenderNext(gameData1, nextPosition);
	RenderScore(gameData1, scorePosition);
	if (gameData2 != NULL) {
		RenderBoard(gameData2, boardPosition2);
		RenderShadow(gameData2, boardPosition2);
		RenderHold(gameData2, holdPosition2);
		RenderNext(gameData2, nextPosition2);
		RenderScore(gameData2, scorePosition2);
	}

	// print to screen
	puts(renderBuffer);
}

void ClearBuffer() {
	//system("cls");
	memset(renderBuffer, 32, RANDER_WIDTH * RENDER_HEIGHT);
	for (int i = 0; i < RENDER_HEIGHT; i++) {
		renderBuffer[RANDER_WIDTH * i + RANDER_WIDTH - 1] = '\n';
	}
	renderBuffer[RANDER_WIDTH * RENDER_HEIGHT - 1] = '\0';
}

int ShadowCheck(GameData* gameData, Point *newPosition, Point *oldPosition) {
	for (int i = 0; i < 4; i++) {
		int isInBound = newPosition[i].y < BOARD_HEIGHT;
		int isEmpty = newPosition[i].y < 0 || gameData->board[newPosition[i].y][newPosition[i].x] == 0;
		int isInclude = 0;
		for (int j = 0; j < 4; j++) {
			if (newPosition[i].x == oldPosition[j].x && newPosition[i].y == oldPosition[j].y) {
				isInclude = 1;
			}
		}
		if (!isInBound || !isEmpty && !isInclude) {
			return 0;
		}
	}
	return 1;
}

void RenderBoard(GameData* gameData, ScreenPoint position) {
	// frame
	for (int i = 0; i < BOARD_HEIGHT; i++) {
		renderBuffer[ScreenPointToPosition(position) + i * RANDER_WIDTH - 2] = 161;
		renderBuffer[ScreenPointToPosition(position) + i * RANDER_WIDTH - 2 + 1] = 188;
		renderBuffer[ScreenPointToPosition(position) + i * RANDER_WIDTH + BOARD_WIDTH * 2] = 161;
		renderBuffer[ScreenPointToPosition(position) + i * RANDER_WIDTH + BOARD_WIDTH * 2 + 1] = 188;
	}
	for (int i = -1; i <= BOARD_WIDTH; i++) {
		renderBuffer[ScreenPointToPosition(position) + BOARD_HEIGHT * RANDER_WIDTH + i * 2] = 161;
		renderBuffer[ScreenPointToPosition(position) + BOARD_HEIGHT * RANDER_WIDTH + i * 2 + 1] = 188;
	}
	// block
	for (int x = 0; x < BOARD_WIDTH; x++) {
		for (int y = 0; y < BOARD_HEIGHT; y++) {
			ScreenPoint p = { x * 2, y };
			if (gameData->board[y][x] == 0) {
				//renderBuffer[ScreenPointToPosition(position) + ScreenPointToPosition(p)] = 161;
				//renderBuffer[ScreenPointToPosition(position) + ScreenPointToPosition(p) + 1] = 188;
			}
			else if (gameData->board[y][x] == 1) {
				renderBuffer[ScreenPointToPosition(position) + ScreenPointToPosition(p)] = 161;
				renderBuffer[ScreenPointToPosition(position) + ScreenPointToPosition(p) + 1] = 189;
			}
			else if (gameData->board[y][x] == 2) {
				renderBuffer[ScreenPointToPosition(position) + ScreenPointToPosition(p)] = 161;
				renderBuffer[ScreenPointToPosition(position) + ScreenPointToPosition(p) + 1] = 242;
			}
		}
	}
}

void RenderShadow(GameData* gameData, ScreenPoint position) {
	Point shadowPosition[4];
	memcpy(shadowPosition, gameData->block.data, sizeof(shadowPosition));
	while (ShadowCheck(gameData, shadowPosition, gameData->block.data)) {
		for (int i = 0; i < 4; i++) {
			shadowPosition[i].y++;
		}
	}
	for (int i = 0; i < 4; i++) {
		shadowPosition[i].y--;
		if (shadowPosition[i].y >= 0 && gameData->board[shadowPosition[i].y][shadowPosition[i].x] == 0) {
			ScreenPoint p = { shadowPosition[i].x * 2, shadowPosition[i].y };
			renderBuffer[ScreenPointToPosition(position) + ScreenPointToPosition(p)] = 162;
			renderBuffer[ScreenPointToPosition(position) + ScreenPointToPosition(p) + 1] = 194;
		}
	}
}

void RenderHold(GameData* gameData, ScreenPoint position) {
	memcpy(renderBuffer + ScreenPointToPosition(position), "¢Ö¢Ý¢Ú¢Ò", sizeof(char) * 8);
	if (gameData->holdBlock != -1) {
		int iconPosition = 2 + 3 * RANDER_WIDTH;
		for (int i = 0; i < 4; i++) {
			ScreenPoint p = { BlockData[gameData->holdBlock][0][i].x * 2, BlockData[gameData->holdBlock][0][i].y };
			renderBuffer[ScreenPointToPosition(position) + iconPosition + ScreenPointToPosition(p)] = 161;
			renderBuffer[ScreenPointToPosition(position) + iconPosition + ScreenPointToPosition(p) + 1] = 189;
		}
	}
}

void RenderNext(GameData* gameData, ScreenPoint position) {
	memcpy(renderBuffer + ScreenPointToPosition(position), "¢Ü¢Ó¢æ¢â", sizeof(char) * 8);
	int iconPosition = 2 + 3 * RANDER_WIDTH;
	for (int blockI = 0; blockI < 5; blockI++) {
		for (int i = 0; i < 4; i++) {
			ScreenPoint p = { BlockData[gameData->blockQueue[gameData->blockQueueIndex + blockI]][0][i].x * 2, BlockData[gameData->blockQueue[gameData->blockQueueIndex + blockI]][0][i].y };
			renderBuffer[ScreenPointToPosition(position) + iconPosition + ScreenPointToPosition(p)] = 161;
			renderBuffer[ScreenPointToPosition(position) + iconPosition + ScreenPointToPosition(p) + 1] = 189;
		}
		iconPosition += RANDER_WIDTH * 3;
	}
}

void RenderScore(GameData* gameData, ScreenPoint position) {
	char scoreBuffer[10];
	strcpy(scoreBuffer, "LINE:");
	itoa(gameData->score, scoreBuffer + strlen(scoreBuffer), 10);
	memcpy(renderBuffer + (ScreenPointToPosition(position) + RANDER_WIDTH * 0), scoreBuffer, sizeof(char) * strlen(scoreBuffer));

	strcpy(scoreBuffer, "COMBO:");
	itoa(gameData->combo, scoreBuffer + strlen(scoreBuffer), 10);
	memcpy(renderBuffer + (ScreenPointToPosition(position) + RANDER_WIDTH * 1), scoreBuffer, sizeof(char) * strlen(scoreBuffer));

	if (gameData->gameOver) {
		memcpy(renderBuffer + (ScreenPointToPosition(position) + RANDER_WIDTH * 3), "Game Over", sizeof(char) * strlen("Game Over"));
	}
}
