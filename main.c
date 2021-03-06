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

struct Block {
	unsigned int data[4 * 4];
	unsigned int bounding[4];
};

struct Block tiles[][4] = {

	{ //block
		{
			{
				0, 0, 0, 0,
				0, 1, 1, 0,
				0, 1, 1, 0,
				0, 0, 0, 0
			}, {
				1, 1, 1, 1
			}
		}, {
			{
				0, 0, 0, 0,
				0, 1, 1, 0,
				0, 1, 1, 0,
				0, 0, 0, 0
			}, {
				1, 1, 1, 1
			}
		}, {
			{
				0, 0, 0, 0,
				0, 1, 1, 0,
				0, 1, 1, 0,
				0, 0, 0, 0
			}, {
				1, 1, 1, 1
			}
		}, {
			{
				0, 0, 0, 0,
				0, 1, 1, 0,
				0, 1, 1, 0,
				0, 0, 0, 0
			}, {
				1, 1, 1, 1
			}
		}
	}, { //bar
		{
			{
				0, 0, 1, 0,
				0, 0, 1, 0,
				0, 0, 1, 0,
				0, 0, 1, 0
			}, {
				2, 1, 0, 0
			}
		}, {
			{
				0, 0, 0, 0,
				0, 0, 0, 0,
				1, 1, 1, 1,
				0, 0, 0, 0
			}, {
				0, 0, 2, 1
			}
		}, {
			{
				0, 1, 0, 0,
				0, 1, 0, 0,
				0, 1, 0, 0,
				0, 1, 0, 0
			}, {
				1, 2, 0, 0
			}
		}, {
			{
				0, 0, 0, 0,
				0, 0, 0, 0,
				1, 1, 1, 1,
				0, 0, 0, 0
			}, {
				0, 0, 2, 1
			}
		}
	}, { //S
		{
			{
				0, 0, 0, 0,
				0, 0, 0, 0,
				0, 1, 1, 0,
				1, 1, 0, 0
			}, {
				0, 1, 2, 0
			}
		}, {
			{
				0, 0, 0, 0,
				1, 0, 0, 0,
				1, 1, 0, 0,
				0, 1, 0, 0
			}, {
				0, 2, 1, 0
			}
		}, {
			{
				0, 0, 0, 0,
				0, 0, 0, 0,
				0, 1, 1, 0,
				1, 1, 0, 0
			}, {
				0, 1, 2, 0
			}
		}, {
			{
				0, 0, 0, 0,
				1, 0, 0, 0,
				1, 1, 0, 0,
				0, 1, 0, 0
			}, {
				0, 2, 1, 0
			}
		}
	}, { //Z
		{
			{
				0, 0, 0, 0,
				0, 0, 0, 0,
				1, 1, 0, 0,
				0, 1, 1, 0
			}, {
				0, 1, 2, 0
			}
		}, {
			{
				0, 0, 0, 0,
				0, 0, 1, 0,
				0, 1, 1, 0,
				0, 1, 0, 0
			}, {
				1, 1, 1, 0
			}
		}, {
			{
				0, 0, 0, 0,
				0, 0, 0, 0,
				1, 1, 0, 0,
				0, 1, 1, 0
			}, {
				0, 1, 2, 0
			}
		}, {
			{
				0, 0, 0, 0,
				0, 0, 1, 0,
				0, 1, 1, 0,
				0, 1, 0, 0
			}, {
				1, 1, 1, 0
			}
		}
	}, { //L
		{
			{
				0, 0, 0, 0,
				0, 0, 1, 0,
				1, 1, 1, 0,
				0, 0, 0, 0
			}, {
				1, 0, 1, 1
			}
		}, {
			{
				0, 0, 0, 0,
				0, 1, 0, 0,
				0, 1, 0, 0,
				0, 1, 1, 0
			}, {
				1, 1, 1, 0
			}
		}, {
			{
				0, 0, 0, 0,
				0, 0, 0, 0,
				1, 1, 1, 0,
				1, 0, 0, 0
			}, {
				0, 1, 2, 0
			}
		}, {
			{
				0, 0, 0, 0,
				1, 1, 0, 0,
				0, 1, 0, 0,
				0, 1, 0, 0
			}, {
				0, 2, 1, 0
			}
		}
	}, { //J
		{
			{
				0, 0, 0, 0,
				1, 0, 0, 0,
				1, 1, 1, 0,
				0, 0, 0, 0
			}, {
				0, 1, 1, 1
			}
		}, {
			{
				0, 0, 0, 0,
				0, 1, 1, 0,
				0, 1, 0, 0,
				0, 1, 0, 0
			}, {
				1, 1, 0, 1
			}
		}, {
			{
				0, 0, 0, 0,
				0, 0, 0, 0,
				1, 1, 1, 0,
				0, 0, 1, 0
			}, {
				0, 1, 2, 0
			}
		}, {
			{
				0, 0, 0, 0,
				0, 1, 0, 0,
				0, 1, 0, 0,
				1, 1, 0, 0
			}, {
				0, 2, 1, 0
			}
		}
	}, { //T
		{
			{
				0, 0, 0, 0,
				0, 0, 0, 0,
				1, 1, 1, 0,
				0, 1, 0, 0
			}, {
				0, 1, 2, 0
			}
		}, {
			{
				0, 0, 0, 0,
				0, 1, 0, 0,
				1, 1, 0, 0,
				0, 1, 0, 0
			}, {
				0, 2, 1, 0
			}
		}, {
			{
				0, 0, 0, 0,
				0, 1, 0, 0,
				1, 1, 1, 0,
				0, 0, 0, 0
			}, {
				0, 1, 1, 1
			}
		}, {
			{
				0, 0, 0, 0,
				0, 1, 0, 0,
				0, 1, 1, 0,
				0, 1, 0, 0
			}, {
				1, 1, 1, 0
			}
		}
	}
};

unsigned int tilescolor[] = {
	226, //block
	87, //bar
	46, //s
	45, //z
	208,//l
	21, //j
	93 //t
};

#define tiletimeout 5;

unsigned int currenttile;
unsigned int currentpos;
unsigned int currentdir;
unsigned int currenttimeout;

struct termios term, restore;
void sexit() {
	printf("\x1b[?25h\x1b[0m\x1b[%u;0H", gridy + 1); //show cursor, goto end position, assume color has been reset
	tcsetattr(0, TCSANOW, &restore); // restore terminal state
	exit(0);
}

void print() {

	unsigned int x, y, t;

	printf("\x1b[2J\x1b[H\x1b[48;5;1m");

	// TODO clear with color, then empty out screen ONCE

	for (t = 0; t < gridsize; ++t) {

		if (grid[t] != 0) { 
			do { // continue rendering tiles untill theres no more (or theres a newline)
				printf("\x1b[48;5;%dm  ", tilescolor[grid[t] - 1]);
				if (t % gridx == gridx - 1) // TODO messy :P // same as line ~334
					putchar('\n');	
				++t;
			} while (t < gridsize && (grid[t] != 0));
			printf("\x1b[48;5;1m");
			--t;
		} else {

			printf("  ");
			if (t % gridx == gridx - 1) // TODO messy :P
				putchar('\n');	

		}

	}

	//print current tile falling
	y = (currentpos % (gridx + 3));
	// if (y == 0)
		// printf("%d\x1b[48;5;255m\x1b[%u;%uH", y, currentpos / gridx, 0);
	// else
	printf("\x1b[48;5;%dm", tilescolor[currenttile]);
	if (y - 3 > gridx) { // (t < 0) // compatibility with terminals with line wrap when you goto position -1
		printf("\x1b[%u;0H", (currentpos / (gridx + 3) - 3));
	} else {
		printf("\x1b[%u;%uH", (currentpos / (gridx + 3) - 3), y * 2 - 5);
	}

	x = currentpos / (gridx + 3);
	if (x < 4) {
		x = (3 - x) * 4;
	} else x = 0;

	t = x % 4;
	goto printloopstart;
	for (; x < 4 * 4; ++x) {

		t = x % 4;

		if (t == 0) {
			printf("\x1b[8D\x1b[1B");
		}

		printloopstart:

		if (y < 3 && t < (3 - y)) continue;  // dont render stuff off the left side
		
		if (tiles[currenttile][currentdir].data[x] == 1) {
			putchar(' '); putchar(' ');
		} else {
			printf("\x1b[2C");
			// /printf("__"); // debug
		}
		
	}
	printf("\x1b[48;5;0m");
	fflush(stdout);

}

// TODO lmao doesnt work
void linecheck() {

	unsigned int y = 0;

	for (; y < gridy; ++y) {

		for (unsigned int x = 0; x < gridx; ++x) {

			if (grid[y * gridx + x]	!= 0) {
				goto nonfullline;
			}

		}

		goto fullline;

		nonfullline:
			continue;

	}

	return;

	fullline:

		printf("Line %d fill\n", y);

		exit(0);

}

void tilereset() {
	// if (currenttile == 6) currenttile = 0;
	// else currenttile++;
	currenttile = 6;//rand() % 7;
	currentpos = gridx / 2;
	currentdir = 0;
	currenttimeout = tiletimeout;
	print();
}

void movedir(unsigned int newdir) {

	if      (newdir >  4) newdir = 3;
	else if (newdir == 4) newdir = 0;

	unsigned int i;
	unsigned int newpos = currentpos;
	unsigned int x = (currentpos % (gridx + 3));
	unsigned int y = currentpos / (gridx + 3) + 1;

	if (x > gridx / 2 + tiles[currenttile][currentdir].bounding[1] - 1) {
		if (x > gridx) {
			newpos -= tiles[currenttile][currentdir].bounding[1] - 1;
			x      -= tiles[currenttile][currentdir].bounding[1] - 1;
		}
	} else {
		if (x < tiles[currenttile][currentdir].bounding[0] + 3) {
			newpos += x - tiles[currenttile][currentdir].bounding[0];
			x      += x - tiles[currenttile][currentdir].bounding[0];
		}
	}

	// skip out top chunk (seg fault prot)
	if (y < 7) i = (7 - y) * 4;
	else       i = 0; // probs redundant
	
	for (; i < 4 * 4; ++i) {

		if (tiles[currenttile][newdir].data[i]) {

			while (grid[
				( // y value
					y
					+ (i / 4)
					- 4
				) * gridx
				+ x
				+ (i % 4)
				- 3
			]) {
<<<<<<< HEAD
				y--;
				newpos -= gridx;
				newpos -= 3;
			}

		}

	}

	for (; i < 4 * 4; ++i) {

		if (tiles[currenttile][newdir].data[i]) {

			while (grid[
				( // y value
					y
					+ (i / 4)
					- 4
				) * gridx
				+ x
				+ (i % 4)
				- 3
			]) {
				return;
=======
				
>>>>>>> origin/main
			}

		}

	}

	currentdir = newdir;
	currentpos = newpos;
	currenttimeout = tiletimeout;

	print();

}

void movedown() {

	unsigned int i;
	unsigned int x = (currentpos % (gridx + 3));
	unsigned int y = currentpos / (gridx + 3) + 1;

	// calculate collision of bottom
	if (
		y > gridy - 3 &&
		y > gridy + tiles[currenttile][currentdir].bounding[3]
	) goto movecollision;

	// skip out top chunk (seg fault prot)
	if (y < 7) i = (7 - y) * 4;
	else       i = 0; // probs redundant
	
	for (; i < 4 * 4; ++i) {

		if (tiles[currenttile][currentdir].data[i]) {

		// i = iterator for tile information
			if (grid[
				( // y value
					y
					+ (i / 4)
					- 4
				) * gridx
				+ x
				+ (i % 4)
				- 3
			]) {
				if (y < 5) {
					if (currenttile == 6) currenttile = 0;
					else currenttile++;
					currentpos = gridx * 1.5 + 3;
					currentdir = 0;
					print();
					sexit();
				}
				goto movecollision;
			}

		}

	}

	currentpos += gridx;
	currentpos += 3;

	print();
	
	return;

	movecollision:

		if (currenttimeout == 0) currenttimeout = tiletimeout;
		currenttimeout--;
		if (currenttimeout > 0) return;	

		for (i = 0; i < 4 * 4; ++i) {
			if (tiles[currenttile][currentdir].data[i]) {
				grid[
					( // y value
						y
						+ (i / 4)
						- 5
					) * gridx
					+ x
					+ (i % 4)
					- 3
				] = currenttile + 1;
			}
		}

		tilereset();

	print();
	
}

void move(unsigned int newpos) {

	unsigned int i;
	unsigned int x = (newpos % (gridx + 3));
	unsigned int y = newpos / (gridx + 3);

	// calc collsion left/right
	if (x > gridx / 2) {
		while (x > tiles[currenttile][currentdir].bounding[1] + gridx - 1) {
			newpos--;
			x = (newpos % (gridx + 3));
		}
	} else {
		while (x < 3 - tiles[currenttile][currentdir].bounding[0]) {
			newpos++;
			x = (newpos % (gridx + 3));
		}
	}

	// skip out top chunk (seg fault prot)
	if (y < 4) i = ((4 - y) + 1) * 4;
	else       i = 0; // probs redundant
	
	for (; i < 4 * 4; ++i) {
		if (tiles[currenttile][currentdir].data[i]) {

			while (grid[
				( // y value
					y
					+ (i / 4)
					- 4
				) * gridx
				+ (newpos % (gridx + 3))
				+ (i % 4)
				- 3
			]) {
				return;
			};

		}
	}

	currenttimeout = tiletimeout;
	currentpos = newpos;
	
	print();	

}

void input(void) {
	while (1) {
		movedown(currentpos + gridx + 3);
		usleep(200000);
	}
}

int main(void) {

	time_t _t;
	srand(time(&_t));

	gridx = 15;//10;
	gridy = 21;
	gridsize = gridx * gridy;

	grid = malloc(gridsize * sizeof(unsigned int));
	for (unsigned int i = 0; i < gridsize; ++i) {
		grid[i] = 0;
	} // idk y but malloc is giving me all 0's even tho its uninitialized idk

	// currenttile = 1;
	// currentpos = gridx / 2;
	// currentdir = 0;
	tilereset();

	// set terminal settins (dont echo characters / queue stdin / ignore ctrl-c )
	tcgetattr(0, &term);
	tcgetattr(0, &restore); // backup the original terminal state to restore later
	term.c_lflag &= ~(ICANON|ECHO|~ISIG);
	tcsetattr(0, TCSANOW, &term);

	printf("\x1b[?25l");

	pthread_t thread;
	pthread_create(&thread, NULL, (void *)&input, NULL);

	char c;
	while ((c = getchar()) != BREAK) {
		// printf("%d, %d|", currentpos, c); // debug
		switch (c) {
			case KEYS:
				currenttimeout = 1;
				movedown();
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
				movedir(currentdir + 1);
				continue;
			case KEYE:
				movedir(currentdir - 1);
				continue;
		}

	}

	sexit();
	
}
