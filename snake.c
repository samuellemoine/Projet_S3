

void setRect(SDL_Rect *r, int x, int y, int w, int h){
  r->x = x; r->y = y; r->w=w, r->h=h;
}


void setDir(axe *currentDir, axe *dir){
  currentDir->dx = dir->dx;
  currentDir->dy = dir->dy;
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


/*bool isSameDir(axe *currentDir, axe *dir){
  return currentDir->dx == dir->dx && currentDir->dy == dir->dy;
}*/


bool foodContact(SDL_Rect *head, SDL_Rect *target){
  if ( indice(target->x, SNAKE_WIDTH) == indice(head->x, SNAKE_WIDTH) && indice(target->y, SNAKE_HEIGHT) == indice(head->y, SNAKE_HEIGHT)){
        return true;
  }
  return false;
}


void move(axe *currentDir, snake *head, SDL_Rect grid[NBX][NBY], bool gridBool[NBX][NBY], file *body, SDL_Rect *food, int *size){


  SDL_Delay(ADJUST_LEVEL - LEVEL);

  bool c;
  int tmpPosX, tmpPosY, xx, yy;

  /* update direction */
  setDir(&head->dir, currentDir);

  /* update temporary positons */
  tmpPosX = head->snakeRect.x;
  tmpPosY = head->snakeRect.y;
  tmpPosX += head->dir.dx * VELOCITY;
  tmpPosY += head->dir.dy * VELOCITY;

  /* apply temporary positions */
  if (tmpPosX >= 0 && tmpPosX <= SCREEN_WIDTH - SNAKE_WIDTH){
    head->snakeRect.x = tmpPosX;
    xx = indice(tmpPosX, SNAKE_WIDTH);
  }
  if (tmpPosY >= 0 && tmpPosY <= SCREEN_HEIGHT - SNAKE_HEIGHT){
    yy = indice(tmpPosY, SNAKE_HEIGHT);
    head->snakeRect.y = tmpPosY;
  }



  c = foodContact(&head->snakeRect, food);

  if (xx >= 0 && xx <= NBX && yy >= 0 && yy <= NBY){

  if (c){
    fileIn(body,grid[xx][yy]);
      if (head->snakeRect.x == 0){
        head->snakeRect.y += head->dir.dy * SNAKE_WIDTH;
      }
      else if (head->snakeRect.y == 0){
        head->snakeRect.x += head->dir.dx * SNAKE_HEIGHT;
      }

      *food = randFood(body);
      *size += 1;
      //printf("%d\n", fileSize(body));
      printf("%d\n", *size);
  }
else{
  fileIn(body,grid[xx][yy]);
  fileOut(body);
}

      //printf("no contact\n");
      //fileOut(body);
    //  printf("CONTACTCONTACTCONTACTCONTACT\n");



  }



}
