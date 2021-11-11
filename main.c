#include <stdlib.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>

// Key codes
#define BREAK 3

#define KEYW 119
#define KEYA 97
#define KEYS 115
#define KEYD 100

unsigned int gridx, gridy, gridsize;
unsigned int *grid;

unsigned int tiles[][3 * 4] = {
	{ // block
		0, 0, 0,
		0, 0, 0,
		1, 1, 0,
		1, 1, 0
	}, { // bar
		1, 0, 0,
		1, 0, 0,
		1, 0, 0,
		1, 0, 0
	}, { // S
		0, 0, 0,
		0, 0, 0,
		0, 1, 1,
		1, 1, 0
	}, { // Z
		0, 0, 0,
		0, 0, 0,
		1, 1, 0,
		0, 1, 1
	}, { // L
		1, 0, 0,
		1, 0, 0,
		1, 0, 0,
		1, 1, 0
	}, { // J
		0, 1, 0,
		0, 1, 0,
		0, 1, 0,
		1, 1, 0
	}, { //T
		0, 0, 0,
		0, 0, 0,
		1, 1, 1,
		0, 1, 0
	}
};

unsigned int currenttile;
unsigned int currentpos;

// Used instead of getchar, it will return 0 if theres no character pressed, otherwise returned pressed
char _waiting;
char sgetchar() {
	ioctl(0, FIONREAD, &_waiting);
	if (_waiting == 0) return 0;
	return getchar();
}

void print() {

	unsigned int x, y;

	for (y = 0; y < gridy; ++y) {
		for (x = 0; x < gridx; ++x) {
			putchar(' ');
			putchar(' ');
		}
		putchar('\n');
	}

	printf("\x1b[48;5;255m\x1b[%u;%uH", currentpos / gridx, (currentpos % gridx) * 2);
	for (x = 0; x < 3 * 4; ++x) {
		if (x % 3 == 0 && x != 0) {
			printf("\x1b[1B\x1b[6D");
		}
		if (tiles[currenttile][x] == 1) {
			putchar('#'); putchar('#');
		} else {
			printf("\x1b[2C");
		}
	}
	printf("\x1b[48;5;0m");

}

int main(void) {

	gridx = 10;
	gridy = 21;
	gridsize = gridx * gridy;

	grid = malloc(gridsize * sizeof(unsigned int));
	for (unsigned int *m = grid; m; m++) {
		*m = 0;
	}

	currenttile = 0;
	currentpos = gridx / 2;

	printf("\x1b[?25l");

	unsigned int count = 0;
	char c;
	while ((c = sgetchar()) != BREAK) {

		switch (c) {
			case KEYS:
				currentpos += gridx;
				break;
			case KEYA:
				if (currentpos % gridx == 0) 
					break;
				currentpos -= 1;
				break;
			case KEYD:
				if (currentpos % gridx == gridx) 
					break;
				currentpos += 1;
				break;
		}

		printf("\x1b[2J\x1b[H");

		print();
		fflush(stdout);
		// printf("%d\n", count);

		usleep(200000);
		

	}

	return 0;
}