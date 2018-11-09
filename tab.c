#include "headers.h"

SDL_Rect** allocate_Rect2D(int n, int m){
  SDL_Rect** tab = malloc(n * sizeof(SDL_Rect*));
  int i;
  for (i = 0; i < m; i++){
    tab[i] = malloc(m * sizeof(SDL_Rect));
  }
  return tab;
}

void free_Rect2D(SDL_Rect** tab, int n){
  int i;
  for (i = 0; i < n; i++){
    free(tab[i]);
  }
}

char** allocate_Char2D(int m, int n){
  char** tab = malloc(m * sizeof(char*));
  int i;
  for (i = 0; i < m; i++){
    tab[i] = malloc(n * sizeof(char));
  }
  return tab;
}

void free_Char2D(char** tab, int n){
  int i;
  for (i = 0; i < n; i++){
    free(tab[i]);
  }
}
