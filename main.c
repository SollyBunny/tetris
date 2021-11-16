// Imports
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <termios.h>
#include <pthread.h>

// Key codes
#define BREAK 3

#define KEYW 119
#define KEYA 97
#define KEYS 115
#define KEYD 100

#define KEYQ 113
#define KEYE 101

unsigned int gridx, gridy, gridsize;
unsigned int *grid;

unsigned int tiles[][4][4 * 4] = {

	{ // block
		{
			0, 0, 0, 0,
			0, 0, 0, 0,
			1, 1, 0, 0,
			1, 1, 0, 0
		}, {
			0, 0, 0, 0,
			0, 0, 0, 0,
			1, 1, 0, 0,
			1, 1, 0, 0
		},{
			0, 0, 0, 0,
			0, 0, 0, 0,
			1, 1, 0, 0,
			1, 1, 0, 0
		},{
			0, 0, 0, 0,
			0, 0, 0, 0,
			1, 1, 0, 0,
			1, 1, 0, 0
		},
	}, 

	{ // bar
		{
			0, 1, 0, 0,
			0, 1, 0, 0,
			0, 1, 0, 0,
			0, 1, 0, 0
		}, {
			0, 0, 0, 0,
			0, 0, 0, 0,
			1, 1, 1, 1,
			0, 0, 0, 0
		}, {
			0, 0, 1, 0,
			0, 0, 1, 0,
			0, 0, 1, 0,
			0, 0, 1, 0
		}, {
			0, 0, 0, 0,
			1, 1, 1, 1,
			0, 0, 0, 0,
			0, 0, 0, 0
		}
	}, 

	{ // S
		{
			0, 0, 0, 0,
			0, 0, 0, 0,
			0, 1, 1, 0,
			1, 1, 0, 0
		}, {
			0, 0, 0, 0,
			1, 0, 0, 0,
			1, 1, 0, 0,
			0, 1, 0, 0
		}, {
			0, 0, 0, 0,
			0, 1, 1, 0,
			1, 1, 0, 0,
			0, 0, 0, 0
		}, {
			0, 0, 0, 0,
			0, 1, 0, 0,
			0, 1, 1, 0,
			0, 0, 1, 0
		}
	}, 

	{ // Z
		{
			0, 0, 0, 0,
			0, 0, 0, 0,
			1, 1, 0, 0,
			0, 1, 1, 0
		}, {
			0, 0, 0, 0,
			0, 1, 0, 0,
			1, 1, 0, 0,
			1, 0, 0, 0
		}, {
			0, 0, 0, 0,
			1, 1, 0, 0,
			0, 1, 1, 0,
			0, 0, 0, 0
		}, {
			0, 0, 0, 0,
			0, 0, 1, 0,
			0, 1, 1, 0,
			0, 1, 0, 0
		}
	}, 

	{ // L
		{
			0, 0, 0, 0,
			0, 1, 0, 0,
			0, 1, 0, 0,
			0, 1, 1, 0
		}, {
			0, 0, 0, 0,
			0, 0, 0, 0,
			1, 1, 1, 0,
			1, 0, 0, 0
		}, {
			0, 0, 0, 0,
			1, 1, 0, 0,
			0, 1, 0, 0,
			0, 1, 0, 0
		}, {
			0, 0, 0, 0,
			0, 0, 1, 0,
			1, 1, 1, 0,
			0, 0, 0, 0
		}
	},

	{ // J
		{
			0, 0, 0, 0,
			0, 1, 0, 0,
			0, 1, 0, 0,
			1, 1, 0, 0
		}, {
			0, 0, 0, 0,
			1, 0, 0, 0,
			1, 1, 1, 0,
			0, 0, 0, 0
		}, {
			0, 0, 0, 0,
			0, 1, 1, 0,
			0, 1, 0, 0,
			0, 1, 0, 0
		}, {
			0, 0, 0, 0,
			0, 0, 0, 0,
			1, 1, 1, 0,
			0, 0, 1, 0
		}
	}, 

	{ //T
		{
			0, 0, 0, 0,
			0, 0, 0, 0,
			1, 1, 1, 0,
			0, 1, 0, 0
		}, {
			0, 0, 0, 0,
			0, 1, 0, 0,
			1, 1, 0, 0,
			0, 1, 0, 0
		}, {
			0, 0, 0, 0,
			0, 1, 0, 0,
			1, 1, 1, 0,
			0, 0, 0, 0
		}, {
			0, 0, 0, 0,
			0, 1, 0, 0,
			0, 1, 1, 0,
			0, 1, 0, 0
		}
	}
	
};

unsigned int currenttile;
unsigned int currentpos;
unsigned int currentdir;

void print() {

	unsigned int x, y;

	printf("\x1b[2J\x1b[H\x1b[48;5;1m");
	for (y = 0; y < gridy; ++y) {
		for (x = 0; x < gridx; ++x) {
			putchar(' ');
			putchar(' ');
		}
		putchar('\n');
	}

	printf("\x1b[48;5;255m\x1b[%u;%uH", currentpos / gridx, (currentpos % gridx) * 2 - 1);
	for (x = 0; x < 4 * 4; ++x) {
		if (x % 4 == 0 && x != 0) {
			printf("\x1b[1B\x1b[8D");
		}
		if (tiles[currenttile][currentdir][x] == 1) {
			putchar('#'); putchar('#');
		} else {
			printf("\x1b[2C");
			//printf("__");
		}
	}
	printf("\x1b[48;5;0m");
	fflush(stdout);

}

void move(unsigned int newpos) {

	unsigned int x, y, i;
	i = 0;
	for (y = 0; y < 4; ++y) {
		for (x = 0; x < 4; ++x) {

			if (tiles[currenttile][currentdir][i] == 1) {

				if ( (newpos + x) / gridx != newpos/gridx) {
					return;
				}
				
			}

			++i;
		}
	}

	currentpos = newpos;
	// debug
	if (currentpos > gridsize)
		currentpos -= gridsize;

	print();	

}

void input(void) {
	while (1) {
		move(currentpos + gridx);
		usleep(200000);
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

	currenttile = 2;
	currentpos = gridx / 2;
	currentdir = 0;

	// set terminal settins (dont echo characters / queue stdin / ignore ctrl-c )
	//system("/bin/stty -echo -ixon -icanon time 5 min 1 intr undef");
	struct termios term, restore;
	tcgetattr(0, &term);
	tcgetattr(0, &restore); // backup the original terminal state to restore later
	term.c_lflag &= ~(ICANON|ECHO|~ISIG);
	tcsetattr(0, TCSANOW, &term);

	printf("\x1b[?25l");

	pthread_t thread;
	pthread_create(&thread, NULL, (void *)&input, NULL);

	char c;
	while ((c = getchar()) != BREAK) {
		printf("%d", c);
		switch (c) {
			case KEYS:
				move(currentpos + gridx);
				break;
			case KEYA:
				// if (currentpos % gridx == 1)
					// break;
				move(currentpos - 1);
				break;
			case KEYD:
				// if (currentpos % gridx == gridx - 2)
					// break;
				move(currentpos + 1);
				break;
			case KEYQ:
				currentdir++;	
				if (currentdir >= 4) currentdir = 0;
				print();
				continue;
			case KEYE:
				currentdir--;	
				if (currentdir >= 4) currentdir = 3;
				print();
				continue;
		}

	}

	fputs("\x1b[?25h\x1b[0m\x1b[2J\x1b[H", stdout); // clear, show cursor, goto home position, assume color has been reset
	tcsetattr(0, TCSANOW, &restore); // restore terminal state

	return 0;
}
