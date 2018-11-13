#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

#define SCREEN_WIDTH      625
#define SCREEN_HEIGHT     500
#define SNAKE_WIDTH       25
#define SNAKE_HEIGHT      SNAKE_WIDTH
#define NBX               SCREEN_WIDTH / SNAKE_WIDTH
#define NBY               SCREEN_HEIGHT / SNAKE_HEIGHT
#define TOP_BAR           SNAKE_WIDTH * 3

#define VELOCITY          1
#define ADJUST_LEVEL      5

typedef struct Element Element;
struct Element {
    SDL_Rect pos;
    Element* nextPos;
};

typedef struct Queue queue;
struct Queue {
    Element* head;
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
int init(SDL_Window **, SDL_Renderer **r, TTF_Font **, const Uint8 **, SDL_Rect **);
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
bool validFood(queue *, queue *, int, int);
void randMaze(int, queue *, SDL_Rect **, char **, char *);
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
void readMazeFile(char *, queue *, SDL_Rect **, char **);
void writeMazeFile(char *, char **);
void putInMaze(queue *, SDL_Rect **, char **);
void readScoreFile(char *, int *);
void writeScoreFile(char*, int *, int, int);
int intLog(int, int);
char* formattedScore(int, bool);


/* function and method profiles for tab.c */
SDL_Rect** allocate_Rect2D(int, int);
void free_Rect2D(SDL_Rect **, int);
char** allocate_Char2D(int, int);
void free_Char2D(char **, int);
int** allocate_Int2D(int, int);
void free_Int2D(int **, int);
