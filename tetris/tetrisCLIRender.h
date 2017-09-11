#pragma once
#include "tetris.h"

#define RANDER_WIDTH 120
#define RENDER_HEIGHT 30
#define ScreenPointToPosition(point) (point.x + point.y * RANDER_WIDTH)

typedef struct {
	int x;
	int y;
} ScreenPoint;

// ­«Ã¸µe­±
void Render(GameData* gameData);
void Render2P(GameData* gameData1, GameData* gameData2);
