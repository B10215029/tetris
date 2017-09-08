#pragma once
#include <Windows.h>

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

void PlayMusic() {
	HANDLE thread = CreateThread(NULL, 0, BackgroundMusic, NULL, 0, NULL);
}
