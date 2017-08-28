#pragma warning(disable : 4996)
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <string.h>

#define FPS 30
#define RANDER_WIDTH 120
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
} Block;

typedef struct {
	int size;
	Point *data;
} RotateConstraint;

typedef struct {
	Block block;
	Point blockInitPosition;
	double lastDropTime;
} PlayerData;

typedef struct {
	char board[BOARD_HEIGHT][BOARD_WIDTH];
	int blockQueue[7 * 2];
	int blockQueueIndex;
	int holdBlock;
	int score;
	int gameOver;
	PlayerData* players;
	int playerCount;
} GameData;

const ScreenPoint boardPosition = { 12, 3 };
const ScreenPoint holdPosition = { 2, 3 };
const ScreenPoint nextPosition = { 94, 3 };
const ScreenPoint scorePosition = { 2, 20 };
const Point UP = { 0, -1 };
const Point DOWN = { 0, 1 };
const Point LEFT = { -1, 0 };
const Point RIGHT = { 1, 0 };
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
const Point blockInitializePositions[] = { {4, 0},{ 14, 0 },{ 24, 0 },{ 34, 0 } };

char renderBuffer[RANDER_WIDTH * RENDER_HEIGHT];
GameData game;

//const Point blockInitPosition = { 4, 0 };
//Block block;
//double lastDropTime;
//
//Block block2;
//const Point blockInitPosition2 = { 34, 0 };
//double lastDropTime2;

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
			if (game.board[y][x] == 0) {
				renderBuffer[ScreenPointToPosition(boardPosition) + ScreenPointToPosition(p)] = 161;
				renderBuffer[ScreenPointToPosition(boardPosition) + ScreenPointToPosition(p) + 1] = 188;
			}
			else {
				renderBuffer[ScreenPointToPosition(boardPosition) + ScreenPointToPosition(p)] = 161;
				renderBuffer[ScreenPointToPosition(boardPosition) + ScreenPointToPosition(p) + 1] = 189;
			}
		}
	}
	for (int i = 0; i < game.playerCount; i++) {
		Point shadowPosition[4];
		memcpy(shadowPosition, game.players[i].block.data, sizeof(shadowPosition));
		while (PositionCheck(shadowPosition, game.players[i].block.data)) {
			for (int i = 0; i < 4; i++) {
				shadowPosition[i].y++;
			}
		}
		for (int i = 0; i < 4; i++) {
			shadowPosition[i].y--;
			if (shadowPosition[i].y >= 0 && game.board[shadowPosition[i].y][shadowPosition[i].x] == 0) {
				ScreenPoint p = { shadowPosition[i].x * 2, shadowPosition[i].y };
				renderBuffer[ScreenPointToPosition(boardPosition) + ScreenPointToPosition(p)] = 162;
				renderBuffer[ScreenPointToPosition(boardPosition) + ScreenPointToPosition(p) + 1] = 174;
			}
		}
	}
	// render hold
	memcpy(renderBuffer + ScreenPointToPosition(holdPosition), "¢Ö¢Ý¢Ú¢Ò", sizeof(char) * 8);
	if (game.holdBlock != -1) {
		int iconPosition = ScreenPointToPosition((ScreenPoint) { 2, 3 });
		for (int i = 0; i < 4; i++) {
			ScreenPoint p = { BlockData[game.holdBlock][0][i].x * 2, BlockData[game.holdBlock][0][i].y };
			renderBuffer[ScreenPointToPosition(holdPosition) + iconPosition + ScreenPointToPosition(p)] = -95;
			renderBuffer[ScreenPointToPosition(holdPosition) + iconPosition + ScreenPointToPosition(p) + 1] = -67;
		}
	}
	// render next
	memcpy(renderBuffer + ScreenPointToPosition(nextPosition), "¢Ü¢Ó¢æ¢â", sizeof(char) * 8);
	int iconPosition = ScreenPointToPosition((ScreenPoint) { 2, 3 });
	for (int blockI = 0; blockI < 5; blockI++) {
		for (int i = 0; i < 4; i++) {
			ScreenPoint p = { BlockData[game.blockQueue[game.blockQueueIndex + blockI]][0][i].x * 2, BlockData[game.blockQueue[game.blockQueueIndex + blockI]][0][i].y };
			renderBuffer[ScreenPointToPosition(nextPosition) + iconPosition + ScreenPointToPosition(p)] = -95;
			renderBuffer[ScreenPointToPosition(nextPosition) + iconPosition + ScreenPointToPosition(p) + 1] = -67;
		}
		iconPosition += RANDER_WIDTH * 3;
	}
	// render score
	memcpy(renderBuffer + ScreenPointToPosition(scorePosition), "¢Ú¢×¢Ü¢Ó", sizeof(char) * 8);
	char scoreBuffer[10];
	itoa(game.score, scoreBuffer, 10);
	memcpy(renderBuffer + (ScreenPointToPosition(scorePosition) + RANDER_WIDTH * 2), scoreBuffer, sizeof(char) * strlen(scoreBuffer));

	strcpy(renderBuffer + RANDER_WIDTH * (RENDER_HEIGHT - 1), game.gameOver ? "Game Over" : "");
	// print to screen
	puts(renderBuffer);
}

Point PointAdd(Point a, Point b) {
	Point r = { a.x + b.x, a.y + b.y };
	return r;
}

void RemoveFromBoard(Block* block) {
	for (int i = 0; i < 4; i++) {
		if (block->data[i].y >= 0) {
			game.board[block->data[i].y][block->data[i].x] = 0;
		}
	}
}

void AddToBoard(Block* block) {
	for (int i = 0; i < 4; i++) {
		if (block->data[i].y >= 0) {
			game.board[block->data[i].y][block->data[i].x] = 1;
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

inline int IsInBound(Point position) {
	return position.x >= 0 && position.x < BOARD_WIDTH && position.y < BOARD_HEIGHT;
}

inline int IsEmpty(Point position) {
	return position.y < 0 || game.board[position.y][position.x] == 0;
}

inline int IsInclude(Point position, Point data[4]) {
	for (int i = 0; i < 4; i++) {
		if (position.x == data[i].x && position.y == data[i].y) {
			return 1;
		}
	}
	return 0;
}

inline int IsOverlap(Point position) {
	for (int i = 0; i < game.playerCount; i++) {
		if (IsOverlap(position, game.players[i].block.data)) {
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

// 0°f®É°w 1¶¶®É°w
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

Block CreateBlock(int type, Point position) {
	Block newBlock = {
		.type = type,
		.position = position,
		.rotation = 0,
	};
	memcpy(newBlock.data, BlockData[newBlock.type][newBlock.rotation], sizeof(newBlock.data));
	for (int i = 0; i < 4; i++) {
		if (!IsEmpty(newBlock.data[i] = PointAdd(BlockData[newBlock.type][newBlock.rotation][i], newBlock.position))) {
			game.gameOver = 1;
		}
	}
	UpdateBlockData(&newBlock);
	return newBlock;
}

Block NextBlock(int playerIndex) {
	char fillRow[BOARD_WIDTH];
	memset(fillRow, 1, sizeof(char) * BOARD_WIDTH);
	if (memcmp(game.board[0], fillRow, sizeof(char) * BOARD_WIDTH) == 0) {
		memset(game.board[0], 0, sizeof(char) * BOARD_WIDTH);
	}
	for (int i = 1; i < BOARD_HEIGHT; i++) {
		if (memcmp(game.board[i], fillRow, sizeof(char) * BOARD_WIDTH) == 0) {
			for (int c = i; c > 0; c--) {
				memcpy(game.board[c], game.board[c - 1], sizeof(char) * BOARD_WIDTH);
			}
			for (int i = 0; i < game.playerCount; i++) {
				for (int j = 0; j < 4; j++) {
					game.players[i].block.data[j].y++;
				}
			}
			game.score++;
		}
	}
	return CreateBlock(PopBlockQueue(), game.players[playerIndex].blockInitPosition);
}

int PopBlockQueue() {
	int nextBlock = game.blockQueue[game.blockQueueIndex++];
	if (game.blockQueueIndex == 7) {
		memcpy(game.blockQueue, game.blockQueue + 7, sizeof(int) * 7);
		// shuffle blockQueue last 7 elements
		for (int i = 0; i < 7; i++) {
			int  j = i + rand() / (RAND_MAX / (7 - i) + 1);
			int temp = game.blockQueue[j];
			game.blockQueue[j] = game.blockQueue[i];
			game.blockQueue[i] = temp;
		}
		game.blockQueueIndex = 0;
	}
	return nextBlock;
}

void FallBlock(int playerIndex) {
	Block* block = &game.players[playerIndex].block;
	while (MoveBlock(block, DOWN));
	Block nextBlock = NextBlock(playerIndex);
	memcpy(block, &nextBlock, sizeof(Block));
}

Block HoldBlock(int playerIndex) {
	Block* block = &game.players[playerIndex].block;
	RemoveFromBoard(block);
	if (game.holdBlock == -1) {
		game.holdBlock = block->type;
		return NextBlock(playerIndex);
	}
	else {
		Block newBlock = CreateBlock(game.holdBlock, game.players[playerIndex].blockInitPosition);
		game.holdBlock = block->type;
		return newBlock;
	}
}

void ResetData() {
	// clear board
	memset(game.board, 0, BOARD_WIDTH * BOARD_HEIGHT);
	// initialize blockQueue
	for (int i = 0; i < 7; i++) {
		game.blockQueue[i] = game.blockQueue[i + 7] = i;
	}
	for (int i = 0; i < 7; i++) {
		int  j = i + rand() / (RAND_MAX / (7 - i) + 1);
		int temp = game.blockQueue[j];
		game.blockQueue[j] = game.blockQueue[i];
		game.blockQueue[i] = temp;
		j = i + rand() / (RAND_MAX / (7 - i) + 1);
		temp = game.blockQueue[j + 7];
		game.blockQueue[j + 7] = game.blockQueue[i + 7];
		game.blockQueue[i + 7] = temp;
	}
	game.blockQueueIndex = 0;
	game.holdBlock = -1;
	game.score = 0;
	game.gameOver = 0;
	game.players = malloc(sizeof(PlayerData) * game.playerCount);
	for (int i = 0; i < game.playerCount; i++) {
		game.players[i].blockInitPosition = blockInitializePositions[i];
		game.players[i].lastDropTime = ((double)clock()) / CLOCKS_PER_SEC;
		game.players[i].block = NextBlock(i);
	}
}

void InitRotateConstraint() {
	int dataPointer = 0;
	for (int i = 0; i < 7 * 4 * 2; i++) {
		rotateConstraint[0][0][i].data = rawRotateConstraintData + dataPointer;
		dataPointer += rotateConstraint[0][0][i].size;
	}
}

int main(int argc, char* argv[]) {
	//renderBuffer = malloc(sizeof(char) * RANDER_WIDTH * RENDER_HEIGHT);
	double accumulationTime = ((double)clock()) / CLOCKS_PER_SEC;
	game.playerCount = 1;
	int redraw = 1;

	InitRotateConstraint();
	ResetData();
	while (1) {
		double currentTime = ((double)clock()) / CLOCKS_PER_SEC;
		double biasTime = (1.0 / FPS) - (currentTime - accumulationTime);
		while (kbhit()) {
			unsigned char key = getch();
			if (key == 27) {
				ResetData();
			}
			else if (!game.gameOver) {
				switch (key) {
				case 'z':
					RotateBlock(&game.players[0].block, 0);
					break;
				case 'x':
					RotateBlock(&game.players[0].block, 1);
					break;
				case 'c':
					game.players[0].block = HoldBlock(0);
					break;
				case ' ':
					FallBlock(0);
					game.players[0].lastDropTime = currentTime;
					break;
				case 224:
					key = getch();
					if (key == 72) {
						RotateBlock(&game.players[0].block, 1);
					}
					else if (key == 80) {
						if (MoveBlock(&game.players[0].block, DOWN)) {
							game.players[0].lastDropTime = currentTime;
						}
					}
					else if (key == 75) {
						MoveBlock(&game.players[0].block, LEFT);
					}
					else if (key == 77) {
						MoveBlock(&game.players[0].block, RIGHT);
					}
					break;
				default:
					break;
				}
			}
			redraw = 1;
		}
		for (int i = 0; i < game.playerCount; i++) {
			if (!game.gameOver && (currentTime - game.players[i].lastDropTime > DROP_TIME)) {
				if (!MoveBlock(&game.players[i].block, DOWN)) {
					game.players[i].block = NextBlock(i);
				}
				game.players[i].lastDropTime = currentTime;
				redraw = 1;
			}
		}
		if (redraw) {
			render();
			redraw = 0;
		}
		accumulationTime = currentTime + biasTime;
		double sleepTime = (1.0 / FPS) + biasTime;
		sleepTime = sleepTime > 0 ? sleepTime : 0;
		_sleep(sleepTime * 1000);
	}
	//free(renderBuffer);
	return 0;
}
