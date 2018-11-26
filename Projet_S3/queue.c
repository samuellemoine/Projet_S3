#include "headers.h"

queue* initialize(){
  queue* body = malloc(sizeof(*body));
  body->head = NULL;
  return body;
}

void queueIn(queue* body, SDL_Rect* pos, axe* d){
  Element* newPos = malloc(sizeof(Element));
  if (body == NULL || newPos == NULL){
    exit(EXIT_FAILURE);
  }
  newPos->pos.snakeRect = *pos;
  if (d != NULL) newPos->pos.dir = *d;
  newPos->nextPos = NULL;

  if (body->head != NULL){ /* queue not empty */
    Element* element = body->head;
    while (element->nextPos != NULL){
      element = element->nextPos;
    }
      element->nextPos = newPos;
  }
  else{
    body->head = newPos;
  }
}

void queueOut(queue* body){
  if (body == NULL){
    exit(EXIT_FAILURE);
  }
  if (body->head != NULL){
    Element* element = body->head;
    body->head = element->nextPos;
    free(element);
  }
}

/* returns the snake head */
SDL_Rect queueBack(queue* body){
  if (body == NULL){
    exit(EXIT_FAILURE);
  }
  Element *element = body->head;
  while (element->nextPos != NULL){
    element = element->nextPos;
  }
  return element->pos.snakeRect;
}

/* returns the snake tail */
SDL_Rect queueFront(queue* body){
  if (body == NULL){
    exit(EXIT_FAILURE);
  }
  return body->head->pos.snakeRect;
}

axe tailDir(queue* body){
  if (body == NULL){
    exit(EXIT_FAILURE);
  }
  Element *element = body->head;
  SDL_Rect r0; r0.x = element->pos.snakeRect.x; r0.y = element->pos.snakeRect.y;
  SDL_Rect r1; r1.x = element->nextPos->pos.snakeRect.x; r1.y = element->nextPos->pos.snakeRect.y;
  if (r0.x - r1.x == 0){
    if (r0.y - r1.y == SNAKE_WIDTH) return (axe){0, 1};
    return (axe){0, -1};
  }
  if(r0.y - r1.y == 0){
    if (r1.x - r0.x == SNAKE_WIDTH) return (axe){1, 0};
    return (axe){-1, 0};
  }
  return (axe){1, 1};
}

bool tailInNode(queue* body, snake* head){
  Element* element = body->head;
  if (element == NULL || element->nextPos == NULL || element->nextPos->nextPos == NULL){
    if (element != NULL && element->nextPos == NULL && head->dir.dx != element->pos.dir.dx && head->dir.dy != element->pos.dir.dy)
      return true;
    return false;
  }
  SDL_Rect r0; r0.x = element->pos.snakeRect.x; r0.y = element->pos.snakeRect.y;
  SDL_Rect r1; r1.x = element->nextPos->pos.snakeRect.x; r1.y = element->nextPos->pos.snakeRect.y;
  SDL_Rect r2; r2.x = element->nextPos->nextPos->pos.snakeRect.x; r2.y = element->nextPos->nextPos->pos.snakeRect.y;
  axe dir = tailDir(body);
  if (dir.dx == 0) return (r0.y != r1.y && r1.y == r2.y);
  return (r0.x != r1.x && r1.x == r2.x);
}

bool inNode(Element* e){
  Element* element = e;
  if (element == NULL || element->nextPos == NULL) return false;
  SDL_Rect r0; r0.x = element->pos.snakeRect.x; r0.y = element->pos.snakeRect.y;
  SDL_Rect r1; r1.x = element->nextPos->pos.snakeRect.x; r1.y = element->nextPos->pos.snakeRect.y;

  if (element->pos.dir.dx == 0) return (r0.y == r1.y && r0.x != r1.x);
  return (r0.x == r1.x && r0.y != r1.y);
}

int queueSize(queue* body){
  int size = 0;
  Element* element = body->head;
  while (element != NULL){
    size += 1;
    element = element->nextPos;
  }
  return size;
}
