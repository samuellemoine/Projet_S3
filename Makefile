CC = gcc
CFLAGS = -W -Wextra -Werror -Wall -g -Og -std=c99
LDFLAGS = `sdl2-config --cflags --libs` -lSDL2_ttf

all: game
game: main.o core.o snake.o queue.o file.o tab.o pixels.o headers.h
	$(CC) $(CFLAGS) main.o core.o snake.o queue.o file.o tab.o pixels.o $(LDFLAGS) -o game
main.o: main.c headers.h
	$(CC) $(CFLAGS) -c main.c
core.o: core.c headers.h
		$(CC) $(CFLAGS) -c core.c
snake.o: snake.c headers.h
	$(CC) $(CFLAGS) -c snake.c
queue.o: queue.c headers.h
	$(CC) $(CFLAGS) -c queue.c
file.o: file.c
	$(CC) $(CFLAGS) -c file.c
tab.o: tab.c
	$(CC) $(CFLAGS) -c tab.c
pixels.o: pixels.c
	$(CC) $(CFLAGS) -c pixels.c
clean:
	rm -rf *.o
	rm -rf *.txt

mrproper: clean
	rm -rf game
