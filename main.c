#include <stdlib.h>
#include <stdio.h>

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

void print() {

	unsigned int x, y;

	for (y = 0; y < gridy; ++y) {
		for (x = 0; x < gridx; ++x) {
			putchar('[');
			putchar(']');
		}
		putchar('\n');
	}

	printf("\x1b[%u;%uH", currentpos / gridy, currentpos % gridy);
	goto printstartloop;
	for (x = 0; x < 3 * 4; ++x) {
		if (x % 3 == 0) {
			printf("\x1b[%u;%uH", (currentpos / gridy) + (x / 3) + 1, currentpos % gridy);
		}
		printstartloop:
		if (tiles[currenttile][x] == 1) {
			putchar('#'); putchar('#');
		} else {
			putchar(' ');putchar(' ');
		}
	}

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

	printf("\x1b[2J\x1b[H");

	print();

	

	return 0;
}