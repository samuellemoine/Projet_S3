#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>
#include <time.h>



#define SCREEN_WIDTH      500
#define SCREEN_HEIGHT     500
#define SNAKE_WIDTH       25
#define SNAKE_HEIGHT      25
#define FOOD_WIDTH        8
#define FOOD_HEIGHT       8
#define VELOCITY          1
#define ADJUST_LEVEL      10
#define NBX               SCREEN_WIDTH / SNAKE_WIDTH
#define NBY               SCREEN_HEIGHT / SNAKE_HEIGHT

typedef struct Element Element;
struct Element
{
    SDL_Rect pos;
    Element *nextPos;
};


typedef struct File file;
struct File
{
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

/* function and method profiles for file.c */
file* initialize();
int fileSize(file *);
void fileIn(file *, SDL_Rect *);
void fileOut(file *);
SDL_Rect fileTail(file *);
SDL_Rect fileNb(file *, int);
SDL_Rect fileHead(file *);
void drawSnake(file *, snake *, SDL_Renderer *, SDL_Texture *, SDL_Texture *, SDL_Rect *, SDL_Rect [NBX][NBY]);


/* function and method profiles for snake.c */
void setRect(SDL_Rect *, int, int, int, int);
void setDir(axe *, axe *);
void reset(bool *, bool *, bool *, bool *, snake *, file *, SDL_Rect *, SDL_Rect [NBX][NBY]);
void handleMenu(bool *, SDL_Event *, SDL_Renderer *, SDL_Surface *[], SDL_Rect *, int *);
void handleKeys(const Uint8 *, snake *, dir *, bool *, bool *);
int indice(int, int);
bool validRand(file *, int, int);
SDL_Rect randFood(file *);
bool isSameDir(axe *, axe *);
bool snakeContact(file *);
bool foodContact(SDL_Rect *, SDL_Rect *);
void move(snake *, SDL_Rect [NBX][NBY], file *, SDL_Rect *, bool *, bool *, int *);
