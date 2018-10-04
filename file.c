
file* initialize(){
  file *snake = malloc(sizeof(*snake));
  snake->head = NULL;
  return snake;
}


int fileSize(file *file0){
  int i = 0;
  Element *element = file0->head;
  while (element->nextPos != NULL){
    i+=1;
    element = element->nextPos;
  }
  return i;
}

void fileIn(file *snake, SDL_Rect pos){
  Element *newPos = malloc(sizeof(*newPos));
  if (snake == NULL || newPos == NULL){
    exit(EXIT_FAILURE);
  }
  newPos->pos = pos;
  newPos->nextPos = NULL;

  if (snake->head != NULL){ /* file not empty */
    Element *currentElement = snake->head;
    while (currentElement->nextPos != NULL){
      currentElement = currentElement->nextPos;
    }
      currentElement->nextPos = newPos;
  }
  else{
    snake->head = newPos;
  }
}

void fileOut(file *snake){
  if (snake == NULL){
    exit(EXIT_FAILURE);
  }

  if (snake->head != NULL){
    Element *elementDefile = snake->head;
    snake->head = elementDefile->nextPos;
    free(elementDefile);
  }
}

/* returns the snake head */
SDL_Rect fileTail(file *file0){
  Element *element = file0->head;
  while (element->nextPos != NULL){
    element = element->nextPos;
  }
  return element->pos;
}

SDL_Rect fileNb(file *file0, int size){
  Element *element = file0->head;
  for (int i=0; i<size;i++){
    if (element->nextPos != NULL){
        element = element->nextPos;
    }
  }
  return element->pos;
}

/* returns the snake tail */
SDL_Rect fileHead(file *file0){
  if (file0 == NULL){
    exit(EXIT_FAILURE);
  }
  if (file0->head != NULL){
    return file0->head->pos;
  }
  return (SDL_Rect) {-1, -1, -1, -1};
}


void putSnake(file *file, snake *head, SDL_Renderer *screen, SDL_Texture *texture, SDL_Texture *texture0, SDL_Rect *food, SDL_Rect grid[NBX][NBY]){
    if (file == NULL){
        exit(EXIT_FAILURE);
    }

    int i, j;
    SDL_Rect r = {0, 0, SNAKE_WIDTH, SNAKE_HEIGHT};

    i = 0; j = 0;
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

    /* display */
    SDL_RenderClear(screen);
    SDL_RenderCopy(screen, texture0, NULL, food);
    SDL_RenderCopy(screen, texture, NULL, &r);

    /* display the snake blocks contained in the file */
    Element *element = file->head;
    while (element != NULL){
      SDL_RenderCopy(screen, texture, NULL, &element->pos);
      element = element->nextPos;
    }
    SDL_RenderPresent(screen);
}
