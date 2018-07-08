#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./tetris.h"
#include "./tetrisNetwork.h"

const Point UP = { 0, -1 };
const Point DOWN = { 0, 1 };
const Point LEFT = { -1, 0 };
const Point RIGHT = { 1, 0 };
const Point BlockInitPosition = { 4, 0 };
const Point BlockData[7][4][4] = {
	{// 0
	 // []()[][]
		{ { -1,  0 },{ 0,  0 },{ 1,  0 },{ 2,  0 } },
		//   []
		//   ()
		//   []
		//   []
		{ { 0, -1 },{ 0,  0 },{ 0,  1 },{ 0,  2 } },
		//   ()
		// [][][][]
		{ { -1,  1 },{ 0,  1 },{ 1,  1 },{ 2,  1 } },
		//     []
		//   ()[]
		//     []
		//     []
		{ { 1, -1 },{ 1,  0 },{ 1,  1 },{ 1,  2 } }
	},
	{// 1
	 //   [][]
	 //   ()[]
		{ { 0,  0 },{ 1,  0 },{ 0, -1 },{ 1, -1 } },
		{ { 0,  0 },{ 1,  0 },{ 0, -1 },{ 1, -1 } },
		{ { 0,  0 },{ 1,  0 },{ 0, -1 },{ 1, -1 } },
		{ { 0,  0 },{ 1,  0 },{ 0, -1 },{ 1, -1 } }
	},
	{// 2
	 //   []
	 // []()[]
		{ { -1,  0 },{ 0,  0 },{ 1,  0 },{ 0, -1 } },
		//   []
		//   ()[]
		//   []
		{ { 0,  1 },{ 0,  0 },{ 1,  0 },{ 0, -1 } },
		// []()[]
		//   []
		{ { -1,  0 },{ 0,  0 },{ 1,  0 },{ 0,  1 } },
		//   []
		// []()
		//   []
		{ { -1,  0 },{ 0,  0 },{ 0,  1 },{ 0, -1 } }
	},
	{// 3
	 // [][]
	 //   ()[]
		{ { 0,  0 },{ 1,  0 },{ -1, -1 },{ 0, -1 } },
		//   []
		// []()
		// []
		{ { 0, -1 },{ 0,  0 },{ -1,  0 },{ -1,  1 } },
		// []()
		//   [][]
		{ { 0,  1 },{ 1,  1 },{ -1,  0 },{ 0,  0 } },
		//     []
		//   ()[]
		//   []
		{ { 1, -1 },{ 1,  0 },{ 0,  0 },{ 0,  1 } }
	},
	{// 4
	 // []
	 // []()[]
		{ { -1,  0 },{ 0,  0 },{ 1,  0 },{ -1, -1 } },
		//   [][]
		//   ()
		//   []
		{ { 0, -1 },{ 0,  0 },{ 0,  1 },{ 1, -1 } },
		// []()[]
		//     []
		{ { -1,  0 },{ 0,  0 },{ 1,  0 },{ 1,  1 } },
		//   []
		//   ()
		// [][]
		{ { 0, -1 },{ 0,  0 },{ 0,  1 },{ -1,  1 } }
	},
	{// 5
	 //   [][]
	 // []()
		{ { -1,  0 },{ 0,  0 },{ 0, -1 },{ 1, -1 } },
		// []
		// []()
		//   []
		{ { -1, -1 },{ -1,  0 },{ 0,  0 },{ 0,  1 } },
		//   ()[]
		// [][]
		{ { -1,  1 },{ 0,  1 },{ 0,  0 },{ 1,  0 } },
		//   []
		//   ()[]
		//     []
		{ { 0, -1 },{ 0,  0 },{ 1,  0 },{ 1,  1 } }
	},
	{// 6
	 //     []
	 // []()[]
		{ { -1,  0 },{ 0,  0 },{ 1,  0 },{ 1, -1 } },
		//   []
		//   ()
		//   [][]
		{ { 0, -1 },{ 0,  0 },{ 0,  1 },{ 1,  1 } },
		// []()[]
		// []
		{ { -1,  0 },{ 0,  0 },{ 1,  0 },{ -1,  1 } },
		// [][]
		//   ()
		//   []
		{ { 0, -1 },{ 0,  0 },{ 0,  1 },{ -1, -1 } }
	}
};
Point rawRotateConstraintData[] = {
	{ 0, 0 },/**/{ 0, 0 },
	{ 0, 0 },{ -1, 0 },{ -2, 0 },{ 1, 0 },/**/{ 0, 0 },{ -1, 0 },{ -2, 0 },{ 1, 0 },
	{ 0, 0 },/**/{ 0, 0 },
	{ 0, 0 },{ -1, 0 },{ 1, 0 },{ 2, 0 },/**/{ 0, 0 },{ -1, 0 },{ 1, 0 },{ 2, 0 },

	{ 0, 0 },/**/{ 0, 0 },
	{ 0, 0 },/**/{ 0, 0 },
	{ 0, 0 },/**/{ 0, 0 },
	{ 0, 0 },/**/{ 0, 0 },

	{ 0, 0 },{ 1, 2 },/**/{ 0, 0 },{ -1, 2 },
	{ 0, 0 },{ 1, 0 },{ 1, 1 },/**/{ 0, 0 },{ 1, 0 },{ 0, 1 },
	{ 0, 0 },/**/{ 0, 0 },
	{ 0, 0 },{ -1, 0 },/**/{ 0, 0 },{ -1, 0 },{ -1, 1 },

	{ 0, 0 },{ 0, -1 },/**/{ 0, 0 },{ 0, -1 },
	{ 0, 0 },{ -1, 0 },/**/{ 0, 0 },{ -1, 0 },{ 0, -1 },
	{ 0, 0 },/**/{ 0, 0 },
	{ 0, 0 },{ 1, 0 },/**/{ 0, 0 },{ 1, 0 },

	{ 0, 0 },{ 0, -1 },/**/{ 0, 0 },{ 0, -1 },
	{ 0, 0 },{ 1, 0 },{ 1, 1 },/**/{ 0, 0 },{ 1, 0 },
	{ 0, 0 },{ 0, -1 },/**/{ 0, 0 },{ 0, -1 },
	{ 0, 0 },{ -1, 0 },/**/{ 0, 0 },{ -1, 0 },

	{ 0, 0 },{ 0, -1 },/**/{ 0, 0 },{ 0, -1 },
	{ 0, 0 },{ -1, 0 },{ 0, 1 },/**/{ 0, 0 },{ -1, 0 },
	{ 0, 0 },/**/{ 0, 0 },
	{ 0, 0 },{ 1, 0 },{ 0, -1 },/**/{ 0, 0 },{ 1, 0 },

	{ 0, 0 },{ 0, -1 },/**/{ 0, 0 },{ 0, -1 },
	{ 0, 0 },{ 1, 0 },/**/{ 0, 0 },{ 1, 0 },
	{ 0, 0 },{ 0, -1 },/**/{ 0, 0 },{ 0, -1 },
	{ 0, 0 },{ -1, 0 },/**/{ 0, 0 },{ -1, 0 },{ -1, 1 }
};
RotateConstraint rotateConstraint[7][4][2] = {
	{ { { 1 },{ 1 } },{ { 4 },{ 4 } },{ { 1 },{ 1 } },{ { 4 },{ 4 } } },
	{ { { 1 },{ 1 } },{ { 1 },{ 1 } },{ { 1 },{ 1 } },{ { 1 },{ 1 } } },
	{ { { 2 },{ 2 } },{ { 3 },{ 3 } },{ { 1 },{ 1 } },{ { 2 },{ 3 } } },
	{ { { 2 },{ 2 } },{ { 2 },{ 2 } },{ { 1 },{ 1 } },{ { 3 },{ 2 } } },
	{ { { 2 },{ 2 } },{ { 3 },{ 2 } },{ { 2 },{ 2 } },{ { 2 },{ 2 } } },
	{ { { 2 },{ 2 } },{ { 2 },{ 3 } },{ { 1 },{ 1 } },{ { 3 },{ 2 } } },
	{ { { 2 },{ 2 } },{ { 2 },{ 2 } },{ { 2 },{ 2 } },{ { 2 },{ 3 } } }
};

GameData* currentGame = NULL;

// 偵測碰撞用的function
int IsInBound(Point position);
int IsEmpty(Point position);
int IsInclude(Point position, Point data[4]);
// 更新board用的function
void RemoveFromBoard(Block* block);
void AddToBoard(Block* block);
void UpdateBlockData(Block* block);

int IsInBound(Point position) {
	return position.x >= 0 && position.x < BOARD_WIDTH && position.y < BOARD_HEIGHT;
}

int IsEmpty(Point position) {
	return position.y < 0 || currentGame->board[position.y][position.x] == 0;
}

int IsInclude(Point position, Point data[4]) {
	for (int i = 0; i < 4; i++) {
		if (position.x == data[i].x && position.y == data[i].y) {
			return 1;
		}
	}
	return 0;
}

int PositionCheck(Point *newPosition, Point *oldPosition) {
	for (int i = 0; i < 4; i++) {
		if (!IsInBound(newPosition[i]) || !IsEmpty(newPosition[i]) && oldPosition != NULL && !IsInclude(newPosition[i], oldPosition)) {
			return 0;
		}
	}
	return 1;
}

void RemoveFromBoard(Block* block) {
	for (int i = 0; i < 4; i++) {
		if (block->data[i].y >= 0) {
			currentGame->board[block->data[i].y][block->data[i].x] = 0;
		}
	}
}

void AddToBoard(Block* block) {
	for (int i = 0; i < 4; i++) {
		if (block->data[i].y >= 0) {
			currentGame->board[block->data[i].y][block->data[i].x] = 1;
		}
	}
}

void UpdateBlockData(Block* block) {
	RemoveFromBoard(block);
	for (int i = 0; i < 4; i++) {
		block->data[i] = PointAdd(BlockData[block->type][block->rotation][i], block->position);
	}
	AddToBoard(block);
}

int TriggerBomb(Block* block) {
	for (int i = 0; i < 4; i++) {
		if ((block->data[i].y + 1) >= 0 && (block->data[i].y + 1) < BOARD_HEIGHT && currentGame->board[block->data[i].y + 1][block->data[i].x] == 2) {
			currentGame->board[block->data[i].y + 1][block->data[i].x] = 1;
			for (int j = block->data[i].y + 2; j < BOARD_HEIGHT; j++) {
				if (currentGame->board[j][block->data[i].x] == 2) {
					currentGame->board[j][block->data[i].x] = 1;
				}
			}
			return 1;
		}
	}
	return 0;
}

// 0逆時針 1順時針
void RotateBlock(Block* block, int direction) {
	int rotation = (block->rotation + (direction ? 1 : 3)) % 4;
	for (int i = 0; i < rotateConstraint[block->type][block->rotation][direction].size; i++) {
		Point newPosition[4];
		for (int posi = 0; posi < 4; posi++) {
			newPosition[posi].x = BlockData[block->type][rotation][posi].x + block->position.x + rotateConstraint[block->type][block->rotation][direction].data[i].x;
			newPosition[posi].y = BlockData[block->type][rotation][posi].y + block->position.y + rotateConstraint[block->type][block->rotation][direction].data[i].y;
		}
		if (PositionCheck(newPosition, block->data)) {
			block->position.x += rotateConstraint[block->type][block->rotation][direction].data[i].x;
			block->position.y += rotateConstraint[block->type][block->rotation][direction].data[i].y;
			block->rotation = rotation;
			UpdateBlockData(block);
			break;
		}
	}
}

int MoveBlock(Block* block, Point direction) {
	Point newData[4];
	for (int i = 0; i < 4; i++) {
		newData[i] = PointAdd(block->data[i], direction);
	}
	if (!PositionCheck(newData, block->data)) {
		return 0;
	}
	block->position = PointAdd(block->position, direction);
	UpdateBlockData(block);
	return 1;
}

int PopBlockQueue() {
	int nextBlock = currentGame->blockQueue[currentGame->blockQueueIndex++];
	if (currentGame->blockQueueIndex == 7) {
		memcpy(currentGame->blockQueue, currentGame->blockQueue + 7, sizeof(int) * 7);
		// shuffle blockQueue last 7 elements
		for (int i = 0; i < 7; i++) {
			int  j = i + rand() / (RAND_MAX / (7 - i) + 1);
			int temp = currentGame->blockQueue[j + 7];
			currentGame->blockQueue[j + 7] = currentGame->blockQueue[i + 7];
			currentGame->blockQueue[i + 7] = temp;
		}
		currentGame->blockQueueIndex = 0;
	}
	return nextBlock;
}

Block CreateBlock(int type) {
	Block newBlock = {
		.type = type,
		.position = BlockInitPosition,
		.rotation = 0,
	};
	memcpy(newBlock.data, BlockData[newBlock.type][newBlock.rotation], sizeof(newBlock.data));
	for (int i = 0; i < 4; i++) {
		if (!IsEmpty(newBlock.data[i] = PointAdd(BlockData[newBlock.type][newBlock.rotation][i], newBlock.position))) {
			currentGame->gameOver = 1;
		}
	}
	UpdateBlockData(&newBlock);
	return newBlock;
}

Block NextBlock() {
	char fillRow[BOARD_WIDTH], hasClean = 0;
	memset(fillRow, 1, sizeof(char) * BOARD_WIDTH);
	if (memcmp(currentGame->board[0], fillRow, sizeof(char) * BOARD_WIDTH) == 0) {
		memset(currentGame->board[0], 0, sizeof(char) * BOARD_WIDTH);
	}
	for (int i = 1; i < BOARD_HEIGHT; i++) {
		if (memcmp(currentGame->board[i], fillRow, sizeof(char) * BOARD_WIDTH) == 0) {
			for (int c = i; c > 0; c--) {
				memcpy(currentGame->board[c], currentGame->board[c - 1], sizeof(char) * BOARD_WIDTH);
			}
			currentGame->score++;
			currentGame->combo++;
			hasClean = 1;
			putchar('\a');
		}
	}
	if (!hasClean) {
		if (currentGame->combo > 1) {
			SendBomb(currentGame->combo - 1);
		}
		currentGame->combo = 0;
	}
	while (currentGame->bomb > 0) {
		memcpy(currentGame->board[0], currentGame->board[1], sizeof(char) * BOARD_WIDTH * (BOARD_HEIGHT - 1));
		memcpy(currentGame->board[BOARD_HEIGHT - 1], fillRow, sizeof(char) * BOARD_WIDTH);
		currentGame->board[BOARD_HEIGHT - 1][rand() / (RAND_MAX / BOARD_WIDTH)] = 2;
		currentGame->bomb--;
	}
	return CreateBlock(PopBlockQueue());
}

void DropBlock(Block* block) {
	if (!MoveBlock(block, DOWN)) {
		TriggerBomb(block);
		Block nextBlock = NextBlock();
		memcpy(block, &nextBlock, sizeof(Block));
	}
}

void FallBlock(Block* block) {
	while (MoveBlock(block, DOWN));
	DropBlock(block);
}

void HoldBlock(Block* block) {
	Block newBlock;
	RemoveFromBoard(block);
	newBlock = currentGame->holdBlock == -1 ? NextBlock() : CreateBlock(currentGame->holdBlock);
	currentGame->holdBlock = block->type;
	memcpy(block, &newBlock, sizeof(Block));
}

void ResetData(GameData* gameData) {
	currentGame = gameData;
	// clear board
	memset(currentGame->board, 0, BOARD_WIDTH * BOARD_HEIGHT);
	// initialize blockQueue
	for (int i = 0; i < 7; i++) {
		currentGame->blockQueue[i] = currentGame->blockQueue[i + 7] = i;
	}
	currentGame->blockQueueIndex = 6;
	PopBlockQueue();
	currentGame->blockQueueIndex = 6;
	PopBlockQueue();
	currentGame->holdBlock = -1;
	currentGame->bomb = 0;
	currentGame->score = 0;
	currentGame->combo = 0;
	currentGame->gameOver = 0;
	currentGame->block = NextBlock();
}

void InitTetris() {
	int dataPointer = 0;
	for (int i = 0; i < 7 * 4 * 2; i++) {
		rotateConstraint[0][0][i].data = rawRotateConstraintData + dataPointer;
		dataPointer += rotateConstraint[0][0][i].size;
	}
}
