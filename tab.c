#include "headers.h"

SDL_Rect** allocate_Rect2D(int n, int m){
  SDL_Rect** tab = malloc(n * sizeof(SDL_Rect*));
  for (int i = 0; i < m; i++){
    tab[i] = malloc(m * sizeof(SDL_Rect));
  }
  return tab;
}

void free_Rect2D(SDL_Rect** tab, int n){
  for (int j = 0; j < n; j++){
    free(tab[j]);
  }
}

char** allocate_Char2D(int n, int m){
  char** tab = malloc(n * sizeof(char*));
  for (int i = 0; i < m; i++){
    tab[i] = malloc(m * sizeof(char));
  }
  return tab;
}

void free_Char2D(char** tab, int n){
  for (int j = 0; j < n; j++){
    free(tab[j]);
  }
}
