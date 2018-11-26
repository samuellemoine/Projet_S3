#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>

#define SNAKE_WIDTH       30
#define SNAKE_HEIGHT      SNAKE_WIDTH
#define SCREEN_WIDTH      SNAKE_WIDTH * 25
#define SCREEN_HEIGHT     SNAKE_HEIGHT * 21
#define NBX               SCREEN_WIDTH / SNAKE_WIDTH
#define NBY               SCREEN_HEIGHT / SNAKE_HEIGHT
#define TOP_BAR           SNAKE_WIDTH * 3
#define ADJUST_LEVEL      8

#define VELOCITY          2

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
typedef struct Element Element;
struct Element {
    snake pos;
    Element* nextPos;
};

typedef struct Queue queue;
struct Queue {
    Element* head;
};



typedef struct Coord coord;
struct Coord {
  int x;
  int y;
};

struct FadeColor{
  double rgb;
  double variation;
};
typedef struct FadeColor fadeColor;
/* function and method profiles for core.c */
int init(SDL_Window *, SDL_Renderer **, TTF_Font **, const Uint8 **, SDL_Rect **);
void reset(bool *, bool *, bool *, bool *, int *, snake *, queue *, queue *, SDL_Rect *, SDL_Rect **);
void handleMenu(queue *, bool *, SDL_Rect*, SDL_Event *, SDL_Renderer *, SDL_Texture *, int *, int *, int *, SDL_Rect **);
void drawMenu(SDL_Renderer *, SDL_Texture *, int, int, int);
void handleKeys(const Uint8 *, snake *, dir *, bool *, bool *);
void move(snake *, SDL_Rect **, queue *, queue *, SDL_Rect *, bool *, bool *, int, int *);
void drawScreen(queue *, queue *, snake *, SDL_Renderer *, SDL_Texture *,  SDL_Rect *, int, int, bool, bool, bool, fadeColor*, TTF_Font*, SDL_Rect**, bool *, SDL_Rect *);
void clean(SDL_Renderer *, SDL_Texture *, SDL_Window *, SDL_Rect **, queue *, queue *, TTF_Font *, fadeColor *);

/* function and method profiles for snake.c */
void colorVariation(fadeColor *);
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
SDL_Rect smoothHead(snake *, SDL_Rect **);
SDL_Rect smoothTail(snake *, queue *, SDL_Rect *, int);
void timeout(int);
int abs(int);

/* function and method profiles for queue.c */
queue* initialize();
void queueIn(queue *, SDL_Rect *, axe *);
void queueOut(queue *);
SDL_Rect queueBack(queue *);
SDL_Rect queueFront(queue *);
axe tailDir(queue *);
bool tailInNode(queue *, snake *);
bool inNode(Element *);
int queueSize(queue *);


/* function and method profiles for file.c */
SDL_Texture* loadTTFTexture(SDL_Renderer *, TTF_Font *, SDL_Color *, char *);
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
