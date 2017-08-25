#pragma warning(disable : 4996)
#define FPS 60
#define RANDER_WIDTH 100
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>
#include <string.h>
char* renderBuffer;

	//system("cls");
	for (int i = 0; i < RENDER_HEIGHT; i++) {
		renderBuffer[RANDER_WIDTH * i + RANDER_WIDTH - 1] = '\n';
	}
	renderBuffer[x + RANDER_WIDTH * y] = '#';
	puts(renderBuffer);
}

int main(int argc, char* argv[]) {
	renderBuffer = malloc(sizeof(char) * RANDER_WIDTH * RENDER_HEIGHT);
	double lastUpdateTime = ((double)clock()) / CLOCKS_PER_SEC;
	double accumulationTime = ((double)clock()) / CLOCKS_PER_SEC;
	while (1) {
		double currentTime = ((double)clock()) / CLOCKS_PER_SEC;
		double biasTime = (1.0 / FPS) - (currentTime - accumulationTime);
		while (kbhit()) {
			//cprintf("\"%d\"", key);
			char key = getch();
			if (key == 'w') {
				y--;
			}
			if (key == 'a') {
				x--;
			}
			if (key == 's') {
				y++;
			}
			if (key == 'd') {
				x++;
			}
		}
		//putch(((int)lastUpdateTime != (int)currentTime) ? '|' : (biasTime <= 0 ? '*' : '.'));
		//if ((int)lastUpdateTime != (int)currentTime)
		lastUpdateTime = currentTime;
		accumulationTime = currentTime + biasTime;
		double sleepTime = (1.0 / FPS) + biasTime;
		sleepTime = sleepTime > 0 ? sleepTime : 0;
		_sleep(sleepTime * 1000);
	}
	free(renderBuffer);
	system("pause");
	return 0;
}
