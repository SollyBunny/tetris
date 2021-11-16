main.o:
	gcc main.c -o tetris -s -O3 -lpthread 
	./tetris

compile:
	gcc main.c -o tetris -s -O3 -lpthread

run:
	./tetris
