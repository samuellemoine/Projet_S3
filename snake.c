#include "headers.h"

void colorVariation(fadeColor* col){
  if (col[0].rgb == 100.0) col[0].variation = - fabs(col[0].variation);
  if (col[0].rgb == 40.0) col[0].variation = fabs(col[0].variation);
  col[0].rgb += col[0].variation;
}

void setRect(SDL_Rect* r, int x, int y, int w, int h){
  r->x = x; r->y = y; r->w = w, r->h = h;
}

void setDir(axe* currentDir, axe *dir){
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

bool foodContact(SDL_Rect* head, SDL_Rect* food){
  if ( indice(food->x, SNAKE_WIDTH) == indice(head->x, SNAKE_WIDTH) && indice(food->y, SNAKE_HEIGHT) == indice(head->y, SNAKE_HEIGHT) ){
        return true;
  }
  return false;
}

bool specialFoodContact(SDL_Rect* head, SDL_Rect* bonusFood){
  if ( indice(bonusFood->x, SNAKE_WIDTH) == indice(head->x, SNAKE_WIDTH) && indice(bonusFood->y, SNAKE_HEIGHT) == indice(head->y, SNAKE_HEIGHT) ){
        return true;
  }
  return false;
}

bool snakeContact(queue* body){
  SDL_Rect head = queueBack(body);
  int hx = indice(head.x, SNAKE_WIDTH);
  int hy = indice(head.y, SNAKE_HEIGHT);

  Element* element = body->head;
  axe dir;
  SDL_Rect behindTail;
  /* check if there is collision with the smoothTail rectangle */
  if (element != NULL && element->nextPos != NULL && element->nextPos->nextPos != NULL){
    dir = tailDir(body);
    behindTail = (SDL_Rect) {element->pos.snakeRect.x - dir.dx * SNAKE_WIDTH, element->pos.snakeRect.y + dir.dy * SNAKE_HEIGHT, SNAKE_WIDTH, SNAKE_HEIGHT};
    if (hx == indice(behindTail.x , SNAKE_WIDTH) && hy == indice(behindTail.y , SNAKE_HEIGHT)){
      return true;
    }
  }
  while (element->nextPos != NULL){
    if (indice(element->pos.snakeRect.x, SNAKE_WIDTH) == hx && indice(element->pos.snakeRect.y, SNAKE_HEIGHT) == hy){
      return true;
    }
    element = element->nextPos;
  }
  return false;
}

bool mazeContact(snake* head, queue* mazeq){
  Element *element = mazeq->head;
  bool xContact, yContact;
  while (element != NULL){
    xContact = indice(element->pos.snakeRect.x, SNAKE_WIDTH) == indice(head->snakeRect.x, SNAKE_WIDTH);
    yContact = indice(element->pos.snakeRect.y, SNAKE_HEIGHT) == indice(head->snakeRect.y, SNAKE_HEIGHT);
    if (xContact && yContact) return true;
    element = element->nextPos;
  }
  return false;
}

SDL_Rect randFood(queue* body, queue* mazeq){
  int randx = rand()%(NBX);
  int randy = rand()%(NBY);
  if (!validFood(body, mazeq, randx, randy)){
    return randFood(body, mazeq);
  }
  SDL_Rect r = {randx * SNAKE_WIDTH + 8, randy * SNAKE_HEIGHT + TOP_BAR + 8, 9, 9};
  return r;
}

SDL_Rect randSpecialFood(queue* body, queue* mazeq, SDL_Rect* food){
  int randx = rand()%(NBX);
  int randy = rand()%(NBY);
  if (!validFood(body, mazeq, randx, randy) || (indice(food->x, SNAKE_WIDTH) == randx && indice(food->y, SNAKE_HEIGHT) == randy)){
    return randSpecialFood(body, mazeq, food);
  }
  SDL_Rect r = {randx * SNAKE_WIDTH, randy * SNAKE_HEIGHT + TOP_BAR, SNAKE_WIDTH, SNAKE_HEIGHT};
  return r;
}

bool validFood(queue* body, queue* mazeq, int x, int y){
  int cpt = 0;
  int i, j;
  Element *element = mazeq->head;
  while (element != NULL){
    /* verify that it is possible to eat the food */
    for (i = -1; i <= 1; i++){
      for (j = -1; j <= 1; j++){
        if ((x * SNAKE_WIDTH  == element->pos.snakeRect.x + i *  SNAKE_WIDTH  && y * SNAKE_WIDTH  == (element->pos.snakeRect.y - TOP_BAR) + j * SNAKE_WIDTH ) && fabs((double)i) != fabs((double)j)){
          cpt++;
        }
      }
    }
    if ( (element->pos.snakeRect.x == x * SNAKE_WIDTH && (element->pos.snakeRect.y - TOP_BAR) == y * SNAKE_WIDTH) || cpt == 3){
      return false;
    }
    element = element->nextPos;
  }
  if (body == NULL) return !(NBX/2 == x && NBY/2 == y);

  element = body->head;
  while (element != NULL){
    if (element->pos.snakeRect.x  == x * SNAKE_WIDTH && (element->pos.snakeRect.y - TOP_BAR) == y * SNAKE_HEIGHT){
      return false;
    }
    element = element->nextPos;
  }
  return true;
}

void randMaze(int nb, queue* mazeq, SDL_Rect** grid, char** lines, char* path){
  if (nb == 0){
    writeMazeFile(path, lines);
    return;
  }
  int x = rand()%(NBX);
  int y = rand()%(NBY);
  Element *element = mazeq->head;
  while (element != NULL){
    if ( (element->pos.snakeRect.x == x * SNAKE_WIDTH && (element->pos.snakeRect.y - TOP_BAR) == y * SNAKE_HEIGHT) || !validMaze(x, y)){
      return randMaze(nb, mazeq, grid, lines, path);
    }
    element = element->nextPos;
  }
  queueIn(mazeq, &grid[y][x], NULL);
  lines[y][x] = '1';
  return randMaze(nb - 1, mazeq, grid, lines, path);
}

bool validMaze(int x, int y){
  int i; int j;
  if (x >= NBX || y >= NBY) return false;
  for (i = -3; i < 4; i++){
    for (j = -3; j < 4; j++){
      if ( x == NBX / 2 + i && y == NBY / 2 + j ){
        return false;
      }
    }
  }
  return true;
}



SDL_Rect smoothHead(snake* head, SDL_Rect** grid){
  int i = 0; int j = 0;
  SDL_Rect smooth = {0, 0, SNAKE_WIDTH, SNAKE_HEIGHT};
  if (head->dir.dx == 0){
    i = indice(head->snakeRect.x, SNAKE_WIDTH);
    if (head->dir.dy == -1){
      j = - SNAKE_HEIGHT;
    }
    setRect(&smooth, grid[0][i].x, head->snakeRect.y + j, SNAKE_WIDTH, SNAKE_HEIGHT);
  }
  else if (head->dir.dy == 0){
    j = indice(head->snakeRect.y - TOP_BAR, SNAKE_HEIGHT);
    if (head->dir.dx == -1){
      i = - SNAKE_WIDTH;
    }
    setRect(&smooth, head->snakeRect.x + i, grid[j][0].y, SNAKE_WIDTH, SNAKE_HEIGHT);
  }
  return smooth;
}

SDL_Rect smoothTail(snake* head, queue *body, SDL_Rect* smoothFront, int score){
  axe taildir;
  SDL_Rect smooth;
  SDL_Rect tail = queueFront(body);

  if (score == 0){
    if (head->dir.dx == 0){
      setRect(&smooth, smoothFront->x, smoothFront->y - head->dir.dy * SNAKE_HEIGHT, SNAKE_WIDTH, SNAKE_HEIGHT);
    }
    if (head->dir.dy == 0){
      setRect(&smooth, smoothFront->x - head->dir.dx * SNAKE_WIDTH, smoothFront->y, SNAKE_WIDTH, SNAKE_HEIGHT);
    }
  }

  SDL_Rect hd = queueBack(body);
  int v;
  if (head->dir.dx == 0) v = abs(hd.y - smoothFront->y);
  else v = abs(hd.x - smoothFront->x);
  if (score > 0){
    taildir = tailDir(body);
    if (taildir.dy == 0){
      if (taildir.dx == 1){
        setRect(&smooth, tail.x + v - SNAKE_WIDTH, tail.y, SNAKE_WIDTH, SNAKE_HEIGHT);
      }
      if (taildir.dx == -1){
        setRect(&smooth, tail.x - v + SNAKE_WIDTH, tail.y, SNAKE_WIDTH, SNAKE_HEIGHT);
      }
    }
    if (taildir.dx == 0){
      if (taildir.dy == 1){
        setRect(&smooth, tail.x, tail.y - v + SNAKE_WIDTH, SNAKE_WIDTH, SNAKE_HEIGHT);
      }
      if (taildir.dy == -1){
        setRect(&smooth, tail.x, tail.y + v - SNAKE_WIDTH, SNAKE_WIDTH, SNAKE_HEIGHT);
      }
    }
  }
  return smooth;
}


int abs(int a){
  if (a < 0) return -a;
  return a;
}
