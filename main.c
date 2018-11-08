#include "headers.h"


void loadSurfacesBMP(SDL_Surface* levelSurfaces[], SDL_Surface* mazeSurfaces[]){
  /* load all images needed for the game */
  int i = 0;
  char* path;
  for (i = 0; i < 8; i++){
    path = NULL;
    sprintf(path, "sprites/menu/maze%d.bmp", i+1);
    mazeSurfaces[i] = SDL_LoadBMP(path);
    if (mazeSurfaces[i] == NULL){
      printf("Could not load %s : %s", path, SDL_GetError());
    }
  }
  for (i = 0; i < 10; i++){
    path = NULL;
    sprintf(path, "sprites/menu/maze%d.bmp", i+1);
    levelSurfaces[i] = SDL_LoadBMP(path);
    if (levelSurfaces[i] == NULL){
      printf("Could not load %s : %s", path, SDL_GetError());
    }
  }
}

int main(void){

  if (SDL_Init(SDL_INIT_VIDEO) != 0 || TTF_Init() != 0){
    printf("SDL Initialisation failed ! : %s", SDL_GetError());
    SDL_Quit();
    return EXIT_FAILURE;
  }

  SDL_Window *window = SDL_CreateWindow("Snake", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, TOP_BAR + SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
  SDL_Renderer *screen  = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  if (window == NULL || screen == NULL){
    printf("Window creation failed ! : %s", SDL_GetError());
    SDL_Quit();
    return EXIT_FAILURE;
  }

  SDL_SetRenderDrawColor(screen, 54, 54, 54, 255);  /* background color */

  TTF_Font *font = TTF_OpenFont("font.ttf", 17);
  if (!font){
    printf("Error while opening font: %s\n", TTF_GetError());
  }

  srand(time(0));             /* initiate the Pseudo-Random Number Generator */
  const Uint8 *keyboardState = SDL_GetKeyboardState(NULL);  /* pointer to an array of key states */
  SDL_Event event;


  /* { axe.dx, axe.dy }   left,   right,    up,     down */
  dir direction     = { {-1, 0}, {1, 0}, {0, -1}, {0, 1} };
  SDL_Rect** grid = allocate_Rect2D(NBX, NBY);   /* fixed positions of the snake */
  int i; int j;
  for (i = 0; i < NBX; i++){
    for (j = 0; j < NBY; j++){
      setRect(&grid[i][j], i * SNAKE_WIDTH, TOP_BAR + j * SNAKE_HEIGHT, SNAKE_WIDTH, SNAKE_HEIGHT);
    }
  }


  snake head;               /* head.dir and head.snakeRect initiate the movement */
  SDL_Rect food;

  queue *body = initialize();  /* queue of SDL_Rect contains grid coordinates making the body follow the head */
  queueIn(body, &head.snakeRect);
  queue *mazeq = initialize();  /* queue of SDL_Rect contains grid coordinates for the maze */

  int choice = 1;  /* switch between maze and speed selection in menu */
  bool playing = true;      /* controls main loop */
  bool started;             /* controls the menu at start */
  bool pause;               /* pauses the game */
  bool gameover;            /* stops the snake from moving on on collision */
  bool dirChanged;

  int score;
  int level = 0;
  int mazeSelector = 0;
  reset(&started, &pause, &gameover, &dirChanged, &score, &head, body, mazeq, &food, grid); /* set the initial variables */


  SDL_Surface *topbarSurface = SDL_LoadBMP("sprites/topbar.bmp");

  SDL_Surface *gameSurface = SDL_LoadBMP("sprites.bmp");

  SDL_Texture *topbarTexture = SDL_CreateTextureFromSurface(screen, topbarSurface);
  SDL_Texture *gameTexture = SDL_CreateTextureFromSurface(screen, gameSurface);

  SDL_SetWindowIcon(window, gameSurface);

  /* main loop */
  while(playing){
    if (!gameover && started){
      handleKeys(&event, keyboardState, &head, &direction, &pause, &dirChanged);
      drawSnake(body, mazeq, &head, screen, gameTexture, topbarTexture, &food, mazeSelector, score, font, grid);
      if (!pause){
        move(&head, grid, body, mazeq, &food, &gameover, &dirChanged, level, &score);
      }
    }


    while(SDL_PollEvent(&event)){
      /* reset to initial variables when game is over */
      if (gameover){
        if (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_RETURN){
          reset(&started, &pause, &gameover, &dirChanged, &score, &head, body, mazeq, &food, grid);
        }
      }
      /* menu selector */
      if (!started){
        handleMenu(mazeq, &started, &event, screen, gameTexture, &choice, &level, &mazeSelector, grid);
        /* food position needs to be called after the maze map has been loaded */
        food = randFood(body, mazeq);
      }
      /* quit the game */
      if (event.type == SDL_QUIT || keyboardState[SDL_SCANCODE_ESCAPE]){
        playing = false;
      }
    }

  }

    /* clean up */
    SDL_DestroyRenderer(screen);
    SDL_FreeSurface(gameSurface);
    SDL_FreeSurface(topbarSurface);
    SDL_DestroyTexture(topbarTexture);
    SDL_DestroyTexture(gameTexture);
    SDL_DestroyWindow(window);
    free_Rect2D(grid, NBX);
    TTF_CloseFont(font);

    while (body->head != NULL){
      queueOut(body);
    }
    while (mazeq->head != NULL){
      queueOut(mazeq);
    }

    SDL_Quit();
    return 0;
}
