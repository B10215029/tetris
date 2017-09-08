#pragma once
#include "tetris.h"

#define RANDER_WIDTH 120
#define RENDER_HEIGHT 30
#define ScreenPointToPosition(point) (point.x + point.y * RANDER_WIDTH)

typedef struct {
	int x;
	int y;
} ScreenPoint;

extern const ScreenPoint boardPosition;
extern const ScreenPoint holdPosition;
extern const ScreenPoint nextPosition;
extern const ScreenPoint scorePosition;

// ­«Ã¸µe­±
void Render(GameData* gameData);
