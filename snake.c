void setRect(SDL_Rect *r, int x, int y, int w, int h){
  r->x = x; r->y = y; r->w=w, r->h=h;
}


void setDir(axe *currentDir, axe *dir){
  currentDir->dx = dir->dx;
  currentDir->dy = dir->dy;
}

void handleKeys(const Uint8 *keyboardState, axe *currentDir, dir *direction){
  if (keyboardState[SDL_SCANCODE_LEFT] && currentDir->dx != 1){
    setDir(currentDir, &direction->left);
  }
  if (keyboardState[SDL_SCANCODE_RIGHT] && currentDir->dx != -1){
    setDir(currentDir, &direction->right);
  }
  if (keyboardState[SDL_SCANCODE_UP] && currentDir->dy != 1){
    setDir(currentDir, &direction->up);
  }
  if (keyboardState[SDL_SCANCODE_DOWN] && currentDir->dy != -1){
    setDir(currentDir, &direction->down);
  }
}

   /* gives better precision in commands than integer division */
int indice(int coord, int size){
  int i = coord / size;
  float ratio = coord / size;
  if (ratio - i >= 0.5){
    return i + 1;
  }
  return i;
}

SDL_Rect randFood(file *file0){
  int randx = rand()%(NBX);
  int randy = rand()%(NBY);
  SDL_Rect r = {randx * SNAKE_WIDTH, randy * SNAKE_HEIGHT, SNAKE_WIDTH, SNAKE_HEIGHT };
  while (!validRand(file0, randx, randy)){
    randFood(file0);
  }
  return r;
}

bool validRand(file *file0, int x, int y){
  Element *element = file0->head;
  while (element != NULL){
    if (element->pos.x / SNAKE_WIDTH == x && element->pos.y / SNAKE_HEIGHT){
      return false;
    }
    element = element->nextPos;
  }
  return true;
}


bool isSameDir(axe *currentDir, axe *dir){
  return currentDir->dx == dir->dx && currentDir->dy == dir->dy;
}

bool snakeContact(file *body){
  Element *element = body->head;
  SDL_Rect head = fileTail(body);
  SDL_Rect tail = fileHead(body);
  if (head.x == tail.x && head.y == tail.y){
    return false;
  }
  while (element != NULL){
    if (indice(element->pos.x, SNAKE_WIDTH) == indice(head.x, SNAKE_WIDTH)
        && indice(element->pos.y, SNAKE_HEIGHT) == indice(head.y, SNAKE_HEIGHT)){
      return true;
    }
    element = element->nextPos;
  }
  return false;
}


bool foodContact(SDL_Rect *head, SDL_Rect *target){
  if ( indice(target->x, SNAKE_WIDTH) == indice(head->x, SNAKE_WIDTH) && indice(target->y, SNAKE_HEIGHT) == indice(head->y, SNAKE_HEIGHT)){
        return true;
  }
  return false;
}


void move(axe *currentDir, snake *head, SDL_Rect grid[NBX][NBY], file *body, SDL_Rect *food){


  SDL_Delay(ADJUST_LEVEL - LEVEL);

  bool nextHead, eats;
  int tmpPosX, tmpPosY;
  int xx = -1; int yy = -1;
  SDL_Rect currentHead;

  /* update direction */
  setDir(&head->dir, currentDir);

  /* update temporary positons */
  tmpPosX = head->snakeRect.x;
  tmpPosY = head->snakeRect.y;
  tmpPosX += head->dir.dx * VELOCITY;
  tmpPosY += head->dir.dy * VELOCITY;

  /* make the snake go through walls */
  if (tmpPosX < 0){
    head->snakeRect.x = SCREEN_WIDTH;
  }
  else if (tmpPosX >= SCREEN_WIDTH - SNAKE_WIDTH){
    head->snakeRect.x = 0;
  }
  if (tmpPosY < 0){
    head->snakeRect.y = SCREEN_HEIGHT;
  }
  else if (tmpPosY >= SCREEN_HEIGHT - SNAKE_HEIGHT){
    head->snakeRect.y = 0;
  }

  /* apply temporary positions whe tmpPos within limits */
  if (tmpPosX >= 0 && tmpPosX < SCREEN_WIDTH){
    head->snakeRect.x = tmpPosX;
  }
  if (tmpPosY >= 0 && tmpPosY < SCREEN_HEIGHT){
    head->snakeRect.y = tmpPosY;
  }
  /* determine position in the grid */
  xx = indice(head->snakeRect.x, SNAKE_WIDTH);
  yy = indice(head->snakeRect.y, SNAKE_HEIGHT);


  /* setting the two necessary booleans to move the snake */
  currentHead = fileTail(body);
  eats = foodContact(&head->snakeRect, food);
  nextHead = (indice(currentHead.x, SNAKE_WIDTH) != indice(head->snakeRect.x, SNAKE_WIDTH))
      || (indice(currentHead.y, SNAKE_HEIGHT) != indice(head->snakeRect.y, SNAKE_HEIGHT));


  if (xx >= 0 && xx < NBX && yy >= 0 && yy < NBY){

    if (eats && nextHead){
      fileIn(body,grid[xx][yy]);
      *food = randFood(body);
    }

    else if (!eats && nextHead){
      fileIn(body,grid[xx][yy]);
      fileOut(body);
      printf("Tail: <%d,%d>\n", fileHead(body).x/SNAKE_WIDTH, fileHead(body).y/SNAKE_HEIGHT);
    }
  }

}
