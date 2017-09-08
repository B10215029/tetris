#pragma warning(disable : 4996)
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include "tetris.h"
#include "tetrisCLIRender.h"
#include "audio.h"

#define FPS 30
#define DROP_TIME 1

int main(int argc, char* argv[]) {
	// ��l�Ƹ��
	double accumulationTime = ((double)clock()) / CLOCKS_PER_SEC;
	double lastDropTime = accumulationTime;
	GameData game;
	int redraw = 1;
	InitTetris();
	ResetData(&game);
	// ����BGM
	PlayMusic();
	while (1) {
		double currentTime = ((double)clock()) / CLOCKS_PER_SEC;
		// ���䱱��
		while (kbhit()) {
			unsigned char key = getch();
			if (key == 27) {
				ResetData(&game);
				lastDropTime = currentTime;
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
					lastDropTime = currentTime;
					break;
				case 224:
					key = getch();
					if (key == 72) {
						RotateBlock(&game.block, 1);
					}
					else if (key == 80) {
						if (MoveBlock(&game.block, DOWN)) {
							lastDropTime = currentTime;
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
		// ����U��
		if (!game.gameOver && (currentTime - lastDropTime > DROP_TIME)) {
			DropBlock(&game.block);
			lastDropTime = currentTime;
			redraw = 1;
		}
		// ��s�e��
		if (redraw) {
			Render(&game);
			redraw = 0;
		}
		// �p��U����s�ɶ�
		double biasTime = (1.0 / FPS) - (currentTime - accumulationTime);
		accumulationTime = currentTime + biasTime;
		double sleepTime = (1.0 / FPS) + biasTime;
		sleepTime = sleepTime > 0 ? sleepTime : 0;
		_sleep((int)(sleepTime * 1000));
	}
	return 0;
}
