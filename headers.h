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
#define LEVEL             7
#define ADJUST_LEVEL      15
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

void setRect(SDL_Rect *, int, int, int, int);
int indice(int, int);
bool validRand(file *, int, int);
