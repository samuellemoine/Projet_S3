#include "headers.h"

SDL_Texture* loadBMPSurface(SDL_Renderer* screen, char* path){
  SDL_Surface* surf = SDL_LoadBMP(path);
  if (surf == NULL){
    SDL_Log("%s\n", SDL_GetError());
  }
  SDL_Texture* text = SDL_CreateTextureFromSurface(screen, surf);
  if (text == NULL){
    SDL_Log("%s\n", SDL_GetError());
  }
  SDL_FreeSurface(surf);
  return text;
}

void readMazeFile(char* str, char** lines){
  FILE* maze;
  maze = fopen(str, "r");
  int c;
  int line = 0; int col = 0;
  while ( (c = fgetc (maze)) != EOF ){
    if (c != 10){  /* 10 is new line in ascii */
      lines[line][col] = c;
      col++;
    }
    else{
      line++;
      col = 0;
    }
  }
  fclose(maze);
}

void putInMaze(queue *mazeq, SDL_Rect** grid, char** lines){
  int i; int j;
  for (i = 0; i < NBX; i++){
    for (j = 0; j < NBY; j++){
      if (lines[i][j] - '0' == 1){
        queueIn(mazeq, &grid[i][j]);
      }
    }
  }
}



void readScoreFile(char* path, char** highScore){
  FILE* file = fopen(path, "r");
  if (file == NULL){
    fclose(file);
    file = fopen(path, "w");
    printf("No %s file was found.\nCreating the file ...\n", path);
    fprintf(file, "0\n0\n0\n0\n");
    fclose(file);
    printf("Done.\n");
    file = fopen(path, "r");
  }
  char c;
  int line = 0; int col = 0;
  while ( (c = fgetc (file)) != EOF ){
    if (c != '\n'){
      highScore[line][col] = c;
      col++;
    }
    else{
      line++;
      col = 0;
    }
  }
  fclose(file);
}

void writeScoreFile(char* path, char** currentHigh, int line, int newHighScore){
  FILE* file = fopen(path, "w");
  int i;
  for (i = 0; i < 4; i++){
    if (i != line){
      fprintf(file,"%s\n", currentHigh[i]);
    }
    else{
      fprintf(file, "%d\n", newHighScore);
    }
  }
  fclose(file);
}
