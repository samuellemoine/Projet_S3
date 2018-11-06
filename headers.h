#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>


#define SCREEN_WIDTH      500
#define SCREEN_HEIGHT     500
#define SNAKE_WIDTH       25
#define SNAKE_HEIGHT      25
#define TOP_BAR           SNAKE_WIDTH * 3

#define VELOCITY          1
#define ADJUST_LEVEL      5
#define NBX               SCREEN_WIDTH / SNAKE_WIDTH
#define NBY               SCREEN_HEIGHT / SNAKE_HEIGHT

typedef struct Element Element;
struct Element {
    SDL_Rect pos;
    Element *nextPos;
};


typedef struct Queue queue;
struct Queue {
    Element *head;
};


typedef struct Axe axe;
struct Axe {
  int dx;
  int dy;
};


typedef struct Snake snake;
struct Snake {
  SDL_Rect snakeRect;
  axe dir;
};


typedef struct Dir dir;
struct Dir {
  axe left;
  axe right;
  axe up;
  axe down;
};

typedef struct Coord coord;
struct Coord {
  int x;
  int y;
};
/* function and method profiles for queue.c */
queue* initialize();
int queueSize(queue *);
void queueIn(queue *, SDL_Rect *);
void queueOut(queue *);
SDL_Rect queueBack(queue *);
SDL_Rect queueTail(queue *);
SDL_Rect randFood(queue *, queue *);
bool validRand(queue *, queue *, int, int);
void randMaze(int, queue *, SDL_Rect**);
bool validMaze(int, int);
bool snakeContact(queue *);
bool mazeContact(snake *, queue *);
void drawSnake(queue *, queue *, snake *, SDL_Renderer *, SDL_Texture *, SDL_Texture *, SDL_Texture *, SDL_Texture *, SDL_Rect *, int, int, int, TTF_Font*, SDL_Rect**);


/* function and method profiles for snake.c */
SDL_Rect** allocate_Rect2D(int, int);
void free_Rect2D(SDL_Rect**, int);
void timeout(int);
void setRect(SDL_Rect *, int, int, int, int);
void setDir(axe *, axe *);
void reset(bool *, bool *, bool *, bool *, int *, snake *, queue *, queue *, SDL_Rect *, SDL_Rect **);
void handleMenu(queue *, bool *, SDL_Event *, SDL_Renderer *, SDL_Surface *[], SDL_Surface *[], bool *, int *, int *, SDL_Rect **);
void handleKeys(const Uint8 *, snake *, dir *, bool *, bool *);
int indice(int, int);
bool foodContact(SDL_Rect *, SDL_Rect *);
void move(snake *, SDL_Rect **, queue *, queue *, SDL_Rect *, bool *, bool *, int, int *);

/* function and method profiles for file.c */
void readMazeFile(char*, char**);
void putInMaze(queue *, SDL_Rect **, char **);
void readScoreFile(char *, char **);
void writeScoreFile(char*, char **, int, int);

/* function and method profiles for tab.c */
SDL_Rect** allocate_Rect2D(int, int);
void free_Rect2D(SDL_Rect **, int);
char** allocate_Char2D(int, int);
void free_Char2D(char **, int);
