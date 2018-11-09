CC = gcc
CFLAGS = -W -Wextra -Werror -Wall -g -Og -std=c99
LDFLAGS = `sdl2-config --cflags --libs` -lSDL2_ttf

all: game
game: main.o core.o snake.o queue.o file.o tab.o headers.h
	$(CC) $(CFLAGS) main.o core.o snake.o queue.o file.o tab.o $(LDFLAGS) -o game
main.o: main.c headers.h
	$(CC) $(CFLAGS) -c main.c headers.h
core.o: core.c headers.h
		$(CC) $(CFLAGS) -c core.c headers.h
snake.o: snake.c headers.h
	$(CC) $(CFLAGS) -c snake.c
queue.o: queue.c headers.h
	$(CC) $(CFLAGS) -c queue.c
file.o: file.c
	$(CC) $(CFLAGS) -c file.c
tab.o: tab.c
	$(CC) $(CFLAGS) -c tab.c

clean:
	rm -rf *.o

mrproper: clean
	rm -rf game
	rm -rf highscores.txt
