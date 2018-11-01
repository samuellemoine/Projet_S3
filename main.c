#include "headers.h"

int main(int argc, char *argv[]){

  if (SDL_Init(SDL_INIT_VIDEO) != 0 || TTF_Init() != 0){   //initialisation de la SDL
    printf("SDL Initialisation failed ! : %s", SDL_GetError());
    SDL_Quit();
    return EXIT_FAILURE;
  }

  SDL_Window *window = SDL_CreateWindow("Snake", SDL_WINDOWPOS_CENTERED,
           SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, TOP_BAR + SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
           /* create the renderer */
  SDL_Renderer *screen  = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  TTF_Font *font = TTF_OpenFont("old_school_united_regular.ttf", TOP_BAR);
  if (!font){
    printf("Error while opening font: %s\n", TTF_GetError());
  }
  if (window == NULL || screen == NULL){
    printf("Window creation failed ! : %s", SDL_GetError());
    SDL_Quit();
    return EXIT_FAILURE;
  }

  /* { axe.dx, axe.dy }   left,   right,    up,     down */
  dir direction     = { {-1, 0}, {1, 0}, {0, -1}, {0, 1} };
  SDL_Rect** grid = allocate_Rect2D(NBX, NBY);   /* fixed positions of the snake */
  int i; int j;
  for (i = 0; i < NBX; i++){
    for (j = 0; j < NBY; j++){
      setRect(&grid[i][j], i * SNAKE_WIDTH, TOP_BAR + j * SNAKE_HEIGHT, SNAKE_WIDTH, SNAKE_HEIGHT);
    }
  }


  const Uint8 *keyboardState = SDL_GetKeyboardState(NULL);  /* pointer to an array of key states */

  srand(time(0));             /* initiate the Pseudo-Random Number Generator */


  snake head;               /* head.dir and head.snakeRect initiate the movement */
  SDL_Rect food;


  queue *body = initialize();  /* file of SDL_Rect contains grid coordinates
                                            making the body follow the head */
  queueIn(body, &head.snakeRect);

  bool playing = true;    /* controls main loop */
  bool started; /* controls the menu at start */
  bool pause;   /* pauses the game */
  bool gameover; /* stops the snake from moving on on collision */
  bool dirChanged;

  int score;
  SDL_Event event;
  queue *mazeq = initialize();
  reset(&started, &pause, &gameover, &dirChanged, &score, &head, body, mazeq, &food, grid);

  int level = 5;
  int mazeSelector = 0;




  /* load all images needed for the game */
  SDL_Surface *snakeSurface = SDL_LoadBMP("rect.bmp");
  SDL_Surface *wallSurface = SDL_LoadBMP("wall.bmp");
  SDL_Surface *foodSurface = SDL_LoadBMP("food.bmp");
  SDL_Surface *levelSurface1 = SDL_LoadBMP("level1.bmp");
  SDL_Surface *levelSurface2 = SDL_LoadBMP("level2.bmp");
  SDL_Surface *levelSurface3 = SDL_LoadBMP("level3.bmp");
  SDL_Surface *levelSurface4 = SDL_LoadBMP("level4.bmp");
  SDL_Surface *levelSurface5 = SDL_LoadBMP("level5.bmp");
  SDL_Surface *levelSurface6 = SDL_LoadBMP("level6.bmp");
  SDL_Surface *levelSurface7 = SDL_LoadBMP("level7.bmp");
  SDL_Surface *levelSurface8 = SDL_LoadBMP("level8.bmp");
  SDL_Surface *levelSurface9 = SDL_LoadBMP("level9.bmp");
  SDL_Surface *levelSurface10 = SDL_LoadBMP("level10.bmp");

  SDL_Surface *mazeSurface1 = SDL_LoadBMP("maze1.bmp");
  SDL_Surface *mazeSurface2 = SDL_LoadBMP("maze2.bmp");
  SDL_Surface *mazeSurface3 = SDL_LoadBMP("maze3.bmp");
  SDL_Surface *mazeSurface4 = SDL_LoadBMP("maze4.bmp");

  /* arrays containing image surfaces of the level and maze selctor */
  SDL_Surface *levelSurfaces[10] = {levelSurface1, levelSurface2, levelSurface3, levelSurface4, levelSurface5, levelSurface6, levelSurface7, levelSurface8, levelSurface9, levelSurface10};
  SDL_Surface *mazeSurfaces[4] = {mazeSurface1, mazeSurface2, mazeSurface3, mazeSurface4};

  SDL_Texture *snakeTexture = SDL_CreateTextureFromSurface(screen, snakeSurface);
  SDL_Texture *wallTexture = SDL_CreateTextureFromSurface(screen, wallSurface);
  SDL_Texture *foodTexture = SDL_CreateTextureFromSurface(screen, foodSurface);

  SDL_SetWindowIcon(window, snakeSurface);

  /* main loop */
  while(playing){
    if (!gameover && started){
      handleKeys(keyboardState, &head, &direction, &pause, &dirChanged);
      drawSnake(body, mazeq, &head, screen, snakeTexture, wallTexture, foodTexture, &food, level, mazeSelector, score, font, grid);
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
        handleMenu(mazeq, &started, &event, screen, levelSurfaces, mazeSurfaces, &level, &mazeSelector, grid);
      }
      /* quit the game */
      if (event.type == SDL_QUIT || keyboardState[SDL_SCANCODE_ESCAPE]){
        playing = false;
      }
    }

  }

    //Quitter SDL
    SDL_DestroyRenderer(screen);
    SDL_FreeSurface(snakeSurface);
    SDL_FreeSurface(wallSurface);
    SDL_FreeSurface(foodSurface);
    SDL_FreeSurface(levelSurface1);
    SDL_FreeSurface(levelSurface2);
    SDL_FreeSurface(levelSurface3);
    SDL_FreeSurface(levelSurface4);
    SDL_FreeSurface(levelSurface5);
    SDL_FreeSurface(levelSurface6);
    SDL_FreeSurface(levelSurface7);
    SDL_FreeSurface(levelSurface8);
    SDL_FreeSurface(levelSurface9);
    SDL_FreeSurface(levelSurface10);
    SDL_FreeSurface(mazeSurface1);
    SDL_FreeSurface(mazeSurface2);
    SDL_FreeSurface(mazeSurface3);
    SDL_FreeSurface(mazeSurface4);
    SDL_DestroyTexture(snakeTexture);
    SDL_DestroyTexture(foodTexture);
    SDL_DestroyWindow(window);
    free_Rect2D(grid, NBY);

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
