
file* initialize(){
  file *body = malloc(sizeof(*body));
  body->head = NULL;
  return body;
}


int fileSize(file *body){
  int i = 0;
  Element *element = body->head;
  while (element->nextPos != NULL){
    i += 1;
    element = element->nextPos;
  }
  return i;
}

void fileIn(file *body, SDL_Rect pos){
  Element *newPos = malloc(sizeof(*newPos));
  if (body == NULL || newPos == NULL){
    exit(EXIT_FAILURE);
  }
  newPos->pos = pos;
  newPos->nextPos = NULL;

  if (body->head != NULL){ /* file not empty */
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

void fileOut(file *body){
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
SDL_Rect fileTail(file *body){
  Element *element = body->head;
  while (element->nextPos != NULL){
    element = element->nextPos;
  }
  return element->pos;
}

SDL_Rect fileNb(file *body, int size){
  Element *element = body->head;
  int i = 0;
  while (i < size && element->nextPos != NULL){
    element = element->nextPos;
    i += 1;
  }
  return element->pos;
}

/* returns the snake tail */
SDL_Rect fileHead(file *body){
  if (body == NULL){
    exit(EXIT_FAILURE);
  }
  return body->head->pos;
}


void drawSnake(file *body, snake *head, SDL_Renderer *screen, SDL_Texture *texture, SDL_Texture *texture0, SDL_Rect *food, SDL_Rect grid[NBX][NBY]){
    if (body == NULL){
        exit(EXIT_FAILURE);
    }

    int i = 0; int j = 0;
    SDL_Rect r = {0, 0, SNAKE_WIDTH, SNAKE_HEIGHT};

    /* this if - else if block smoothens the snake's movement */
    if (head->dir.dx == 0){
      i = indice(head->snakeRect.x, SNAKE_WIDTH);
      if (head->dir.dy == -1){
        j = - SNAKE_HEIGHT;
      }
      setRect(&r, grid[i][0].x, head->snakeRect.y + j, SNAKE_WIDTH, SNAKE_HEIGHT);
    }
    else if (head->dir.dy == 0){
      j = indice(head->snakeRect.y, SNAKE_HEIGHT);
      if (head->dir.dx == -1){
        i = - SNAKE_WIDTH;
      }
      setRect(&r, head->snakeRect.x + i, grid[0][j].y, SNAKE_WIDTH, SNAKE_HEIGHT);
    }

    /* update aligned (to the grid) positions */
    if (head->dir.dx == -1){
      head->snakeRect.x = r.x + SNAKE_WIDTH;
    }
    else if (head->dir.dx == 1){
      head->snakeRect.x = r.x;
    }
    if (head->dir.dy == -1){
      head->snakeRect.y = r.y + SNAKE_WIDTH;
    }
    else if (head->dir.dy == 1){
      head->snakeRect.y = r.y;
    }




    /* display */
    SDL_RenderClear(screen);
    SDL_RenderCopy(screen, texture0, NULL, food);
    SDL_RenderCopy(screen, texture, NULL, &r);
    //SDL_RenderCopy(screen, texture, NULL, &r0);

    /* display the snake blocks contained in the file */
    Element *element = body->head;
    while (element != NULL){
      SDL_RenderCopy(screen, texture, NULL, &element->pos);
      element = element->nextPos;
    }
    SDL_RenderPresent(screen);
}
