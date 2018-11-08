#include "headers.h"

queue* initialize(){
  queue *body = malloc(sizeof(*body));
  body->head = NULL;
  return body;
}

int queueSize(queue *body){
  int i = 0;
  Element *element = body->head;
  if (element == NULL){
    return -1;
  }
  while (element->nextPos != NULL){
    i += 1;
    element = element->nextPos;
  }
  return i;
}

void queueIn(queue *body, SDL_Rect *pos){
  Element *newPos = malloc(sizeof(*newPos));
  if (body == NULL || newPos == NULL){
    exit(EXIT_FAILURE);
  }
  newPos->pos = *pos;
  newPos->nextPos = NULL;

  if (body->head != NULL){ /* queue not empty */
    Element *element = body->head;
    while (element->nextPos != NULL){
      element = element->nextPos;
    }
      element->nextPos = newPos;
  }
  else{
    body->head = newPos;
  }
}

void queueOut(queue *body){
  if (body == NULL){
    exit(EXIT_FAILURE);
  }

  if (body->head != NULL){
    Element *element = body->head;
    body->head = element->nextPos;
    free(element);
  }
}

/* returns the snake head */
SDL_Rect queueBack(queue *body){
  Element *element = body->head;
  while (element->nextPos != NULL){
    element = element->nextPos;
  }
  return element->pos;
}


/* returns the snake tail */
SDL_Rect queueFront(queue *body){
  if (body == NULL){
    exit(EXIT_FAILURE);
  }
  return body->head->pos;
}

SDL_Rect randFood(queue *body, queue *mazeq){
  int randx = rand()%(NBX);
  int randy = rand()%(NBY);
  if (!validRand(body, mazeq, randx, randy)){
    return randFood(body, mazeq);
  }
  SDL_Rect r = {randx * SNAKE_WIDTH, randy * SNAKE_HEIGHT + TOP_BAR, SNAKE_WIDTH, SNAKE_HEIGHT };
  return r;
}

bool validRand(queue *body, queue *mazeq, int x, int y){
  if ( x < 0 || y < 0 || x >= NBX || y >= NBY ) return false;
  int cpt = 0;
  int i, j;
  Element *element = body->head;
  while (element != NULL){
    if (element->pos.x  == x * SNAKE_WIDTH && (element->pos.y - TOP_BAR) == y * SNAKE_HEIGHT){
      return false;
    }
    element = element->nextPos;
  }
  element = mazeq->head;
  while (element != NULL){
    /* verify that it is possible to eat the food */
    for (i = -1; i <= 1; i++){
      for (j = -1; j <= 1; j++){
        if ((x * SNAKE_WIDTH  == element->pos.x + i *  SNAKE_WIDTH  && y * SNAKE_WIDTH  == (element->pos.y - TOP_BAR) + j * SNAKE_WIDTH ) && fabs((double)i) != fabs((double)j)){
          cpt++;
        }
      }
    }
    if ( (element->pos.x == x * SNAKE_WIDTH && (element->pos.y - TOP_BAR) == y * SNAKE_WIDTH) || cpt == 3){
      return false;
    }
    element = element->nextPos;
  }

  return true;
}

void randMaze(int nb, queue *mazeq, SDL_Rect** grid){
  if (nb == 0){
    return;
  }
  int x = rand()%(NBX);
  int y = rand()%(NBY);
  Element *element = mazeq->head;
  while (element != NULL){
    if ( (element->pos.x / SNAKE_WIDTH == x && (element->pos.y - TOP_BAR) / SNAKE_HEIGHT == y) || !validMaze(x, y)){
      return randMaze(nb, mazeq, grid);
    }
    element = element->nextPos;
  }
  queueIn(mazeq, &grid[x][y]);
  return randMaze(nb - 1, mazeq, grid);
}

bool validMaze(int x, int y){
  int i, j;
  for (i = -3; i < 4; i++){
    for (j = -3; j < 4; j++){
      if ( x == NBX/2 + i && y == NBX/2 + j ){
        return false;
      }
    }
  }
  return true;
}


bool snakeContact(queue *body){
  Element *element = body->head;
  SDL_Rect head = queueBack(body);

  int hx = indice(head.x, SNAKE_WIDTH);
  int hy = indice(head.y, SNAKE_HEIGHT);

  while (element->nextPos != NULL){
    if (indice(element->pos.x, SNAKE_WIDTH) == hx && indice(element->pos.y, SNAKE_HEIGHT) == hy){
      return true;
    }
    element = element->nextPos;
  }
  return false;
}

bool mazeContact(snake *head, queue *mazeq){
  Element *element = mazeq->head;
  while (element != NULL){
    if ( indice(element->pos.x, SNAKE_WIDTH) == indice(head->snakeRect.x, SNAKE_WIDTH) && indice(element->pos.y, SNAKE_HEIGHT) == indice(head->snakeRect.y, SNAKE_HEIGHT)){
      return true;
    }
    element = element->nextPos;
  }
  return false;
}

void drawSnake(queue *body, queue *mazeq, snake *head, SDL_Renderer *screen, SDL_Texture *gameTexture, SDL_Texture *topbarTexture, SDL_Rect *food, int mazeSelector, int score, TTF_Font *font, SDL_Rect** grid){
    if (body == NULL){
        exit(EXIT_FAILURE);
    }


    char scoreChar[12];
    sprintf(scoreChar, "Score: %d", score);

    char highScoreChar[12];
    char** highScores = allocate_Char2D(4, 4);
    readScoreFile("highScores.txt", highScores);
    int highScore = atoi(highScores[mazeSelector]);
    if (score > highScore){
      writeScoreFile("highScores.txt", highScores, mazeSelector, score);
    }
    free_Char2D(highScores, 4);
    sprintf(highScoreChar, "High: %d", highScore);
    SDL_Color fontColor = (SDL_Color) {22, 22, 22, 255};
    SDL_Surface *scoreSurface = TTF_RenderText_Solid(font, scoreChar, fontColor);
    SDL_Texture *scoreTexture = SDL_CreateTextureFromSurface(screen, scoreSurface);
    SDL_Surface *highScoreSurface = TTF_RenderText_Solid(font, highScoreChar, fontColor);
    SDL_Texture *highScoreTexture = SDL_CreateTextureFromSurface(screen, highScoreSurface);


    SDL_Rect scoreRect = {SNAKE_WIDTH, SNAKE_HEIGHT / 2, 200, SNAKE_WIDTH * 2};
    SDL_Rect highScoreRect = {SCREEN_WIDTH / 2 + SNAKE_WIDTH, SNAKE_HEIGHT / 2, 200, SNAKE_WIDTH * 2};

    /* this if - else if block smoothens the snake's movement */
    int i = 0;
    int j = 0;
    SDL_Rect smoothHead = {0, 0, SNAKE_WIDTH, SNAKE_HEIGHT};
    if (head->dir.dx == 0){
      i = indice(head->snakeRect.x, SNAKE_WIDTH);
      if (head->dir.dy == -1){
        j = - SNAKE_HEIGHT;
      }
      setRect(&smoothHead, grid[i][0].x, head->snakeRect.y + j, SNAKE_WIDTH, SNAKE_HEIGHT);
    }
    else if (head->dir.dy == 0){
      j = indice(head->snakeRect.y - TOP_BAR, SNAKE_HEIGHT);
      if (head->dir.dx == -1){
        i = - SNAKE_WIDTH;
      }
      setRect(&smoothHead, head->snakeRect.x + i, grid[0][j].y, SNAKE_WIDTH, SNAKE_HEIGHT);
    }

    SDL_Rect topbarRect = (SDL_Rect) { 0, 0, SCREEN_WIDTH, TOP_BAR };
    SDL_Rect snakeRect = (SDL_Rect) { 1280, 340, 30, 30 };
    SDL_Rect foodRect = (SDL_Rect) { 1340, 340, 30, 30 };
    SDL_Rect wallRect = (SDL_Rect) { 1310, 340, 30, 30 };
    SDL_RenderClear(screen);
    SDL_RenderCopy(screen, gameTexture, &foodRect, food);
    SDL_RenderCopy(screen, gameTexture, &snakeRect, &smoothHead); /* display the aligned head to smoothen the front movement*/
    SDL_RenderCopy(screen, topbarTexture, &topbarRect, &topbarRect);
    SDL_RenderCopy(screen, scoreTexture, NULL, &scoreRect);
    SDL_RenderCopy(screen, highScoreTexture, NULL, &highScoreRect);

    /* display the snake blocks */
    Element *element = body->head;
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
    SDL_RenderPresent(screen);
    SDL_FreeSurface(scoreSurface);
    SDL_DestroyTexture(scoreTexture);
    SDL_FreeSurface(highScoreSurface);
    SDL_DestroyTexture(highScoreTexture);
}
