#include "headers.h"

int main(int argc, char *argv[]){

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

  TTF_Font *font = TTF_OpenFont("font.ttf", TOP_BAR + 5);
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

  bool firstChoice = true;  /* switch between maze and speed selection in menu */
  bool playing = true;      /* controls main loop */
  bool started;             /* controls the menu at start */
  bool pause;               /* pauses the game */
  bool gameover;            /* stops the snake from moving on on collision */
  bool dirChanged;

  int score;
  int level = 0;
  int mazeSelector = 0;
  reset(&started, &pause, &gameover, &dirChanged, &score, &head, body, mazeq, &food, grid); /* set the initial variables */


  /* load all images needed for the game */
  SDL_Surface *snakeSurface = SDL_LoadBMP("sprites/game/rect.bmp");
  SDL_Surface *wallSurface = SDL_LoadBMP("sprites/game/wall.bmp");
  SDL_Surface *foodSurface = SDL_LoadBMP("sprites/game/food.bmp");
  SDL_Surface *topbarSurface = SDL_LoadBMP("sprites/game/topbar.bmp");

  SDL_Surface *levelSurface1 = SDL_LoadBMP("sprites/menu/level1.bmp");
  SDL_Surface *levelSurface2 = SDL_LoadBMP("sprites/menu/level2.bmp");
  SDL_Surface *levelSurface3 = SDL_LoadBMP("sprites/menu/level3.bmp");
  SDL_Surface *levelSurface4 = SDL_LoadBMP("sprites/menu/level4.bmp");
  SDL_Surface *levelSurface5 = SDL_LoadBMP("sprites/menu/level5.bmp");
  SDL_Surface *levelSurface1f = SDL_LoadBMP("sprites/menu/level1faded.bmp");
  SDL_Surface *levelSurface2f = SDL_LoadBMP("sprites/menu/level2faded.bmp");
  SDL_Surface *levelSurface3f = SDL_LoadBMP("sprites/menu/level3faded.bmp");
  SDL_Surface *levelSurface4f = SDL_LoadBMP("sprites/menu/level4faded.bmp");
  SDL_Surface *levelSurface5f = SDL_LoadBMP("sprites/menu/level5faded.bmp");


  SDL_Surface *mazeSurface1 = SDL_LoadBMP("sprites/menu/maze1.bmp");
  SDL_Surface *mazeSurface2 = SDL_LoadBMP("sprites/menu/maze2.bmp");
  SDL_Surface *mazeSurface3 = SDL_LoadBMP("sprites/menu/maze3.bmp");
  SDL_Surface *mazeSurface4 = SDL_LoadBMP("sprites/menu/maze4.bmp");

  SDL_Surface *mazeSurface1f = SDL_LoadBMP("sprites/menu/maze1faded.bmp");
  SDL_Surface *mazeSurface2f = SDL_LoadBMP("sprites/menu/maze2faded.bmp");
  SDL_Surface *mazeSurface3f = SDL_LoadBMP("sprites/menu/maze3faded.bmp");
  SDL_Surface *mazeSurface4f = SDL_LoadBMP("sprites/menu/maze4faded.bmp");

  /* arrays containing image surfaces of the level and maze selctor */
  SDL_Surface *levelSurfaces[10] = {levelSurface1, levelSurface1f, levelSurface2, levelSurface2f, levelSurface3, levelSurface3f, levelSurface4, levelSurface4f, levelSurface5, levelSurface5f};
  SDL_Surface *mazeSurfaces[8] = {mazeSurface1, mazeSurface1f, mazeSurface2, mazeSurface2f, mazeSurface3, mazeSurface3f, mazeSurface4, mazeSurface4f};

  SDL_Texture *snakeTexture = SDL_CreateTextureFromSurface(screen, snakeSurface);
  SDL_Texture *wallTexture = SDL_CreateTextureFromSurface(screen, wallSurface);
  SDL_Texture *foodTexture = SDL_CreateTextureFromSurface(screen, foodSurface);
  SDL_Texture *topbarTexture = SDL_CreateTextureFromSurface(screen, topbarSurface);

  SDL_SetWindowIcon(window, snakeSurface);

  /* main loop */
  while(playing){
    if (!gameover && started){
      handleKeys(keyboardState, &head, &direction, &pause, &dirChanged);
      drawSnake(body, mazeq, &head, screen, snakeTexture, wallTexture, foodTexture, topbarTexture, &food, level, mazeSelector, score, font, grid);
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
        handleMenu(mazeq, &started, &event, screen, levelSurfaces, mazeSurfaces, &firstChoice, &level, &mazeSelector, grid);
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
    SDL_FreeSurface(snakeSurface);
    SDL_FreeSurface(wallSurface);
    SDL_FreeSurface(foodSurface);
    SDL_FreeSurface(levelSurface1);
    SDL_FreeSurface(levelSurface2);
    SDL_FreeSurface(levelSurface3);
    SDL_FreeSurface(levelSurface4);
    SDL_FreeSurface(levelSurface5);
    SDL_FreeSurface(levelSurface1f);
    SDL_FreeSurface(levelSurface2f);
    SDL_FreeSurface(levelSurface3f);
    SDL_FreeSurface(levelSurface4f);
    SDL_FreeSurface(levelSurface5f);
    SDL_FreeSurface(mazeSurface1);
    SDL_FreeSurface(mazeSurface2);
    SDL_FreeSurface(mazeSurface3);
    SDL_FreeSurface(mazeSurface4);
    SDL_FreeSurface(mazeSurface1f);
    SDL_FreeSurface(mazeSurface2f);
    SDL_FreeSurface(mazeSurface3f);
    SDL_FreeSurface(mazeSurface4f);
    SDL_FreeSurface(topbarSurface);
    SDL_DestroyTexture(topbarTexture);
    SDL_DestroyTexture(snakeTexture);
    SDL_DestroyTexture(foodTexture);
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
