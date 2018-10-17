CC = gcc
CFLAGS = -W -Wall -ansi -g -std=c99
LDFLAGS = `sdl2-config --cflags --libs` -lSDL2_ttf

all: game
game: main.o snake.o queue.o file.o headers.h
	$(CC)  main.o snake.o queue.o file.o $(LDFLAGS) -o game
main.o: main.c headers.h
	$(CC) -c main.c headers.h
snake.o: snake.c headers.h
	$(CC) -c snake.c
queue.o: queue.c headers.h
	$(CC) -c queue.c
file.o: file.c
	$(CC) -c file.c

clean:
	rm -rf *.o

mrproper: clean
	rm -rf game
