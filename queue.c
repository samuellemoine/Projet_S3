#include "headers.h"

queue* initialize(){
  queue* body = malloc(sizeof(*body));
  body->head = NULL;
  return body;
}

int queueSize(queue* body){
  int i = 0;
  Element* element = body->head;
  if (element == NULL){
    return -1;
  }
  while (element->nextPos != NULL){
    i += 1;
    element = element->nextPos;
  }
  return i;
}

void queueIn(queue* body, SDL_Rect* pos){
  Element* newPos = malloc(sizeof(*newPos));
  if (body == NULL || newPos == NULL){
    exit(EXIT_FAILURE);
  }
  newPos->pos = *pos;
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
  return element->pos;
}

/* returns the snake tail */
SDL_Rect queueFront(queue* body){
  if (body == NULL){
    exit(EXIT_FAILURE);
  }
  return body->head->pos;
}
