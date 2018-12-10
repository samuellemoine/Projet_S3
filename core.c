#include "headers.h"

int init(SDL_Window* w, SDL_Renderer** r, TTF_Font** f, const Uint8** keystate, SDL_Rect** grid){
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
  w = SDL_CreateWindow("Snake", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, TOP_BAR + SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
  if (w == NULL){
    printf("Window creation failed ! : %s", SDL_GetError());
    SDL_Quit();
    return EXIT_FAILURE;
  }
  *r = SDL_CreateRenderer(w, -1, SDL_RENDERER_ACCELERATED);
  if (*r == NULL){
    SDL_Log("Renderer creation failed ! : %s", SDL_GetError());
    SDL_Quit();
    return EXIT_FAILURE;
  }
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

void reset(bool* started, bool* pause, bool* gameover, bool* dirChanged, int* score, snake* head, queue* body, queue* mazeq, SDL_Rect* food, SDL_Rect* bonusFood, SDL_Rect** grid){
    SDL_ShowCursor(SDL_ENABLE);
    *started = false;
    *pause = true;
    *gameover = false;
    *dirChanged = false;
    *score = 0;
    axe ini = {0, 0};
    while (body->head != NULL){
      queueOut(body);
    }
    while (mazeq->head != NULL){
      queueOut(mazeq);
    }
    setRect(&head->snakeRect, grid[NBY/2][NBX/2].x, grid[NBY/2][NBX/2].y, SNAKE_WIDTH, SNAKE_HEIGHT);
    setDir(&head->dir, &ini);
    queueIn(body, &head->snakeRect, &head->dir);
    *food = randFood(body, mazeq);
    setRect(bonusFood, -1, -1, 0, 0);
}

void handleMenu(queue* mazeq, bool* started, SDL_Rect* food, SDL_Event* event, SDL_Renderer* screen, SDL_Texture* menuTexture, int* choice, int* level, int* maze, bool* keyPressed, SDL_Rect** grid){
  int x, y, sizey;
  if (!*keyPressed){
    SDL_GetMouseState(&x, &y);
    sizey = 17 * SCREEN_HEIGHT / 125;
    if ( x >= 4 * SCREEN_WIDTH / 25 && x <= 12 * SCREEN_WIDTH / 25 ){
      if ((y - 6 * SCREEN_HEIGHT / 25)/ sizey >= 0 && (y - 6 * SCREEN_HEIGHT / 25) / sizey < 5){
        *maze = (y - 6 * SCREEN_HEIGHT / 25) / sizey;
        *choice = true;
      }
    }
    else if ( x >= 13 * SCREEN_WIDTH / 25 && x <= 21 * SCREEN_WIDTH / 25 ){
      if ((y - 6 * SCREEN_HEIGHT / 25) / sizey >= 0 && (y - 6 * SCREEN_HEIGHT / 25) / sizey < 5){
        *level = (y - 6 * SCREEN_HEIGHT / 25) / sizey;
        *choice = false;
      }
    }
  }

  char** lines = allocate_Char2D(NBY, NBX);
  switch (event->type){
    case SDL_MOUSEBUTTONDOWN:
      loadMaze(maze, mazeq, grid, lines, food, started);
      SDL_ShowCursor(SDL_DISABLE);
    break;
    case SDL_MOUSEMOTION:
      *keyPressed = false;
      SDL_ShowCursor(SDL_ENABLE);
    break;
    case SDL_KEYDOWN:
    *keyPressed = true;
    SDL_ShowCursor(SDL_DISABLE);
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
            else  *maze = 4;
          }
          else{
            if (*level > 0)  *level -= 1;
            else  *level = 4;
          }
        break;
        case SDLK_DOWN:
          if (*choice){
            if (*maze < 4)  *maze += 1;
            else  *maze = 0;
          }
          else{
            if (*level < 4)  *level += 1;
            else  *level = 0;
          }
        break;
        case SDLK_RETURN:
          loadMaze(maze, mazeq, grid, lines, food, started);
          SDL_ShowCursor(SDL_DISABLE);
        break;
      }
    break;
  }
  drawMenu(screen, menuTexture, *choice, *level, *maze);
  free_Char2D(lines, NBY);

}

void drawMenu(SDL_Renderer* screen, SDL_Texture* menuTexture, int choice, int level, int maze){
  SDL_Rect leftRect = (SDL_Rect) { 4 * SCREEN_WIDTH / 25, 6 * SCREEN_HEIGHT / 25, 8 * SCREEN_WIDTH / 25, 17 * SCREEN_HEIGHT / 25 };
  SDL_Rect rightRect = (SDL_Rect) { 13 * SCREEN_WIDTH / 25, 6 * SCREEN_HEIGHT / 25, 8 * SCREEN_WIDTH / 25, 17 * SCREEN_HEIGHT / 25 };
  SDL_Rect mazeSprite = (SDL_Rect) { (maze * 2 + choice) * 240, 510, 240, 510 };
  SDL_Rect levelSprite = (SDL_Rect) { (level * 2 + choice) * 240, 0, 240, 510 };
  SDL_RenderClear(screen);
  SDL_RenderCopy(screen, menuTexture, &mazeSprite, &leftRect);
  SDL_RenderCopy(screen, menuTexture, &levelSprite, &rightRect);
  SDL_RenderPresent(screen);
}

void handleKeys(const Uint8* keyboardState, snake* head, dir* direction, bool* pause, bool* dirChanged){
  bool arrowPressed = keyboardState[SDL_SCANCODE_LEFT] || keyboardState[SDL_SCANCODE_RIGHT] || keyboardState[SDL_SCANCODE_UP] || keyboardState[SDL_SCANCODE_DOWN];
  if (arrowPressed && *pause){
    *pause = false;
  }
  if (keyboardState[SDL_SCANCODE_SPACE]){
    if (!*pause) *pause = true;
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

void move(snake* head, SDL_Rect** grid, queue* body, queue* mazeq, SDL_Rect* food, SDL_Rect* bonusFood, bool* gameover, bool* dirChanged, int level, int* score, int* bonusSize, unsigned long* lastBonus){
  bool headMoved, isInLimits;
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

  /* apply temporary positions when tmpPos within limits */
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
  headMoved = (indice(currentHead.x, SNAKE_WIDTH) != xx) || (indice(currentHead.y - TOP_BAR, SNAKE_HEIGHT) != yy);
  isInLimits = xx >= 0 && xx < NBX && yy >= 0 && yy < NBY;
  unsigned long bonusTime = ((unsigned long) ADJUST_BONUS * 1000 - (unsigned long) level*1000) / SNAKE_WIDTH * 2;
  *bonusSize = (*lastBonus - clock() * 1000 / CLOCKS_PER_SEC) / bonusTime;
  if (bonusFood->w != 0 && *bonusSize == 4){
    setRect(bonusFood, -1, -1, 0, 0);
  }
  if (isInLimits && headMoved){
    *dirChanged = false;
    if (foodContact(&head->snakeRect, food)){
      queueIn(body, &grid[yy][xx], &head->dir);
      *score += level + 1;
      *food = randFood(body, mazeq);
      if (bonusFood->w == 0 && rand()%BONUS_FOOD == 0){
        *bonusFood = randBonusFood(body, mazeq, food);
        *lastBonus = clock() * 1000 / CLOCKS_PER_SEC + (unsigned long) ADJUST_BONUS * 1000 - (unsigned long) level*1000;
      }
    }
    else if(bonusFoodContact(&head->snakeRect, bonusFood)){
      queueIn(body, bonusFood, &head->dir);
      setRect(bonusFood, -1, -1, 0, 0);
      *score += (int) ((double)*score / 20 * (double)(level + 1) ) * (*lastBonus / 1000 - clock() / CLOCKS_PER_SEC);
    }
    else{
      queueIn(body, &grid[yy][xx], &head->dir);
      queueOut(body);
    }
  }

}

void drawScreen(queue* body, queue* mazeq, snake* head, SDL_Renderer* screen, SDL_Texture* gameTexture, SDL_Rect* food, SDL_Rect* bonusFood, int mazeSelector, int score, bool pause, bool gameover, bool firstRound, fadeColor* col, TTF_Font* font, SDL_Rect** grid, bool* smoothTailFrame, SDL_Rect* lastTail, int* bonusSize){
  if (body == NULL){
      exit(EXIT_FAILURE);
  }
  colorVariation(col);
  SDL_SetRenderDrawColor(screen, 140 - (int) col[0].rgb, 140 - (int) col[0].rgb, 140 - (int) col[0].rgb, 255);

  /* create textures to display score and high score */
  int* allScores = malloc(5 * sizeof(int));
  readScoreFile("highScores.txt", allScores);
  int highScore = allScores[mazeSelector];
  if (score > highScore) writeScoreFile("highScores.txt", allScores, mazeSelector, score);
  free(allScores);
  char* highScoreChar = formattedScore(highScore, true);
  char* scoreChar = formattedScore(score, false);

  SDL_Color fontColor = (SDL_Color) {150 - col[0].rgb, 200 - col[0].rgb, 200, 255};
  SDL_Rect scoreRect = {SNAKE_WIDTH, SNAKE_HEIGHT / 2, SCREEN_WIDTH / 5 * 2, SCREEN_HEIGHT / 10};
  SDL_Rect highScoreRect = {SCREEN_WIDTH / 2 + SNAKE_WIDTH, SNAKE_HEIGHT / 2, SCREEN_WIDTH / 5 * 2, SCREEN_HEIGHT / 10};
  SDL_Texture* scoreTexture = loadTTFTexture(screen, font, &fontColor, scoreChar);
  SDL_Texture* highScoreTexture = loadTTFTexture(screen, font, &fontColor, highScoreChar);
  free(scoreChar);
  free(highScoreChar);

  SDL_Rect smoothFront = smoothHead(head, grid);
  if (gameover){
    smoothFront.x -= VELOCITY * head->dir.dx;
    smoothFront.y -= VELOCITY * head->dir.dy;
  }
  SDL_Rect smoothBack = smoothTail(head, body, &smoothFront, score);

  SDL_RenderClear(screen);
  SDL_SetRenderDrawColor(screen, 50, 200 - col[0].rgb, 150 - col[0].rgb, 255);
  SDL_RenderFillRect(screen, food);
  SDL_Texture* bonusFoodTexture = circleTexture(screen, 205, 255 - 200 + col[0].rgb, 255 - 150 + col[0].rgb, 255, SNAKE_WIDTH / 2, SNAKE_HEIGHT / 2, SNAKE_WIDTH, SNAKE_HEIGHT, *bonusSize);
  if (bonusFood->w != 0) SDL_RenderCopy(screen, bonusFoodTexture, NULL, bonusFood);
  SDL_DestroyTexture(bonusFoodTexture);

  /* display the aligned head to smoothen the front movement*/
  if (!(head->dir.dx == 0 && head->dir.dy == 0)){
    SDL_RenderFillRect(screen, &smoothFront);
    SDL_RenderCopy(screen, gameTexture, &(SDL_Rect) { 50, 0, 50, 50 }, &smoothFront);
  }
  /* display the aligned tail to smoothen the back movement*/
  SDL_Rect qfront = queueFront(body);
  if(!(qfront.x == lastTail->x && qfront.y == lastTail->y)) *smoothTailFrame = true;
  if (*smoothTailFrame && !(head->dir.dx == 0 && head->dir.dy == 0)){
    SDL_RenderFillRect(screen, &smoothBack);
    SDL_RenderCopy(screen, gameTexture, &(SDL_Rect) { 50, 0, 50, 50 }, &smoothBack);
  }
  Element *element = body->head;
  if(tailInNode(body, head)){
    *smoothTailFrame = false;
    if (element->nextPos != NULL) *lastTail = element->nextPos->pos.snakeRect;
  }
  if((*smoothTailFrame && element->nextPos == NULL) || element->nextPos != NULL){
    /* display the snake blocks */
    element = body->head;
    while (element != NULL){
      SDL_RenderFillRect(screen, &element->pos.snakeRect);
      SDL_RenderCopy(screen, gameTexture, &(SDL_Rect) { 50, 0, 50, 50 }, &element->pos.snakeRect);
      element = element->nextPos;
    }
  }

  /* display background color behind score and high score */
  SDL_SetRenderDrawColor(screen, 140 - (int) col[0].rgb, 140 - (int) col[0].rgb, 140 - (int) col[0].rgb, 255);
  SDL_Rect topbarRect = (SDL_Rect) { 0, 0, SCREEN_WIDTH, TOP_BAR };
  SDL_RenderFillRect(screen, &topbarRect);
  SDL_RenderCopy(screen, scoreTexture, NULL, &scoreRect);
  SDL_RenderCopy(screen, highScoreTexture, NULL, &highScoreRect);

  SDL_SetRenderDrawColor(screen, 150 - col[0].rgb, 200 - col[0].rgb, 200, 255);
  SDL_Rect barRect = (SDL_Rect) { 0, TOP_BAR - 3, SCREEN_WIDTH, 3 };
  SDL_RenderFillRect(screen, &barRect);

  /* display the maze blocks */
  SDL_SetRenderDrawColor(screen, 200, 200 - col[0].rgb, 150 - col[0].rgb, 255);
  element = mazeq->head;
  while (element != NULL){
    SDL_RenderFillRect(screen, &element->pos.snakeRect);
    SDL_RenderCopy(screen, gameTexture, &(SDL_Rect) { 0, 0, 50, 50 }, &element->pos.snakeRect);
    element = element->nextPos;
  }

  /* display Paused or Gameover */
  SDL_Rect messageRect;
  SDL_Texture* pauseTexture;
  SDL_Texture* gameoverTexture;
  if (!firstRound){
    messageRect = (SDL_Rect) { SNAKE_WIDTH / 6 * 5, SCREEN_HEIGHT / 2 - SCREEN_HEIGHT / 8 + TOP_BAR / 2, SCREEN_WIDTH - SNAKE_WIDTH, SCREEN_HEIGHT / 4 };
    if (gameover){
      gameoverTexture = loadTTFTexture(screen, font, &fontColor, "Gameover");
      SDL_RenderCopy(screen, gameoverTexture, NULL, &messageRect);
      SDL_DestroyTexture(gameoverTexture);
    }
    else if (pause){
      pauseTexture = loadTTFTexture(screen, font, &fontColor, "Paused");
      SDL_RenderCopy(screen, pauseTexture, NULL, &messageRect);
      SDL_DestroyTexture(pauseTexture);
    }
  }

  SDL_RenderPresent(screen);

  /* clean used surfaces and textures */
  SDL_DestroyTexture(scoreTexture);
  SDL_DestroyTexture(highScoreTexture);
  SDL_SetRenderDrawColor(screen, 140 - (int) col[0].rgb, 140 - (int) col[0].rgb, 140 - (int) col[0].rgb, 255);
}

void clean(SDL_Renderer* screen, SDL_Texture* menuTexture, SDL_Window* window, SDL_Rect** grid, queue* body, queue* mazeq, TTF_Font* font, fadeColor* color){
  /* clean up and free freeable memory */
  SDL_DestroyRenderer(screen);
  SDL_DestroyTexture(menuTexture);
  SDL_DestroyWindow(window);
  free_Rect2D(grid, NBY);
  free(color);
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
