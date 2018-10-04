#include "headers.h"
#include "file.c"
#include "snake.c"

int main(int argc, char *argv[]){

  if (SDL_Init(SDL_INIT_VIDEO) != 0){   //initialisation de la SDL
    printf("SDL Initialisation failed ! : %s", SDL_GetError());
    SDL_Quit();
    return EXIT_FAILURE;
  }


  srand(time(0));

  bool playing = true;
  snake head;
  file *body = initialize();

  SDL_Rect grid[NBX][NBY];

  int i, j;
  for (i = 0; i < NBX; i++){
    for (j = 0; j < NBY; j++){
      gridBool[i][j] = false;
      setRect(&grid[i][j], i * SNAKE_WIDTH, j * SNAKE_HEIGHT, SNAKE_WIDTH, SNAKE_HEIGHT);
    }
  }
  gridBool[NBX/2][NBY/2] = true;

  setRect(&head.snakeRect, grid[NBX/2][NBY/2].x, grid[NBX/2][NBY/2].y, SNAKE_WIDTH, SNAKE_HEIGHT);

  fileIn(body, head.snakeRect);
                  /* left,   right,    up,     down    { axe.dx, axe.dy } */
  dir direction = { {-1, 0}, {1, 0}, {0, -1}, {0, 1} };
  axe currentDir = direction.right;

  SDL_Window *window = SDL_CreateWindow("Game", SDL_WINDOWPOS_CENTERED,
           SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
  if (window == NULL){
    printf("Window creation failed ! : %s", SDL_GetError());
    SDL_Quit();
    return EXIT_FAILURE;
  }
  SDL_Renderer *screen  = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  SDL_Surface *image = SDL_LoadBMP("rect.bmp");
  SDL_Surface *image0 = SDL_LoadBMP("food.bmp");
  SDL_Texture *texture = SDL_CreateTextureFromSurface(screen, image);
  SDL_Texture *texture0 = SDL_CreateTextureFromSurface(screen, image0);
  SDL_Rect food = randFood(body);
  SDL_Event event;
  const Uint8 *keyboardState = SDL_GetKeyboardState(NULL);

  /* main loop */
  while(playing){

    putSnake(body, &head, screen, texture, texture0, &food, grid);
    handleKeys(keyboardState, &currentDir, &direction);
    move(&currentDir, &head, grid, body, &food);

    while(SDL_PollEvent(&event)){
      if (event.type == SDL_QUIT || keyboardState[SDL_SCANCODE_ESCAPE]){
        playing = false;
      }
    }
  }

    //Quitter SDL
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}
