#include "headers.h"

int main(void){
  SDL_Window* window = NULL;
  SDL_Renderer* screen = NULL;
  TTF_Font* font = NULL;
  const Uint8* keyboardState = NULL;
  SDL_Rect** grid = allocate_Rect2D(NBY, NBX);   /* fixed positions of the snake */
  if (init(window, &screen, &font, &keyboardState, grid) == EXIT_FAILURE){
    return EXIT_FAILURE;
  }
  SDL_SetWindowIcon(window, NULL);

  SDL_Event event;

  /* { axe.dx, axe.dy }   left,   right,    up,     down */
  dir direction     = { {-1, 0}, {1, 0}, {0, -1}, {0, 1} };
  snake head;               /* head.dir and head.snakeRect initiate the movement */
  SDL_Rect food;            /* contains the food coordinates */
  SDL_Rect bonusFood;
  queue* body = initialize();    /* queue of SDL_Rect contains grid coordinates making the body follow the head */
  queueIn(body, &head.snakeRect, NULL);
  queue* mazeq = initialize();   /* queue of SDL_Rect for grid coordinates of the maze */

  int choice = 1;           /* switch between maze and speed selection in menu */
  int level = 0;            /* level in menu */
  int mazeSelector = 0;     /* mode in menu */
  bool keyPressed = false;  /* handle the cursor and keys in menu */

  int score;
  bool playing = true;      /* controls main loop */
  bool started;             /* controls the menu at start */
  bool pause;               /* pauses the game */
  bool gameover;            /* stops the snake from moving on on collision */
  bool dirChanged;          /* prevents the player from doing a complete turn back with the snake */
  reset(&started, &pause, &gameover, &dirChanged, &score, &head, body, mazeq, &food, &bonusFood, grid); /* set the initial variables */

  SDL_Texture* gameTexture = loadBMPSurface(screen, "surf.bmp");
  SDL_Texture* menuTexture = loadBMPSurface(screen, "menu.bmp");

  /* control of color variations speed */
  double dc = 1.0 / 32.0;
  fadeColor* color = malloc(sizeof(fadeColor));
  color[0].rgb = 49.0 + rand()%(61); color[0].variation = -dc;
  SDL_SetRenderDrawColor(screen, 140 - (int) color[0].rgb, 140 - (int) color[0].rgb, 140 - (int) color[0].rgb, 255);  /* background color */

  int bonusSize = 0;

  /* control of drawScreen() framerate */
  unsigned long milliseconds, end;
  double fps = 0;
  int frames = 0;
  int frameNb = 0;
  unsigned long lastBonus;
  bool smoothTailFrame = true;
  SDL_Rect lastTail = (SDL_Rect) {0, 0, SNAKE_WIDTH, SNAKE_HEIGHT};
  /* main loop */
  while(playing){
    if (frameNb >= frames) frameNb = 0;
    milliseconds = clock() * 1000 / CLOCKS_PER_SEC;

    if (started){
      if (frameNb == frames - 1) drawScreen(body, mazeq, &head, screen, gameTexture, &food, &bonusFood, mazeSelector, score, pause, gameover, head.dir.dx == 0 && head.dir.dy == 0, color, font, grid, &smoothTailFrame, &lastTail, &bonusSize);
      if (!gameover){
        handleKeys(keyboardState, &head, &direction, &pause, &dirChanged);
        if (!pause){
          move(&head, grid, body, mazeq, &food, &bonusFood, &gameover, &dirChanged, level, &score, &bonusSize, &lastBonus);
        }
      }
    }
    while (SDL_PollEvent(&event)){
      if (gameover){      /* reset to initial variables when game is over */
        if (event.type == SDL_KEYUP && event.key.keysym.sym == SDLK_RETURN){
          reset(&started, &pause, &gameover, &dirChanged, &score, &head, body, mazeq, &food, &bonusFood, grid);
        }
      }
      if (!started){      /* menu selector */
        handleMenu(mazeq, &started, &food, &event, screen, menuTexture, font, color, &choice, &level, &mazeSelector, &keyPressed, grid);
      }
      /* quit the game */
      if (event.type == SDL_QUIT || keyboardState[SDL_SCANCODE_ESCAPE]){
        playing = false;
      }
    }

    if (started){
      end = milliseconds + ADJUST_LEVEL - level - 1;
      fps = (double)(1.0 / ((end - milliseconds)/1000.0));
      frames = (int) (fps / 35);
      milliseconds = clock() * 1000 / CLOCKS_PER_SEC;
      if ((double) milliseconds / 1000 - (double) end / 1000 > 0.05) printf("Slow execution... Took %lf seconds too long.\n", (double) milliseconds/1000 - (double) end / 1000);
      while (milliseconds < end) milliseconds = clock() * 1000 / CLOCKS_PER_SEC;
      frameNb += 1;
    }
  }
  clean(screen, menuTexture, window, grid, body, mazeq, font, color);

  return EXIT_SUCCESS;
}
