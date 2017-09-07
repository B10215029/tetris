#pragma warning(disable : 4996)
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <string.h>
#include <Windows.h>

#define FPS 30
#define RANDER_WIDTH 120
#define RENDER_HEIGHT 30
#define BOARD_WIDTH 10
#define BOARD_HEIGHT 20
#define DROP_TIME 1
#define ScreenPointToPosition(point) (point.x + point.y * RANDER_WIDTH)
#define PointAdd(a, b) ((Point){ a.x + b.x, a.y + b.y })

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
	char board[BOARD_HEIGHT][BOARD_WIDTH];
	int blockQueue[7 * 2];
	int blockQueueIndex;
	int holdBlock;
	int bomb;
	int ko;
	int score;
	int gameOver;
	Block block;
	double lastDropTime;
} GameData;

const ScreenPoint boardPosition = { 14, 3 };
const ScreenPoint holdPosition = { 2, 3 };
const ScreenPoint nextPosition = { 38, 3 };
const ScreenPoint scorePosition = { 2, 20 };
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

GameData game;
char renderBuffer[RANDER_WIDTH * RENDER_HEIGHT];

int IsInBound(Point position);
int IsEmpty(Point position);
int IsInclude(Point position, Point data[4]);
int PositionCheck(Point *newPosition, Point *oldPosition);

void RemoveFromBoard(Block* block);
void AddToBoard(Block* block);
void UpdateBlockData(Block* block);

void ResetData();
void InitRotateConstraint();

// 旋轉方塊
void RotateBlock(Block* block, int direction);
// 移動方塊
int MoveBlock(Block* block, Point direction);
// 取得下一個方塊類型
int PopBlockQueue();
// 產生方塊
Block CreateBlock(int type);
// 消行、計分、產生下一個方塊
Block NextBlock();
// 方塊瞬降
void FallBlock(Block* block);
// 保留方塊
void HoldBlock(Block* block);
// 重繪畫面
void render();
// 判斷是否碰到炸彈並消除炸彈
int TriggerBomb(Block* block);

void render() {
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
			if (game.board[y][x] == 0) {
				//renderBuffer[ScreenPointToPosition(boardPosition) + ScreenPointToPosition(p)] = 161;
				//renderBuffer[ScreenPointToPosition(boardPosition) + ScreenPointToPosition(p) + 1] = 188;
			}
			else if (game.board[y][x] == 1) {
				renderBuffer[ScreenPointToPosition(boardPosition) + ScreenPointToPosition(p)] = 161;
				renderBuffer[ScreenPointToPosition(boardPosition) + ScreenPointToPosition(p) + 1] = 189;
			}
			else if (game.board[y][x] == 2) {
				renderBuffer[ScreenPointToPosition(boardPosition) + ScreenPointToPosition(p)] = 161;
				renderBuffer[ScreenPointToPosition(boardPosition) + ScreenPointToPosition(p) + 1] = 242;
			}
		}
	}
	// render shadow
	Point shadowPosition[4];
	memcpy(shadowPosition, game.block.data, sizeof(shadowPosition));
	while (PositionCheck(shadowPosition, game.block.data)) {
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
	// render hold
	memcpy(renderBuffer + ScreenPointToPosition(holdPosition), "ＨＯＬＤ", sizeof(char) * 8);
	if (game.holdBlock != -1) {
		int iconPosition = 2 + 3 * RANDER_WIDTH;
		for (int i = 0; i < 4; i++) {
			ScreenPoint p = { BlockData[game.holdBlock][0][i].x * 2, BlockData[game.holdBlock][0][i].y };
			renderBuffer[ScreenPointToPosition(holdPosition) + iconPosition + ScreenPointToPosition(p)] = 161;
			renderBuffer[ScreenPointToPosition(holdPosition) + iconPosition + ScreenPointToPosition(p) + 1] = 189;
		}
	}
	// render next
	memcpy(renderBuffer + ScreenPointToPosition(nextPosition), "ＮＥＸＴ", sizeof(char) * 8);
	int iconPosition = 2 + 3 * RANDER_WIDTH;
	for (int blockI = 0; blockI < 5; blockI++) {
		for (int i = 0; i < 4; i++) {
			ScreenPoint p = { BlockData[game.blockQueue[game.blockQueueIndex + blockI]][0][i].x * 2, BlockData[game.blockQueue[game.blockQueueIndex + blockI]][0][i].y };
			renderBuffer[ScreenPointToPosition(nextPosition) + iconPosition + ScreenPointToPosition(p)] = 161;
			renderBuffer[ScreenPointToPosition(nextPosition) + iconPosition + ScreenPointToPosition(p) + 1] = 189;
		}
		iconPosition += RANDER_WIDTH * 3;
	}
	// render score
	memcpy(renderBuffer + ScreenPointToPosition(scorePosition), "ＬＩＮＥ", sizeof(char) * 8);
	char scoreBuffer[10];
	itoa(game.score, scoreBuffer, 10);
	memcpy(renderBuffer + (ScreenPointToPosition(scorePosition) + RANDER_WIDTH * 2), scoreBuffer, sizeof(char) * strlen(scoreBuffer));

	strcpy(renderBuffer + RANDER_WIDTH * (RENDER_HEIGHT - 1), game.gameOver ? "Game Over" : "");
	// print to screen
	puts(renderBuffer);
}

int IsInBound(Point position) {
	return position.x >= 0 && position.x < BOARD_WIDTH && position.y < BOARD_HEIGHT;
}

int IsEmpty(Point position) {
	return position.y < 0 || game.board[position.y][position.x] == 0;
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

int TriggerBomb(Block* block) {
	for (int i = 0; i < 4; i++) {
		if ((block->data[i].y + 1) >= 0 && (block->data[i].y + 1) < BOARD_HEIGHT && game.board[block->data[i].y + 1][block->data[i].x] == 2) {
			game.board[block->data[i].y + 1][block->data[i].x] = 1;
			for (int j = block->data[i].y + 2; j < BOARD_HEIGHT; j++) {
				if (game.board[j][block->data[i].x] == 2) {
					game.board[j][block->data[i].x] = 1;
				}
			}
		}
	}
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
	int nextBlock = game.blockQueue[game.blockQueueIndex++];
	if (game.blockQueueIndex == 7) {
		memcpy(game.blockQueue, game.blockQueue + 7, sizeof(int) * 7);
		// shuffle blockQueue last 7 elements
		for (int i = 0; i < 7; i++) {
			int  j = i + rand() / (RAND_MAX / (7 - i) + 1);
			int temp = game.blockQueue[j + 7];
			game.blockQueue[j + 7] = game.blockQueue[i + 7];
			game.blockQueue[i + 7] = temp;
		}
		game.blockQueueIndex = 0;
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
			game.gameOver = 1;
		}
	}
	UpdateBlockData(&newBlock);
	return newBlock;
}

Block NextBlock() {
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
			game.score++;
			putchar('\a');
		}
	}
	while (game.bomb > 0) {
		memcpy(game.board[0], game.board[1], sizeof(char) * BOARD_WIDTH * (BOARD_HEIGHT - 1));
		memcpy(game.board[BOARD_HEIGHT - 1], fillRow, sizeof(char) * BOARD_WIDTH);
		game.board[BOARD_HEIGHT - 1][rand() / (RAND_MAX / BOARD_WIDTH)] = 2;
		game.bomb--;
	}
	return CreateBlock(PopBlockQueue());
}

void FallBlock(Block* block) {
	while (MoveBlock(block, DOWN));
	TriggerBomb(block);
	Block nextBlock = NextBlock();
	memcpy(block, &nextBlock, sizeof(Block));
}

void HoldBlock(Block* block) {
	Block newBlock;
	RemoveFromBoard(block);
	newBlock = game.holdBlock == -1 ? NextBlock() : CreateBlock(game.holdBlock);
	game.holdBlock = block->type;
	memcpy(block, &newBlock, sizeof(Block));
}

void ResetData() {
	// clear board
	memset(game.board, 0, BOARD_WIDTH * BOARD_HEIGHT);
	// initialize blockQueue
	for (int i = 0; i < 7; i++) {
		game.blockQueue[i] = game.blockQueue[i + 7] = i;
	}
	game.blockQueueIndex = 6;
	PopBlockQueue();
	game.blockQueueIndex = 6;
	PopBlockQueue();
	game.holdBlock = -1;
	game.bomb = 0;
	game.score = 0;
	game.gameOver = 0;
	game.lastDropTime = ((double)clock()) / CLOCKS_PER_SEC;
	game.block = NextBlock();
}

void InitRotateConstraint() {
	int dataPointer = 0;
	for (int i = 0; i < 7 * 4 * 2; i++) {
		rotateConstraint[0][0][i].data = rawRotateConstraintData + dataPointer;
		dataPointer += rotateConstraint[0][0][i].size;
	}
}

DWORD WINAPI BackgroundMusic(void* data) {
	while (1) {
		int i = 0;
		while (i < 2) {
			Beep(1320, 500);
			Beep(990, 250);
			Beep(1056, 250);
			Beep(1188, 500);
			Beep(1056, 250);
			Beep(990, 250);
			Beep(880, 500);
			Beep(880, 250);
			Beep(1056, 250);
			Beep(1320, 500);
			Beep(1188, 250);
			Beep(1056, 250);
			Beep(990, 750);
			Beep(1056, 250);
			Beep(1188, 500);
			Beep(1320, 500);
			Beep(1056, 500);
			Beep(880, 500);
			Beep(880, 500);
			Beep(0, 250);
			Beep(1188, 500);
			Beep(1408, 250);
			Beep(1760, 500);
			Beep(1584, 250);
			Beep(1408, 250);
			Beep(1320, 750);
			Beep(1056, 250);
			Beep(1320, 500);
			Beep(1188, 250);
			Beep(1056, 250);
			Beep(990, 500);
			Beep(990, 250);
			Beep(1056, 250);
			Beep(1188, 500);
			Beep(1320, 500);
			Beep(1056, 500);
			Beep(880, 500);
			Beep(880, 500);
			Beep(0, 500);
			i++;
		}
		while (i < 4) {
			Beep(660, 1000);
			Beep(528, 1000);
			Beep(594, 1000);
			Beep(495, 1000);
			Beep(528, 1000);
			Beep(440, 1000);
			Beep(419, 1000);
			Beep(495, 1000);
			Beep(660, 1000);
			Beep(528, 1000);
			Beep(594, 1000);
			Beep(495, 1000);
			Beep(528, 500);
			Beep(660, 500);
			Beep(880, 1000);
			Beep(838, 2000);
			i++;
		}
		Beep(0, 5000);
	}
	return 0;
}

int main(int argc, char* argv[]) {
	HANDLE thread = CreateThread(NULL, 0, BackgroundMusic, NULL, 0, NULL);
	double accumulationTime = ((double)clock()) / CLOCKS_PER_SEC;
	int redraw = 1;
	InitRotateConstraint();
	ResetData();
	while (1) {
		double currentTime = ((double)clock()) / CLOCKS_PER_SEC;
		// 按鍵控制
		while (kbhit()) {
			unsigned char key = getch();
			if (key == 27) {
				ResetData();
			}
			else if (!game.gameOver) {
				switch (key) {
				case '`':
					game.bomb++;
					break;
				case 'z':
					RotateBlock(&game.block, 0);
					break;
				case 'x':
					RotateBlock(&game.block, 1);
					break;
				case 'c':
					HoldBlock(&game.block);
					break;
				case ' ':
					FallBlock(&game.block);
					game.lastDropTime = currentTime;
					break;
				case 224:
					key = getch();
					if (key == 72) {
						RotateBlock(&game.block, 1);
					}
					else if (key == 80) {
						if (MoveBlock(&game.block, DOWN)) {
							game.lastDropTime = currentTime;
						}
					}
					else if (key == 75) {
						MoveBlock(&game.block, LEFT);
					}
					else if (key == 77) {
						MoveBlock(&game.block, RIGHT);
					}
					break;
				default:
					break;
				}
			}
			redraw = 1;
		}
		// 方塊下降
		if (!game.gameOver && (currentTime - game.lastDropTime > DROP_TIME)) {
			if (!MoveBlock(&game.block, DOWN)) {
				TriggerBomb(&game.block);
				game.block = NextBlock();
			}
			game.lastDropTime = currentTime;
			redraw = 1;
		}
		// 更新畫面
		if (redraw) {
			render();
			redraw = 0;
		}
		double biasTime = (1.0 / FPS) - (currentTime - accumulationTime);
		accumulationTime = currentTime + biasTime;
		double sleepTime = (1.0 / FPS) + biasTime;
		sleepTime = sleepTime > 0 ? sleepTime : 0;
		_sleep((int)(sleepTime * 1000));
	}
	return 0;
}
