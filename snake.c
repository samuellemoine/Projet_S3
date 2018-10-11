
void setRect(SDL_Rect *r, int x, int y, int w, int h){
  r->x = x; r->y = y; r->w=w, r->h=h;
}


void setDir(axe *currentDir, axe *dir){
  currentDir->dx = dir->dx;
  currentDir->dy = dir->dy;
}

void handleKeys(const Uint8 *keyboardState, axe *currentDir, dir *direction, bool *dirChanged, bool *pause){
  bool arrowPressed = keyboardState[SDL_SCANCODE_LEFT] || keyboardState[SDL_SCANCODE_RIGHT] || keyboardState[SDL_SCANCODE_UP] || keyboardState[SDL_SCANCODE_DOWN];
  if (arrowPressed  && *pause){
        *pause = false;
  }
  if (keyboardState[SDL_SCANCODE_P]){
    *pause = !(*pause);
    SDL_Delay(350);
  }

  if (keyboardState[SDL_SCANCODE_LEFT] && currentDir->dx != 1 && !*dirChanged){
    setDir(currentDir, &direction->left);
    *dirChanged = true;
  }
  if (keyboardState[SDL_SCANCODE_RIGHT] && currentDir->dx != -1 && !*dirChanged){
    setDir(currentDir, &direction->right);
    *dirChanged = true;
  }
  if (keyboardState[SDL_SCANCODE_UP] && currentDir->dy != 1 && !*dirChanged){
    setDir(currentDir, &direction->up);
    *dirChanged = true;
  }
  if (keyboardState[SDL_SCANCODE_DOWN] && currentDir->dy != -1 && !*dirChanged){
    setDir(currentDir, &direction->down);
    *dirChanged = true;
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

bool validRand(file *body, int x, int y){
  Element *element = body->head;
  while (element != NULL){
    if (element->pos.x / SNAKE_WIDTH == x && element->pos.y / SNAKE_HEIGHT == y){
      return false;
    }
    element = element->nextPos;
  }

  return true;
}

SDL_Rect randFood(file *body){
  int randx = rand()%(NBX);
  int randy = rand()%(NBY);
  if (!validRand(body, randx, randy)){
    return randFood(body);
  }
  SDL_Rect r = {randx * SNAKE_WIDTH, randy * SNAKE_HEIGHT, SNAKE_WIDTH, SNAKE_HEIGHT };
  return r;
}




bool isSameDir(axe *currentDir, axe *dir){
  return currentDir->dx == dir->dx && currentDir->dy == dir->dy;
}

bool snakeContact(file *body){

  Element *element = body->head;
  SDL_Rect head = fileTail(body);

  int hx = indice(head.x, SNAKE_WIDTH);
  int hy = indice(head.y, SNAKE_HEIGHT);

  while (element->nextPos != NULL){
    if (indice(element->pos.x, SNAKE_WIDTH) == hx
        && indice(element->pos.y, SNAKE_HEIGHT) == hy){
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





void move(axe *currentDir, snake *head, SDL_Rect grid[NBX][NBY], file *body, SDL_Rect *food, bool *gameover, bool *dirChanged){

  SDL_Delay(ADJUST_LEVEL - LEVEL);

  bool headMoved, eats, isInLimits;
  int tmpPosX, tmpPosY;
  int xx = -1; int yy = -1;
  SDL_Rect currentHead = fileTail(body);  /* SDL_Rect of the snake's head */

  /* update direction */
  setDir(&head->dir, currentDir);

  /* update temporary positons */
  tmpPosX = head->snakeRect.x;
  tmpPosY = head->snakeRect.y;
  tmpPosX += head->dir.dx * VELOCITY;
  tmpPosY += head->dir.dy * VELOCITY;

  /* make the snake fuse through walls */
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


  /* setting the three necessary booleans to move the snake */
  eats = foodContact(&head->snakeRect, food);
  headMoved = (indice(currentHead.x, SNAKE_WIDTH) != xx) || (indice(currentHead.y, SNAKE_HEIGHT) != yy);
  isInLimits = xx >= 0 && xx < NBX && yy >= 0 && yy < NBY;

  if (isInLimits && headMoved){
    *dirChanged = false;
    if (eats){
      fileIn(body,grid[xx][yy]);
      *food = randFood(body);
    }
    else{
      fileIn(body,grid[xx][yy]);
      fileOut(body);
    }
    if (snakeContact(body)){
      *gameover = true;
    }
  }

}
