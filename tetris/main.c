#pragma warning(disable : 4996)
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <string.h>

#define FPS 60
#define RANDER_WIDTH 100
#define RENDER_HEIGHT 30
#define BOARD_WIDTH 10
#define BOARD_HEIGHT 20
#define DROP_TIME 1

typedef struct {
	int x;
	int y;
} ScreenPoint;

typedef struct {
	int x;
	int y;
} Point;

typedef struct {
	int type;
	Point position;
	int rotation;
	Point data[4];
} BlockInfo;

typedef struct {
	int size;
	Point *data;
} RotateConstraint;

const ScreenPoint boardPosition = { 12, 3 };
const ScreenPoint holdPosition = { 2, 3 };
const ScreenPoint nextPosition = { 34, 3 };
const ScreenPoint scorePosition = { 2, 20 };
const Point blockInitPosition = { 4, 0 };
const Point BlockData[7][4][4] = {
	{// 0
		// []()[][]
		{ { -1,  0 },{ 0,  0 },{ 1,  0 },{ 2,  0 } },
		//   []
		//   ()
		//   []
		//   []
		{ {  0, -1 },{ 0,  0 },{ 0,  1 },{ 0,  2 } },
		//   ()
		// [][][][]
		{ { -1,  1 },{ 0,  1 },{ 1,  1 },{ 2,  1 } },
		//     []
		//   ()[]
		//     []
		//     []
		{ {  1, -1 },{ 1,  0 },{ 1,  1 },{ 1,  2 } }
	},
	{// 1
		//   [][]
		//   ()[]
		{ {  0,  0 },{ 1,  0 },{ 0, -1 },{ 1, -1 } },
		{ {  0,  0 },{ 1,  0 },{ 0, -1 },{ 1, -1 } },
		{ {  0,  0 },{ 1,  0 },{ 0, -1 },{ 1, -1 } },
		{ {  0,  0 },{ 1,  0 },{ 0, -1 },{ 1, -1 } }
	},
	{// 2
		//   []
		// []()[]
		{ { -1,  0 },{ 0,  0 },{ 1,  0 },{ 0, -1 } },
		//   []
		//   ()[]
		//   []
		{ {  0,  1 },{ 0,  0 },{ 1,  0 },{ 0, -1 } },
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
		{ {  0,  0 },{ 1,  0 },{ -1, -1 },{ 0, -1 } },
		//   []
		// []()
		// []
		{ {  0, -1 },{ 0,  0 },{ -1,  0 },{ -1,  1 } },
		// []()
		//   [][]
		{ {  0,  1 },{ 1,  1 },{ -1,  0 },{ 0,  0 } },
		//     []
		//   ()[]
		//   []
		{ {  1, -1 },{ 1,  0 },{ 0,  0 },{ 0,  1 } }
	},
	{// 4
		// []
		// []()[]
		{ { -1,  0 },{ 0,  0 },{ 1,  0 },{ -1, -1 } },
		//   [][]
		//   ()
		//   []
		{ {  0, -1 },{ 0,  0 },{ 0,  1 },{ 1, -1 } },
		// []()[]
		//     []
		{ { -1,  0 },{ 0,  0 },{ 1,  0 },{ 1,  1 } },
		//   []
		//   ()
		// [][]
		{ {  0, -1 },{ 0,  0 },{ 0,  1 },{ -1,  1 } }
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
		{ {  0, -1 },{ 0,  0 },{ 1,  0 },{ 1,  1 } }
	},
	{// 6
		//     []
		// []()[]
		{ { -1,  0 },{ 0,  0 },{ 1,  0 },{ 1, -1 } },
		//   []
		//   ()
		//   [][]
		{ {  0, -1 },{ 0,  0 },{ 0,  1 },{ 1,  1 } },
		// []()[]
		// []
		{ { -1,  0 },{ 0,  0 },{ 1,  0 },{ -1,  1 } },
		// [][]
		//   ()
		//   []
		{ {  0, -1 },{ 0,  0 },{ 0,  1 },{ -1, -1 } }
	}
};
const Point rawRotateConstraintData[] = {
	{ 0, 0 },{ 0, 0 },
	{ 0, 0 },{ -1, 0 },{ -2, 0 },{ 1, 0 },{ 0, 0 },{ -1, 0 },{ -2, 0 },{ 1, 0 },
	{ 0, 0 },{ 0, 0 },
	{ 0, 0 },{ -1, 0 },{ 1, 0 },{ 2, 0 },{ 0, 0 },{ -1, 0 },{ 1, 0 },{ 2, 0 },

	{ 0, 0 },{ 0, 0 },
	{ 0, 0 },{ 0, 0 },
	{ 0, 0 },{ 0, 0 },
	{ 0, 0 },{ 0, 0 },

	{ 0, 0 },{ 1, 2 },{ 0, 0 },{ -1, 2 },
	{ 0, 0 },{ 1, 0 },{ 1, 1 },{ 0, 0 },{ 1, 0 },
	{ 0, 0 },{ 0, 0 },
	{ 0, 0 },{ -1, 0 },{ 0, 0 },{ -1, 0 },{ -1, 1 },

	{ 0, 0 },{ 0, 0 },
	{ 0, 0 },{ 0, 0 },
	{ 0, 0 },{ 0, 0 },
	{ 0, 0 },{ 0, 0 },

	{ 0, 0 },{ 0, 0 },
	{ 0, 0 },{ 0, 0 },
	{ 0, 0 },{ 0, 0 },
	{ 0, 0 },{ 0, 0 },

	{ 0, 0 },{ 0, 0 },
	{ 0, 0 },{ 0, 0 },
	{ 0, 0 },{ 0, 0 },
	{ 0, 0 },{ 0, 0 },

	{ 0, 0 },{ 0, 0 },
	{ 0, 0 },{ 0, 0 },
	{ 0, 0 },{ 0, 0 },
	{ 0, 0 },{ 0, 0 }
};
RotateConstraint rotateConstraint[7][4][2] = {
	{ { { 1 },{ 1 } },{ { 4 },{ 4 } },{ { 1 },{ 1 } },{ { 4 },{ 4 } } },
	{ { { 1 },{ 1 } },{ { 1 },{ 1 } },{ { 1 },{ 1 } },{ { 1 },{ 1 } } },
	{ { { 2 },{ 2 } },{ { 3 },{ 2 } },{ { 1 },{ 1 } },{ { 2 },{ 3 } } },
	{ { { 1 },{ 1 } },{ { 1 },{ 1 } },{ { 1 },{ 1 } },{ { 1 },{ 1 } } },
	{ { { 1 },{ 1 } },{ { 1 },{ 1 } },{ { 1 },{ 1 } },{ { 1 },{ 1 } } },
	{ { { 1 },{ 1 } },{ { 1 },{ 1 } },{ { 1 },{ 1 } },{ { 1 },{ 1 } } },
	{ { { 1 },{ 1 } },{ { 1 },{ 1 } },{ { 1 },{ 1 } },{ { 1 },{ 1 } } }
};
void InitRotateConstraint() {
	int dataPointer = 0;
	for (int i = 0; i < 7 * 4 * 2; i++) {
		rotateConstraint[0][0][i].data = rawRotateConstraintData + dataPointer;
		dataPointer += rotateConstraint[0][0][i].size;
	}
}

char* renderBuffer;
char board[BOARD_HEIGHT][BOARD_WIDTH];
int blockQueue[7 * 2];
int blockQueueIndex;
BlockInfo block;
int holdBlock;
int score = 0;
int gameOver = 0;

inline int ScreenPointToPosition(ScreenPoint point) {
	return point.x + point.y * RANDER_WIDTH;
}

void render() {
	//system("cls");
	// clear renderBuffer
	memset(renderBuffer, 32, RANDER_WIDTH * RENDER_HEIGHT);
	for (int i = 0; i < RENDER_HEIGHT; i++) {
		renderBuffer[RANDER_WIDTH * i + RANDER_WIDTH - 1] = '\n';
	}
	// render board
	for (int x = 0; x < BOARD_WIDTH; x++) {
		for (int y = 0; y < BOARD_HEIGHT; y++) {
			ScreenPoint p = { x * 2, y };
			if (board[y][x] == 0) {
				renderBuffer[ScreenPointToPosition(boardPosition) + ScreenPointToPosition(p)] = 161;
				renderBuffer[ScreenPointToPosition(boardPosition) + ScreenPointToPosition(p) + 1] = 188;
			}
			else {
				renderBuffer[ScreenPointToPosition(boardPosition) + ScreenPointToPosition(p)] = 161;
				renderBuffer[ScreenPointToPosition(boardPosition) + ScreenPointToPosition(p) + 1] = 189;
			}
		}
	}
	Point shadowPosition[4];
	memcpy(shadowPosition, block.data, sizeof(shadowPosition));
	while (PositionCheck(block.data, shadowPosition)) {
		for (int i = 0; i < 4; i++) {
			shadowPosition[i].y++;
		}
	}
	for (int i = 0; i < 4; i++) {
		shadowPosition[i].y--;
		if (board[shadowPosition[i].y][shadowPosition[i].x] == 0) {
			ScreenPoint p = { shadowPosition[i].x * 2, shadowPosition[i].y };
			renderBuffer[ScreenPointToPosition(boardPosition) + ScreenPointToPosition(p)] = 162;
			renderBuffer[ScreenPointToPosition(boardPosition) + ScreenPointToPosition(p) + 1] = 174;
		}
	}
	// render hold
	memcpy(renderBuffer + ScreenPointToPosition(holdPosition), "¢Ö¢Ý¢Ú¢Ò", sizeof(char) * 8);
	if (holdBlock != -1) {
		int iconPosition = ScreenPointToPosition((ScreenPoint) { 2, 3 });
		for (int i = 0; i < 4; i++) {
			ScreenPoint p = { BlockData[holdBlock][0][i].x * 2, BlockData[holdBlock][0][i].y };
			renderBuffer[ScreenPointToPosition(holdPosition) + iconPosition + ScreenPointToPosition(p)] = -95;
			renderBuffer[ScreenPointToPosition(holdPosition) + iconPosition + ScreenPointToPosition(p) + 1] = -67;
		}
	}
	// render next
	memcpy(renderBuffer + ScreenPointToPosition(nextPosition), "¢Ü¢Ó¢æ¢â", sizeof(char) * 8);
	int iconPosition = ScreenPointToPosition((ScreenPoint) { 2, 3 });
	for (int blockI = 0; blockI < 5; blockI++) {
		for (int i = 0; i < 4; i++) {
			ScreenPoint p = { BlockData[blockQueue[blockQueueIndex + blockI]][0][i].x * 2, BlockData[blockQueue[blockQueueIndex + blockI]][0][i].y };
			renderBuffer[ScreenPointToPosition(nextPosition) + iconPosition + ScreenPointToPosition(p)] = -95;
			renderBuffer[ScreenPointToPosition(nextPosition) + iconPosition + ScreenPointToPosition(p) + 1] = -67;
		}
		iconPosition += RANDER_WIDTH * 3;
	}
	// render score
	memcpy(renderBuffer + ScreenPointToPosition(scorePosition), "¢Ú¢×¢Ü¢Ó", sizeof(char) * 8);
	char scoreBuffer[10];
	itoa(score, scoreBuffer, 10);
	memcpy(renderBuffer + (ScreenPointToPosition(scorePosition) + RANDER_WIDTH * 2), scoreBuffer, sizeof(char) * strlen(scoreBuffer));

	strcpy(renderBuffer + RANDER_WIDTH * (RENDER_HEIGHT - 1), gameOver ? "Game Over" : "");
	// print to screen
	puts(renderBuffer);
}

void UpdateBlockData(BlockInfo* block) {
	for (int i = 0; i < 4; i++) {
		if (block->data[i].y >= 0) {
			board[block->data[i].y][block->data[i].x] = 0;
		}
	}
	memcpy(block->data, BlockData[block->type][block->rotation], sizeof(block->data));
	for (int i = 0; i < 4; i++) {
		block->data[i].x += block->position.x;
		block->data[i].y += block->position.y;
	}
	//BlockData(block);
	for (int i = 0; i < 4; i++) {
		if (block->data[i].y >= 0) {
			board[block->data[i].y][block->data[i].x] = 1;
		}
	}
}

inline int IsInBound(Point point) {
	return point.x >= 0 && point.x < BOARD_WIDTH && point.y < BOARD_HEIGHT;
}

int PositionCheck(Point *oldPosition, Point *newPosition) {
	for (int i = 0; i < 4; i++) {
		if (!IsInBound(newPosition[i])) {
			return 0;
		}
		else if (newPosition[i].y < 0 || board[newPosition[i].y][newPosition[i].x] == 0) {
			continue;
		}
		else {
			int overlapSelf = 0;
			for (int otherData = 0; otherData < 4; otherData++) {
				if (newPosition[i].x == oldPosition[otherData].x && newPosition[i].y == oldPosition[otherData].y) {
					overlapSelf = 1;
					break;
				}
			}
			if (!overlapSelf) {
				return 0;
			}
		}
	}
	return 1;
}

// 0°f®É°w 1¶¶®É°w
void RotateBlock(BlockInfo* block, int direction) {
	int rotation = (block->rotation + (direction ? 1 : 3)) % 4;
	for (int i = 0; i < rotateConstraint[block->type][block->rotation][direction].size; i++) {
		Point newPosition[4];
		for (int posi = 0; posi < 4; posi++) {
			newPosition[posi].x = BlockData[block->type][rotation][posi].x + block->position.x + rotateConstraint[block->type][block->rotation][direction].data[i].x;
			newPosition[posi].y = BlockData[block->type][rotation][posi].y + block->position.y + rotateConstraint[block->type][block->rotation][direction].data[i].y;
		}
		if (PositionCheck(block->data, newPosition)) {
			block->position.x += rotateConstraint[block->type][block->rotation][direction].data[i].x;
			block->position.y += rotateConstraint[block->type][block->rotation][direction].data[i].y;
			block->rotation = rotation;
			UpdateBlockData(block);
			break;
		}
	}
}

int MoveCheck(BlockInfo* block, Point vector) {
	for (int i = 0; i < 4; i++) {
		Point newPosition = { block->data[i].x + vector.x , block->data[i].y + vector.y };
		if (!IsInBound(newPosition)) {
			return 0;
		}
		else if (newPosition.y < 0 || board[newPosition.y][newPosition.x] == 0) {
			continue;
		}
		else {
			int overlapSelf = 0;
			for (int otherData = 0; otherData < 4; otherData++) {
				if (newPosition.x == block->data[otherData].x && newPosition.y == block->data[otherData].y) {
					overlapSelf = 1;
					break;
				}
			}
			if (!overlapSelf) {
				return 0;
			}
		}
	}
	return 1;
}

int DropBlock(BlockInfo* block) {
	Point direction = { 0, 1 };
	if (MoveCheck(block, direction)) {
		block->position.y++;
		UpdateBlockData(block);
		return 0;
	}
	else {
		return 1;
	}
}

BlockInfo CreateBlock(int type) {
	BlockInfo newBlock = {
		.type = type,
		.position = blockInitPosition,
		.rotation = 0,
	};
	memcpy(newBlock.data, BlockData[newBlock.type][newBlock.rotation], sizeof(newBlock.data));
	for (int i = 0; i < 4; i++) {
		newBlock.data[i].x += newBlock.position.x;
		newBlock.data[i].y += newBlock.position.y;
	}
	//BlockData(&newBlock);
	for (int i = 0; i < 4; i++) {
		if (board[newBlock.data[i].y][newBlock.data[i].x] != 0) {
			gameOver = 1;
		}
	}
	UpdateBlockData(&newBlock);
	return newBlock;
}

BlockInfo NextBlock() {
	static const char fillRow[10] = { 1, 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 };
	if (memcmp(board[0], fillRow, sizeof(char) * BOARD_WIDTH) == 0) {
		memset(board[0], 0, sizeof(char) * BOARD_WIDTH);
	}
	for (int i = 1; i < BOARD_HEIGHT; i++) {
		if (memcmp(board[i], fillRow, sizeof(char) * BOARD_WIDTH) == 0) {
			for (int c = i; c > 0; c--) {
				memcpy(board[c], board[c - 1], sizeof(char) * BOARD_WIDTH);
			}
			score++;
		}
	}
	return CreateBlock(PopBlockQueue());
}

void FallBlock(BlockInfo* block) {
	while (!DropBlock(block));
	BlockInfo nextBlock = NextBlock();
	memcpy(block, &nextBlock, sizeof(BlockInfo));
}

int PopBlockQueue() {
	int nextBlock = blockQueue[blockQueueIndex++];
	if (blockQueueIndex == 7) {
		memcpy(blockQueue, blockQueue + 7, sizeof(int) * 7);
		// shuffle blockQueue last 7 elements
		for (int i = 0; i < 7; i++) {
			int  j = i + rand() / (RAND_MAX / (7 - i) + 1);
			int temp = blockQueue[j];
			blockQueue[j] = blockQueue[i];
			blockQueue[i] = temp;
		}
		blockQueueIndex = 0;
	}
	return nextBlock;
}

BlockInfo HoldBlock(BlockInfo* block) {
	for (int i = 0; i < 4; i++) {
		if (block->data[i].y >= 0) {
			board[block->data[i].y][block->data[i].x] = 0;
		}
	}
	if (holdBlock == -1) {
		holdBlock = block->type;
		return NextBlock();
	}
	else {
		BlockInfo newBlock = CreateBlock(holdBlock);
		holdBlock = block->type;
		return newBlock;
	}
}

void ResetData() {
	// clear board
	memset(board, 0, BOARD_WIDTH * BOARD_HEIGHT);
	// initialize blockQueue
	for (int i = 0; i < 7; i++) {
		blockQueue[i] = blockQueue[i + 7] = i;
	}
	for (int i = 0; i < 7; i++) {
		int  j = i + rand() / (RAND_MAX / (7 - i) + 1);
		int temp = blockQueue[j];
		blockQueue[j] = blockQueue[i];
		blockQueue[i] = temp;
		j = i + rand() / (RAND_MAX / (7 - i) + 1);
		temp = blockQueue[j + 7];
		blockQueue[j + 7] = blockQueue[i + 7];
		blockQueue[i + 7] = temp;
	}
	blockQueueIndex = 0;
	// initialize current block
	block = NextBlock();
	holdBlock = -1;
	score = 0;
	gameOver = 0;
}

int main(int argc, char* argv[]) {
	renderBuffer = malloc(sizeof(char) * RANDER_WIDTH * RENDER_HEIGHT);
	InitRotateConstraint();
	ResetData();
	double lastDropTime = ((double)clock()) / CLOCKS_PER_SEC;
	double accumulationTime = ((double)clock()) / CLOCKS_PER_SEC;
	int dirty = 1;
	while (1) {
		double currentTime = ((double)clock()) / CLOCKS_PER_SEC;
		double biasTime = (1.0 / FPS) - (currentTime - accumulationTime);
		while (kbhit()) {
			unsigned char key = getch();
			if (key == 27) {
				ResetData();
			}
			else if (!gameOver) {
				switch (key) {
				case 'z':
					RotateBlock(&block, 0);
					break;
				case 'x':
					RotateBlock(&block, 1);
					break;
				case 'c':
					block = HoldBlock(&block);
					break;
				case ' ':
					FallBlock(&block);
					break;
				case 224:
					key = getch();
					if (key == 72) {
						RotateBlock(&block, 1);
					}
					else if (key == 80) {
						if (!DropBlock(&block)) {
							lastDropTime = currentTime;
						}
					}
					else if (key == 75) {
						Point direction = { -1, 0 };
						if (MoveCheck(&block, direction)) {
							block.position.x--;
							UpdateBlockData(&block);
						}
					}
					else if (key == 77) {
						Point direction = { 1, 0 };
						if (MoveCheck(&block, direction)) {
							block.position.x++;
							UpdateBlockData(&block);
						}
					}
					break;
				default:
					break;
				}
			}
			dirty = 1;
		}

		if (!gameOver && (currentTime - lastDropTime > DROP_TIME)) {
			if (DropBlock(&block)) {
				block = NextBlock();
			}
			lastDropTime = currentTime;
			dirty = 1;
		}
		if (dirty) {
			render();
			dirty = 0;
		}
		accumulationTime = currentTime + biasTime;
		double sleepTime = (1.0 / FPS) + biasTime;
		sleepTime = sleepTime > 0 ? sleepTime : 0;
		_sleep(sleepTime * 1000);
	}
	free(renderBuffer);
	system("pause");
	return 0;
}
