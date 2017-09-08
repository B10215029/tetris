#pragma once
#define BOARD_WIDTH 10
#define BOARD_HEIGHT 20
#define PointAdd(a, b) ((Point){ a.x + b.x, a.y + b.y })

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
} GameData;

extern const Point UP;
extern const Point DOWN;
extern const Point LEFT;
extern const Point RIGHT;
extern const Point BlockInitPosition;
extern const Point BlockData[7][4][4];

// 初始化遊戲必須的資料
void InitTetris();
// 重置遊戲資料
void ResetData(GameData* gameData);
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
// 方塊下降
void DropBlock(Block* block);
// 方塊瞬降
void FallBlock(Block* block);
// 保留方塊
void HoldBlock(Block* block);
// 判斷是否碰到炸彈並消除炸彈
int TriggerBomb(Block* block);
// 檢查位置
int PositionCheck(Point *newPosition, Point *oldPosition);
