#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

#define NBX               20
#define NBY               NBX
#define SCREEN_WIDTH      500
#define SCREEN_HEIGHT     SCREEN_WIDTH
#define SNAKE_WIDTH       SCREEN_WIDTH / NBX
#define SNAKE_HEIGHT      SCREEN_HEIGHT / NBY
#define TOP_BAR           SNAKE_WIDTH * 3

#define VELOCITY          1
#define ADJUST_LEVEL      5

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

/* function and method profiles for core.c */
int init(SDL_Window **, SDL_Renderer **r, TTF_Font **, const Uint8 **);
void reset(bool *, bool *, bool *, bool *, int *, snake *, queue *, queue *, SDL_Rect *, SDL_Rect **);
void handleMenu(queue *, bool *, SDL_Event *, SDL_Renderer *, SDL_Texture *, int *, int *, int *, SDL_Rect **);
void handleKeys(const Uint8 *, snake *, dir *, bool *, bool *);
void move(snake *, SDL_Rect **, queue *, queue *, SDL_Rect *, bool *, bool *, int, int *);
void drawScreen(queue *, queue *, snake *, SDL_Renderer *, SDL_Texture *, SDL_Texture *, SDL_Rect *, int, int, bool, bool, bool, TTF_Font*, SDL_Rect**);
void clean(SDL_Renderer *, SDL_Texture *, SDL_Texture *, SDL_Window *, SDL_Rect **, queue *, queue *, TTF_Font *);

/* function and method profiles for snake.c */
void setRect(SDL_Rect *, int, int, int, int);
void setDir(axe *, axe *);
int indice(int, int);
bool foodContact(SDL_Rect *, SDL_Rect *);
bool snakeContact(queue *);
bool mazeContact(snake *, queue *);
SDL_Rect randFood(queue *, queue *);
bool validRand(queue *, queue *, int, int);
void randMaze(int, queue *, SDL_Rect**);
bool validMaze(int, int);
void timeout(int);

/* function and method profiles for queue.c */
queue* initialize();
int queueSize(queue *);
void queueIn(queue *, SDL_Rect *);
void queueOut(queue *);
SDL_Rect queueBack(queue *);
SDL_Rect queueTail(queue *);

/* function and method profiles for file.c */
SDL_Texture* loadBMPSurface(SDL_Renderer *, char *);
void readMazeFile(char*, char**);
void putInMaze(queue *, SDL_Rect **, char **);
void readScoreFile(char *, char **);
void writeScoreFile(char*, char **, int, int);

/* function and method profiles for tab.c */
SDL_Rect** allocate_Rect2D(int, int);
void free_Rect2D(SDL_Rect **, int);
char** allocate_Char2D(int, int);
void free_Char2D(char **, int);
int** allocate_Int2D(int, int);
void free_Int2D(int **, int);
