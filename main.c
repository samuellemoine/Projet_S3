#include "headers.h"
#include "file.c"
#include "snake.c"

int main(int argc, char *argv[]){

  if (SDL_Init(SDL_INIT_VIDEO) != 0){   //initialisation de la SDL
    printf("SDL Initialisation failed ! : %s", SDL_GetError());
    SDL_Quit();
    return EXIT_FAILURE;
  }
  SDL_Window *window = SDL_CreateWindow("Snake", SDL_WINDOWPOS_CENTERED,
           SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
  if (window == NULL){
    printf("Window creation failed ! : %s", SDL_GetError());
    SDL_Quit();
    return EXIT_FAILURE;
  }

  const Uint8 *keyboardState = SDL_GetKeyboardState(NULL);  /* pointer to an array of key states */
  srand(time(0));   /* initiate the Pseudo-Random Number Generator */

  bool playing = true;    /* controls main loop */
  bool gameover = false; /* stops the snake from moving on on collision */
  bool dirChanged = false;
  bool pause = false;

  SDL_Rect grid[NBX][NBY];   /* fixed positions of the snake */
  for (int i = 0; i < NBX; i++){
    for (int j = 0; j < NBY; j++){
      setRect(&grid[i][j], i * SNAKE_WIDTH, j * SNAKE_HEIGHT, SNAKE_WIDTH, SNAKE_HEIGHT);
    }
  }

  snake head;               /* head.dir and head.snakeRect initiate the movement */
  setRect(&head.snakeRect, grid[NBX/2][NBY/2].x, grid[NBX/2][NBY/2].y, SNAKE_WIDTH, SNAKE_HEIGHT);

  file *body = initialize();  /* file of SDL_Rect contains grid coordinates
                                            making the body follow the head */
  fileIn(body, head.snakeRect);

  /* { axe.dx, axe.dy }   left,   right,    up,     down */
  dir direction     = { {-1, 0}, {1, 0}, {0, -1}, {0, 1} };
  axe currentDir = direction.right; /* initial direction is right */


  SDL_Renderer *screen  = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  SDL_Surface *image = SDL_LoadBMP("rect.bmp");
  SDL_Surface *image0 = SDL_LoadBMP("food.bmp");
  SDL_Texture *texture = SDL_CreateTextureFromSurface(screen, image);
  SDL_Texture *texture0 = SDL_CreateTextureFromSurface(screen, image0);
  SDL_Rect food = randFood(body);
  SDL_Event event;
  SDL_SetWindowIcon(window, image);


  /* main loop */
  while(playing){
    if (!gameover){
      handleKeys(keyboardState, &currentDir, &direction, &dirChanged, &pause, &event);
      if (!pause){
        move(&currentDir, &head, grid, body, &food, &gameover, &dirChanged);
        drawSnake(body, &head, screen, texture, texture0, &food, grid);
      }
    }

    /* quit the game */
    while(SDL_PollEvent(&event)){
      if (event.type == SDL_QUIT || keyboardState[SDL_SCANCODE_ESCAPE]){
        playing = false;
      }
    }
  }

    //Quitter SDL
    SDL_DestroyRenderer(screen);
    SDL_FreeSurface(image); SDL_FreeSurface(image0);
    SDL_DestroyTexture(texture); SDL_DestroyTexture(texture0);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
