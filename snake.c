#include "headers.h"

void setRect(SDL_Rect *r, int x, int y, int w, int h){
  r->x = x; r->y = y; r->w=w, r->h=h;
}

void setDir(axe *currentDir, axe *dir){
  currentDir->dx = dir->dx;
  currentDir->dy = dir->dy;
}

int indice(int coord, int size){
  int i = coord / size;
  float ratio = coord / size;
  if (ratio - i >= 0.5){
    return i + 1;
  }
  return i;
}

bool foodContact(SDL_Rect *head, SDL_Rect *food){
  if ( indice(food->x, SNAKE_WIDTH) == indice(head->x, SNAKE_WIDTH) && indice(food->y, SNAKE_HEIGHT) == indice(head->y, SNAKE_HEIGHT) ){
        return true;
  }
  return false;
}

bool snakeContact(queue *body){
  SDL_Rect head = queueBack(body);
  int hx = indice(head.x, SNAKE_WIDTH);
  int hy = indice(head.y, SNAKE_HEIGHT);

  Element *element = body->head;
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

void timeout(int milliseconds){
    int milliseconds_since = clock() * 1000 / CLOCKS_PER_SEC;
    int end = milliseconds_since + milliseconds;
    do {
        milliseconds_since = clock() * 1000 / CLOCKS_PER_SEC;
    } while (milliseconds_since <= end);
}
