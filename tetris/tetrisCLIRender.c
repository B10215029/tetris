#pragma warning(disable : 4996)
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tetrisCLIRender.h"

const ScreenPoint boardPosition = { 14, 3 };
const ScreenPoint holdPosition = { 2, 3 };
const ScreenPoint nextPosition = { 38, 3 };
const ScreenPoint scorePosition = { 2, 20 };
char renderBuffer[RANDER_WIDTH * RENDER_HEIGHT];

void Render(GameData* gameData) {
	//system("cls");
	// clear renderBuffer
	memset(renderBuffer, 32, RANDER_WIDTH * RENDER_HEIGHT);
	for (int i = 0; i < RENDER_HEIGHT; i++) {
		renderBuffer[RANDER_WIDTH * i + RANDER_WIDTH - 1] = '\n';
	}
	// render board
	for (int i = 0; i < BOARD_HEIGHT; i++) {
		renderBuffer[ScreenPointToPosition(boardPosition) + i * RANDER_WIDTH - 2] = 161;
		renderBuffer[ScreenPointToPosition(boardPosition) + i * RANDER_WIDTH - 2 + 1] = 188;
		renderBuffer[ScreenPointToPosition(boardPosition) + i * RANDER_WIDTH + BOARD_WIDTH * 2] = 161;
		renderBuffer[ScreenPointToPosition(boardPosition) + i * RANDER_WIDTH + BOARD_WIDTH * 2 + 1] = 188;
	}
	for (int i = -1; i <= BOARD_WIDTH; i++) {
		renderBuffer[ScreenPointToPosition(boardPosition) + BOARD_HEIGHT * RANDER_WIDTH + i * 2] = 161;
		renderBuffer[ScreenPointToPosition(boardPosition) + BOARD_HEIGHT * RANDER_WIDTH + i * 2 + 1] = 188;
	}
	for (int x = 0; x < BOARD_WIDTH; x++) {
		for (int y = 0; y < BOARD_HEIGHT; y++) {
			ScreenPoint p = { x * 2, y };
			if (gameData->board[y][x] == 0) {
				//renderBuffer[ScreenPointToPosition(boardPosition) + ScreenPointToPosition(p)] = 161;
				//renderBuffer[ScreenPointToPosition(boardPosition) + ScreenPointToPosition(p) + 1] = 188;
			}
			else if (gameData->board[y][x] == 1) {
				renderBuffer[ScreenPointToPosition(boardPosition) + ScreenPointToPosition(p)] = 161;
				renderBuffer[ScreenPointToPosition(boardPosition) + ScreenPointToPosition(p) + 1] = 189;
			}
			else if (gameData->board[y][x] == 2) {
				renderBuffer[ScreenPointToPosition(boardPosition) + ScreenPointToPosition(p)] = 161;
				renderBuffer[ScreenPointToPosition(boardPosition) + ScreenPointToPosition(p) + 1] = 242;
			}
		}
	}
	// render shadow
	Point shadowPosition[4];
	memcpy(shadowPosition, gameData->block.data, sizeof(shadowPosition));
	while (PositionCheck(shadowPosition, gameData->block.data)) {
		for (int i = 0; i < 4; i++) {
			shadowPosition[i].y++;
		}
	}
	for (int i = 0; i < 4; i++) {
		shadowPosition[i].y--;
		if (shadowPosition[i].y >= 0 && gameData->board[shadowPosition[i].y][shadowPosition[i].x] == 0) {
			ScreenPoint p = { shadowPosition[i].x * 2, shadowPosition[i].y };
			renderBuffer[ScreenPointToPosition(boardPosition) + ScreenPointToPosition(p)] = 162;
			renderBuffer[ScreenPointToPosition(boardPosition) + ScreenPointToPosition(p) + 1] = 174;
		}
	}
	// render hold
	memcpy(renderBuffer + ScreenPointToPosition(holdPosition), "¢Ö¢Ý¢Ú¢Ò", sizeof(char) * 8);
	if (gameData->holdBlock != -1) {
		int iconPosition = 2 + 3 * RANDER_WIDTH;
		for (int i = 0; i < 4; i++) {
			ScreenPoint p = { BlockData[gameData->holdBlock][0][i].x * 2, BlockData[gameData->holdBlock][0][i].y };
			renderBuffer[ScreenPointToPosition(holdPosition) + iconPosition + ScreenPointToPosition(p)] = 161;
			renderBuffer[ScreenPointToPosition(holdPosition) + iconPosition + ScreenPointToPosition(p) + 1] = 189;
		}
	}
	// render next
	memcpy(renderBuffer + ScreenPointToPosition(nextPosition), "¢Ü¢Ó¢æ¢â", sizeof(char) * 8);
	int iconPosition = 2 + 3 * RANDER_WIDTH;
	for (int blockI = 0; blockI < 5; blockI++) {
		for (int i = 0; i < 4; i++) {
			ScreenPoint p = { BlockData[gameData->blockQueue[gameData->blockQueueIndex + blockI]][0][i].x * 2, BlockData[gameData->blockQueue[gameData->blockQueueIndex + blockI]][0][i].y };
			renderBuffer[ScreenPointToPosition(nextPosition) + iconPosition + ScreenPointToPosition(p)] = 161;
			renderBuffer[ScreenPointToPosition(nextPosition) + iconPosition + ScreenPointToPosition(p) + 1] = 189;
		}
		iconPosition += RANDER_WIDTH * 3;
	}
	// render score
	memcpy(renderBuffer + ScreenPointToPosition(scorePosition), "¢Ú¢×¢Ü¢Ó", sizeof(char) * 8);
	char scoreBuffer[10];
	itoa(gameData->score, scoreBuffer, 10);
	memcpy(renderBuffer + (ScreenPointToPosition(scorePosition) + RANDER_WIDTH * 2), scoreBuffer, sizeof(char) * strlen(scoreBuffer));

	strcpy(renderBuffer + RANDER_WIDTH * (RENDER_HEIGHT - 1), gameData->gameOver ? "Game Over" : "");
	// print to screen
	puts(renderBuffer);
}
