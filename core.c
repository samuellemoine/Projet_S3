#include "headers.h"

int init(SDL_Window** w, SDL_Renderer** r, TTF_Font** f, const Uint8** keystate, SDL_Rect** grid){
  if (SDL_Init(SDL_INIT_VIDEO)){
    SDL_Log("SDL Initialisation failed ! : %s", SDL_GetError());
    SDL_Quit();
    return EXIT_FAILURE;
  }
  if (TTF_Init() != 0){
    printf("TTF Initialisation failed ! : %s", SDL_GetError());
    TTF_Quit();
    return EXIT_FAILURE;
  }
  *f = TTF_OpenFont("font.ttf", 150);
  if (*f == NULL){
    SDL_Log("Error while opening font: %s\n", TTF_GetError());
  }
  *w = SDL_CreateWindow("Snake", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, TOP_BAR + SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
  if (*w == NULL){
    printf("Window creation failed ! : %s", SDL_GetError());
    SDL_Quit();
    return EXIT_FAILURE;
  }
  *r = SDL_CreateRenderer(*w, -1, SDL_RENDERER_ACCELERATED);
  if (*r == NULL){
    SDL_Log("Renderer creation failed ! : %s", SDL_GetError());
    SDL_Quit();
    return EXIT_FAILURE;
  }
  SDL_SetRenderDrawColor(*r, 54, 54, 54, 255);  /* background color */
  *keystate = SDL_GetKeyboardState(NULL);  /* pointer to an array of key states */
  srand(time(0));             /* initiate the Pseudo-Random Number Generator */
  int i; int j;
  for (i = 0; i < NBY; i++){
    for (j = 0; j < NBX; j++){
      setRect(&grid[i][j], j * SNAKE_WIDTH, TOP_BAR + i * SNAKE_HEIGHT, SNAKE_WIDTH, SNAKE_HEIGHT);
    }
  }
  return EXIT_SUCCESS;
}

void reset(bool* started, bool* pause, bool* gameover, bool* dirChanged, int* score, snake* head, queue* body, queue* mazeq, SDL_Rect* food, SDL_Rect** grid){
    *started = false;
    *pause = true;
    *gameover = false;
    *dirChanged = false;
    *score = 0;
    axe ini = {0, 0};
    while (queueSize(body) > 0){
      queueOut(body);
    }
    while (queueSize(mazeq) >= 0){
      queueOut(mazeq);
    }
    setRect(&head->snakeRect, grid[NBY/2][NBX/2].x, grid[NBY/2][NBX/2].y, SNAKE_WIDTH, SNAKE_HEIGHT);
    setDir(&head->dir, &ini);
    queueIn(body, &head->snakeRect);
    queueOut(body);
    *food = randFood(body, mazeq);
}

void handleMenu(queue* mazeq, bool* started, SDL_Event* event, SDL_Renderer* screen, SDL_Texture* menuTexture, int* choice, int* level, int* maze, SDL_Rect** grid){
  char** lines = allocate_Char2D(NBY, NBX);
  if(event->type == SDL_KEYDOWN){
    switch (event->key.keysym.sym){
      case SDLK_LEFT:
        *choice = !*choice;
      break;
      case SDLK_RIGHT:
        *choice = !*choice;
      break;
      case SDLK_UP:
        if (*choice){
          if (*maze > 0)  *maze -= 1;
          else  *maze = 3;
        }
        else{
          if (*level > 0)  *level -= 1;
          else  *level = 4;
        }
        break;
      case SDLK_DOWN:
        if (*choice){
          if (*maze < 3)  *maze += 1;
          else  *maze = 0;
        }
        else{
          if (*level < 4)  *level += 1;
          else  *level = 0;
        }
      break;
      case SDLK_RETURN:
        *started = true;
        switch (*maze){
          case 1:
            readMazeFile("mazeWalls.txt", mazeq, grid, lines);
          break;
	        case 2:
            randMaze(15 + rand()%15, mazeq, grid, lines, "lastRandom.txt");
	        break;
          case 3:
            randMaze(33 + rand()%17, mazeq, grid, lines, "lastHardRandom.txt");
          break;
        }
      break;
    }
  }
  free_Char2D(lines, NBY);

  SDL_Rect leftRect = (SDL_Rect) { 4 * SCREEN_WIDTH / 25, 6 * SCREEN_HEIGHT / 25, 8 * SCREEN_WIDTH / 25, 17 * SCREEN_HEIGHT / 25 };
  SDL_Rect rightRect = (SDL_Rect) { 13 * SCREEN_WIDTH / 25, 6 * SCREEN_HEIGHT / 25, 8 * SCREEN_WIDTH / 25, 17 * SCREEN_HEIGHT / 25 };
  SDL_Rect mazeSprite = (SDL_Rect) { (*maze * 2 + !*choice) * 160, 340, 160, 340 };
  SDL_Rect levelSprite = (SDL_Rect) { (*level * 2 + *choice) * 160, 0, 160, 340 };

  SDL_RenderClear(screen);
  SDL_RenderCopy(screen, menuTexture, &mazeSprite, &leftRect);
  SDL_RenderCopy(screen, menuTexture, &levelSprite, &rightRect);
  SDL_RenderPresent(screen);

}

void handleKeys(const Uint8* keyboardState, snake* head, dir* direction, bool* pause, bool* dirChanged){
  bool arrowPressed = keyboardState[SDL_SCANCODE_LEFT] || keyboardState[SDL_SCANCODE_RIGHT] || keyboardState[SDL_SCANCODE_UP] || keyboardState[SDL_SCANCODE_DOWN];
  if (arrowPressed  && *pause){
        *pause = false;
  }
  if (keyboardState[SDL_SCANCODE_SPACE]){
    *pause = !(*pause);
    timeout(250);
  }
  if (keyboardState[SDL_SCANCODE_LEFT] && head->dir.dx != 1 && !*dirChanged){
    setDir(&head->dir, &direction->left);
    *dirChanged = true;
  }
  if (keyboardState[SDL_SCANCODE_RIGHT] && head->dir.dx != -1 && !*dirChanged){
    setDir(&head->dir, &direction->right);
    *dirChanged = true;
  }
  if (keyboardState[SDL_SCANCODE_UP] && head->dir.dy != 1 && !*dirChanged){
    setDir(&head->dir, &direction->up);
    *dirChanged = true;
  }
  if (keyboardState[SDL_SCANCODE_DOWN] && head->dir.dy != -1 && !*dirChanged){
    setDir(&head->dir, &direction->down);
    *dirChanged = true;
  }
}

void move(snake* head, SDL_Rect** grid, queue* body, queue* mazeq, SDL_Rect* food, bool* gameover, bool* dirChanged, int level, int* score){
  timeout(ADJUST_LEVEL - level);

  bool headMoved, eats, isInLimits;
  int tmpPosX, tmpPosY;
  int xx = -1; int yy = -1;
  SDL_Rect currentHead = queueBack(body);  /* SDL_Rect of the snake's head */

  /* update temporary positons */
  tmpPosX = head->snakeRect.x;
  tmpPosY = head->snakeRect.y;
  tmpPosX += head->dir.dx * VELOCITY;
  tmpPosY += head->dir.dy * VELOCITY;

  /* make the snake fuse through walls */
  if (tmpPosX < 0)  head->snakeRect.x = SCREEN_WIDTH;
  else if (tmpPosX >= SCREEN_WIDTH - SNAKE_WIDTH)  head->snakeRect.x = 0;
  if (tmpPosY < TOP_BAR)  head->snakeRect.y = SCREEN_HEIGHT + TOP_BAR;
  else if (tmpPosY >= SCREEN_HEIGHT + TOP_BAR - SNAKE_HEIGHT)  head->snakeRect.y = TOP_BAR;

  /* apply temporary positions whe tmpPos within limits */
  if (tmpPosX >= 0 && tmpPosX < SCREEN_WIDTH)  head->snakeRect.x = tmpPosX;
  if (tmpPosY >= TOP_BAR && tmpPosY < SCREEN_HEIGHT + TOP_BAR)  head->snakeRect.y = tmpPosY;
  if (snakeContact(body) || mazeContact(head, mazeq)){
    *gameover = true;
    return;
  }
  /* determine position in the grid */
  xx = indice(head->snakeRect.x, SNAKE_WIDTH);
  yy = indice(head->snakeRect.y - TOP_BAR, SNAKE_HEIGHT);

  /* setting the three necessary booleans to move the snake */
  eats = foodContact(&head->snakeRect, food);
  headMoved = (indice(currentHead.x, SNAKE_WIDTH) != xx) || (indice(currentHead.y - TOP_BAR, SNAKE_HEIGHT) != yy);
  isInLimits = xx >= 0 && xx < NBX && yy >= 0 && yy < NBY;
  if (isInLimits && headMoved){
    *dirChanged = false;
    if (eats){
      queueIn(body, &grid[yy][xx]);
      *score +=  level + 1;
      *food = randFood(body, mazeq);
    }
    else{
      queueIn(body, &grid[yy][xx]);
      queueOut(body);
    }
  }
}

void drawScreen(queue *body, queue *mazeq, snake *head, SDL_Renderer *screen, SDL_Texture *gameTexture, SDL_Texture *topbarTexture, SDL_Rect *food, int mazeSelector, int score, bool pause, bool gameover, bool firstRound, TTF_Font *font, SDL_Rect** grid){
    if (body == NULL){
        exit(EXIT_FAILURE);
    }
    int* allScores = malloc(4 * sizeof(int));
    readScoreFile("highScores.txt", allScores);
    int highScore = allScores[mazeSelector];
    if (score > highScore){
      writeScoreFile("highScores.txt", allScores, mazeSelector, score);
    }
    free(allScores);
    char* highScoreChar = formattedScore(highScore, true);
    char* scoreChar = formattedScore(score, false);

    SDL_Color fontColor = (SDL_Color) {22, 22, 22, 255};
    SDL_Rect scoreRect = {SNAKE_WIDTH, SNAKE_HEIGHT / 2, SCREEN_WIDTH / 5 * 2, SCREEN_HEIGHT / 10};
    SDL_Rect highScoreRect = {SCREEN_WIDTH / 2 + SNAKE_WIDTH, SNAKE_HEIGHT / 2, SCREEN_WIDTH / 5 * 2, SCREEN_HEIGHT / 10};
    SDL_Surface* scoreSurface = TTF_RenderText_Solid(font, scoreChar, fontColor);
    SDL_Texture* scoreTexture = SDL_CreateTextureFromSurface(screen, scoreSurface);
    SDL_Surface* highScoreSurface = TTF_RenderText_Solid(font, highScoreChar, fontColor);
    SDL_Texture* highScoreTexture = SDL_CreateTextureFromSurface(screen, highScoreSurface);
    free(scoreChar);
    free(highScoreChar);

    /* this if - else if block smoothens the snake's movement */
    int i = 0;
    int j = 0;
    SDL_Rect smoothHead = {0, 0, SNAKE_WIDTH, SNAKE_HEIGHT};
    if (head->dir.dx == 0){
      i = indice(head->snakeRect.x, SNAKE_WIDTH);
      if (head->dir.dy == -1){
        j = - SNAKE_HEIGHT;
      }
      setRect(&smoothHead, grid[0][i].x, head->snakeRect.y + j, SNAKE_WIDTH, SNAKE_HEIGHT);
    }
    else if (head->dir.dy == 0){
      j = indice(head->snakeRect.y - TOP_BAR, SNAKE_HEIGHT);
      if (head->dir.dx == -1){
        i = - SNAKE_WIDTH;
      }
      setRect(&smoothHead, head->snakeRect.x + i, grid[j][0].y, SNAKE_WIDTH, SNAKE_HEIGHT);
    }

    SDL_Rect topbarRect = (SDL_Rect) { 0, 0, SCREEN_WIDTH, TOP_BAR };
    SDL_Rect snakeRect = (SDL_Rect) { 1280, 340, 30, 30 };
    SDL_Rect foodRect = (SDL_Rect) { 1340, 340, 30, 30 };
    SDL_Rect wallRect = (SDL_Rect) { 1310, 340, 30, 30 };

    SDL_RenderClear(screen);
    SDL_RenderCopy(screen, gameTexture, &foodRect, food);
    /* display the aligned head to smoothen the front movement*/
    if (!gameover) SDL_RenderCopy(screen, gameTexture, &snakeRect, &smoothHead);
    SDL_RenderCopy(screen, topbarTexture, &topbarRect, &topbarRect);
    SDL_RenderCopy(screen, scoreTexture, NULL, &scoreRect);
    SDL_RenderCopy(screen, highScoreTexture, NULL, &highScoreRect);

    /* display the snake blocks */
    Element* element = body->head;
    while (element != NULL){
      SDL_RenderCopy(screen, gameTexture, &snakeRect, &element->pos);
      element = element->nextPos;
    }
    /* display the maze blocks */
    element = mazeq->head;
    while (element != NULL){
      SDL_RenderCopy(screen, gameTexture, &wallRect, &element->pos);
      element = element->nextPos;
    }

    SDL_Surface* pauseSurface = NULL;
    SDL_Texture* pauseTexture = NULL;
    SDL_Surface* gameoverSurface = NULL;
    SDL_Texture* gameoverTexture = NULL;
    SDL_Rect messageRect = (SDL_Rect) { SNAKE_WIDTH / 6 * 5, SCREEN_HEIGHT / 2 - SCREEN_HEIGHT / 8 + TOP_BAR / 2, SCREEN_WIDTH - SNAKE_WIDTH, SCREEN_HEIGHT / 4 };
    if (!firstRound){
      if (gameover){
        gameoverSurface = TTF_RenderText_Solid(font, "Gameover", (SDL_Color) {45, 0, 0, 255});
        gameoverTexture = SDL_CreateTextureFromSurface(screen, gameoverSurface);
        SDL_RenderCopy(screen, gameoverTexture, NULL, &messageRect);
      }
      else if (pause){
        pauseSurface = TTF_RenderText_Solid(font, "Paused", (SDL_Color) {45, 0, 0, 255});
        pauseTexture = SDL_CreateTextureFromSurface(screen, pauseSurface);
        SDL_RenderCopy(screen, pauseTexture, NULL, &messageRect);
      }
    }
    SDL_RenderPresent(screen);

    /* clean used surfaces and textures */
    SDL_FreeSurface(pauseSurface);
    SDL_DestroyTexture(pauseTexture);
    SDL_FreeSurface(gameoverSurface);
    SDL_DestroyTexture(gameoverTexture);
    SDL_FreeSurface(scoreSurface);
    SDL_DestroyTexture(scoreTexture);
    SDL_FreeSurface(highScoreSurface);
    SDL_DestroyTexture(highScoreTexture);
}

void clean(SDL_Renderer *screen, SDL_Texture *topbarTexture, SDL_Texture *gameTexture, SDL_Window *window, SDL_Rect **grid, queue *body, queue *mazeq, TTF_Font *font){
  /* clean up and free freeable memory */
  SDL_DestroyRenderer(screen);
  SDL_DestroyTexture(topbarTexture);
  SDL_DestroyTexture(gameTexture);
  SDL_DestroyWindow(window);
  free_Rect2D(grid, NBY);
  while (body->head != NULL){
    queueOut(body);
  }
  while (mazeq->head != NULL){
    queueOut(mazeq);
  }
  TTF_CloseFont(font);
  TTF_Quit();
  SDL_Quit();
}
