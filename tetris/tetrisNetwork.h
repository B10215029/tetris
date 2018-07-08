#pragma once
#include "tetris.h"
GameData* CreateServer(GameData* data);
GameData* ConnectServer(GameData* data, char* serverIP);
void SendData(GameData* data);
void SendBomb(int number);
int dataUpdate;
