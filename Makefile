main: main.c snake.c file.c
	gcc -Wall -g -std=c99 main.c `sdl2-config --cflags --libs` -o main -lm -lSDL2_ttf
clean:
	rm main
