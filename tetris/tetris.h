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

// ��l�ƹC�����������
void InitTetris();
// ���m�C�����
void ResetData(GameData* gameData);
// ������
void RotateBlock(Block* block, int direction);
// ���ʤ��
int MoveBlock(Block* block, Point direction);
// ���o�U�@�Ӥ������
int PopBlockQueue();
// ���ͤ��
Block CreateBlock(int type);
// ����B�p���B���ͤU�@�Ӥ��
Block NextBlock();
// ����U��
void DropBlock(Block* block);
// �������
void FallBlock(Block* block);
// �O�d���
void HoldBlock(Block* block);
// �P�_�O�_�I�쬵�u�î������u
int TriggerBomb(Block* block);
// �ˬd��m
int PositionCheck(Point *newPosition, Point *oldPosition);
