#include "headers.h"

int main(void){

  SDL_Window* window = NULL;
  SDL_Renderer* screen = NULL;
  TTF_Font* font = NULL;
  const Uint8* keyboardState = NULL;
  SDL_Rect** grid = allocate_Rect2D(NBY, NBX);   /* fixed positions of the snake */
  if (init(&window, &screen, &font, &keyboardState, grid) == EXIT_FAILURE){
    return EXIT_FAILURE;
  }
  SDL_Event event;

  /* { axe.dx, axe.dy }   left,   right,    up,     down */
  dir direction     = { {-1, 0}, {1, 0}, {0, -1}, {0, 1} };

  snake head;               /* head.dir and head.snakeRect initiate the movement */
  SDL_Rect food;            /* contains the food coordinates */
  queue* body = initialize();    /* queue of SDL_Rect contains grid coordinates making the body follow the head */
  queueIn(body, &head.snakeRect);
  queue* mazeq = initialize();   /* queue of SDL_Rect for grid coordinates of the maze */

  int choice = 1;           /* switch between maze and speed selection in menu */
  int level = 0;            /* level in menu */
  int mazeSelector = 0;     /* mode in menu */
  int score;
  bool playing = true;      /* controls main loop */
  bool started;             /* controls the menu at start */
  bool pause;               /* pauses the game */
  bool gameover;            /* stops the snake from moving on on collision */
  bool dirChanged;          /* prevents the player from doing a complete turn back with the snake */
  reset(&started, &pause, &gameover, &dirChanged, &score, &head, body, mazeq, &food, grid); /* set the initial variables */
  SDL_Texture* topbarTexture = loadBMPSurface(screen, "topbar.bmp");
  SDL_Texture* gameTexture = loadBMPSurface(screen, "sprites.bmp");
  SDL_SetWindowIcon(window, NULL);

  /* main loop */
  while(playing){
    if (started){
      handleKeys(keyboardState, &head, &direction, &pause, &dirChanged);
      drawScreen(body, mazeq, &head, screen, gameTexture, topbarTexture, &food, mazeSelector, score, pause, gameover, head.dir.dx == 0 && head.dir.dy == 0, font, grid);
      if (!pause && !gameover){
        move(&head, grid, body, mazeq, &food, &gameover, &dirChanged, level, &score);
      }
    }
    while(SDL_PollEvent(&event)){
      if (gameover){      /* reset to initial variables when game is over */
        if (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_RETURN){
          reset(&started, &pause, &gameover, &dirChanged, &score, &head, body, mazeq, &food, grid);
        }
      }
      if (!started){      /* menu selector */
        handleMenu(mazeq, &started, &event, screen, gameTexture, &choice, &level, &mazeSelector, grid);
        food = randFood(body, mazeq);   /* food position randomizer needs to be called after the maze map has been loaded */
      }
      /* quit the game */
      if (event.type == SDL_QUIT || keyboardState[SDL_SCANCODE_ESCAPE]){
        playing = false;
      }
    }
  }
  clean(screen, topbarTexture, gameTexture, window, grid, body, mazeq, font);

  return EXIT_SUCCESS;
}
