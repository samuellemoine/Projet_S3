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


  const Uint8 *keyboardState = SDL_GetKeyboardState(NULL);  /* pointer to an array of key states */

  srand(time(0));             /* initiate the Pseudo-Random Number Generator */

  /* { axe.dx, axe.dy }   left,   right,    up,     down */
  dir direction     = { {-1, 0}, {1, 0}, {0, -1}, {0, 1} };
  SDL_Rect grid[NBX][NBY];   /* fixed positions of the snake */
  for (int i = 0; i < NBX; i++){
    for (int j = 0; j < NBY; j++){
      setRect(&grid[i][j], i * SNAKE_WIDTH, TOP_BAR + j * SNAKE_HEIGHT, SNAKE_WIDTH, SNAKE_HEIGHT);
    }
  }

  snake head;               /* head.dir and head.snakeRect initiate the movement */
  SDL_Rect food;


  file *body = initialize();  /* file of SDL_Rect contains grid coordinates
                                            making the body follow the head */
  fileIn(body, &head.snakeRect);

  bool playing = true;    /* controls main loop */

  bool started; /* controls the menu at start */
  bool pause;   /* pauses the game */
  bool gameover; /* stops the snake from moving on on collision */
  bool dirChanged;
  SDL_Event event;

  reset(&started, &pause, &gameover, &dirChanged, &head, body, &food, grid);

  int level = 5;


  /* level selector position */
  SDL_Rect levelRect = { (SCREEN_WIDTH - 400) / 2, (SCREEN_HEIGHT - 40) / 4, 400, 40 };

  /* load all images needed for the game */
  SDL_Surface *snakeSurface = SDL_LoadBMP("rect.bmp");
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

  /* an array containing images of the level selctor */
  SDL_Surface *s[10] = {levelSurface1, levelSurface2, levelSurface3, levelSurface4, levelSurface5, levelSurface6, levelSurface7, levelSurface8, levelSurface9, levelSurface10};

  SDL_Texture *snakeTexture = SDL_CreateTextureFromSurface(screen, snakeSurface);
  SDL_Texture *foodTexture = SDL_CreateTextureFromSurface(screen, foodSurface);

  SDL_SetWindowIcon(window, snakeSurface);

  /* main loop */
  while(playing){
    if (!gameover && started){
      handleKeys(keyboardState, &head, &direction, &pause, &dirChanged);
      drawSnake(body, &head, screen, snakeTexture, foodTexture, &food, level, font, grid);
      if (!pause){
        move(&head, grid, body, &food, &gameover, &dirChanged, &level);
      }
    }


    while(SDL_PollEvent(&event)){
      /* reset to initial variables when game is over */
      if (gameover){
        if (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_RETURN){
          reset(&started, &pause, &gameover, &dirChanged, &head, body, &food, grid);
        }
      }
      /* menu selector */
      if (!started){
        handleMenu(&started, &event, screen, s, &levelRect, &level);
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
    SDL_DestroyTexture(snakeTexture);
    SDL_DestroyTexture(foodTexture);
    SDL_DestroyWindow(window);

    TTF_CloseFont(font);
    SDL_Quit();
    return 0;
}
