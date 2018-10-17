#include "headers.h"

queue* initialize(){
  queue *body = malloc(sizeof(*body));
  body->head = NULL;
  return body;
}

int queueSize(queue *body){
  int i = 0;
  Element *element = body->head;
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
  Element *element = body->head;
  while (element != NULL){
    if (element->pos.x / SNAKE_WIDTH == x && (element->pos.y - TOP_BAR) / SNAKE_HEIGHT == y){
      return false;
    }
    element = element->nextPos;
  }
  element = mazeq->head;
  while (element != NULL){
    if (element->pos.x / SNAKE_WIDTH == x && (element->pos.y - TOP_BAR) / SNAKE_HEIGHT == y){
      return false;
    }
    element = element->nextPos;
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

void drawSnake(queue *body, queue *mazeq, snake *head, SDL_Renderer *screen, SDL_Texture *snakeTexture, SDL_Texture *foodTexture, SDL_Rect *food, int level, TTF_Font *font, SDL_Rect grid[NBX][NBY]){
    if (body == NULL){
        exit(EXIT_FAILURE);
    }

    int i = 0;
    int j = 0;
    SDL_Rect smoothHead = {0, 0, SNAKE_WIDTH, SNAKE_HEIGHT};

    SDL_Surface *scorebgSurface = SDL_LoadBMP("scorebg.bmp");
    SDL_Texture *scorebgTexture = SDL_CreateTextureFromSurface(screen, scorebgSurface);
    SDL_Rect scorebgRect = {0, 0, SCREEN_WIDTH, TOP_BAR};
    char scoreChar[12];
    sprintf(scoreChar, "Score: %d",(level + 1) * (queueSize(body)));
    SDL_Surface *scoreSurface = TTF_RenderText_Solid(font, scoreChar, (SDL_Color) {255, 98, 98});
    SDL_Texture *scoreTexture = SDL_CreateTextureFromSurface(screen, scoreSurface);
    SDL_Rect scoreRect = {SNAKE_WIDTH / 2, SNAKE_HEIGHT / 2, 200, TOP_BAR / 3 * 2};

    /* this if - else if block smoothens the snake's movement */
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

    /* update aligned (to the grid) positions */
    if (head->dir.dx == -1){
      head->snakeRect.x = smoothHead.x + SNAKE_WIDTH;
    }
    else if (head->dir.dx == 1){
      head->snakeRect.x = smoothHead.x;
    }
    if (head->dir.dy == -1){
      head->snakeRect.y = smoothHead.y + SNAKE_WIDTH;
    }
    else if (head->dir.dy == 1){
      head->snakeRect.y = smoothHead.y;
    }


    /* display the aligned head to smoothen the front movement*/
    SDL_RenderClear(screen);
    SDL_RenderCopy(screen, foodTexture, NULL, food);
    SDL_RenderCopy(screen, snakeTexture, NULL, &smoothHead);
    SDL_RenderCopy(screen, scorebgTexture, NULL, &scorebgRect);
    SDL_RenderCopy(screen, scoreTexture, NULL, &scoreRect);

    /* display the snake blocks contained in the queue */
    Element *element = body->head;
    while (element != NULL){
      SDL_RenderCopy(screen, snakeTexture, NULL, &element->pos);
      element = element->nextPos;
    }
    element = mazeq->head;
    while (element != NULL){
      SDL_RenderCopy(screen, snakeTexture, NULL, &element->pos);
      element = element->nextPos;
    }
    SDL_RenderPresent(screen);
    SDL_FreeSurface(scoreSurface);
    SDL_DestroyTexture(scoreTexture);
    SDL_FreeSurface(scorebgSurface);
    SDL_DestroyTexture(scorebgTexture);
}
