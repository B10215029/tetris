#pragma warning(disable : 4996)
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <time.h>

int main(int argc, char* argv[]) {
	printf("hello\n");
	char pr;
	time_t timer;
	struct tm y2k = { 0 };
	double seconds;

	y2k.tm_hour = 0;   y2k.tm_min = 0; y2k.tm_sec = 0;
	y2k.tm_year = 100; y2k.tm_mon = 0; y2k.tm_mday = 1;

	time(&timer);  /* get current time; same as: timer = time(NULL)  */
	seconds = difftime(timer, mktime(&y2k));

	double t = ((double)clock()) / CLOCKS_PER_SEC;
	while (1) {
		while (kbhit()) {
			pr = getch();
			cprintf("\"%d\"", pr);
		}
		double nt = ((double)clock()) / CLOCKS_PER_SEC;
		double dt = nt - t;
		double wt = (1.0 / 60.0) + ((1.0 / 60.0) - dt);
		//printf("%d\n", dt);
		t = nt + (1.0 / 60.0) - dt;
		
		//if ((1.0 / 60.0) - dt <= 0) {
		//	t += (1.0 / 60.0) - dt;
		//}
		time(&timer);  /* get current time; same as: timer = time(NULL)  */
		double newseconds = difftime(timer, mktime(&y2k));
		if ((int)newseconds != (int)seconds) {
			putch('|');
		}
		else {
			putch((1.0 / 60.0) - dt <= 0 ? '*' : '.');
			//putch('.');
		}
		seconds = newseconds;
		wt = wt > 0 ? wt : 0;
		_sleep(wt * 1000);
		//printf(".");
		//putch('.');
	}
	system("pause");
	return 0;
}
