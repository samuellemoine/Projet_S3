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

void readMazeFile(char* path, queue* mazeq, SDL_Rect** grid, char** lines){
  FILE* mazeFile = fopen(path, "r");
  if (mazeFile == NULL){
    if (strcmp(path, "mazeWalls.txt") == 0){
      printf("No %s file was found.\nCreating the file ...\n", path);
      mazeFile = fopen(path, "w");
      int x, y;
      for (y = 0; y < NBY; y++){
        for (x = 0; x < NBX; x++){
          if ((x > 0 && x < NBX - 1) && (y > 0 && y < NBY - 1)) sprintf(&lines[y][x], "0");
          else sprintf(&lines[y][x], "1");
        }
      }
      writeMazeFile(path, lines);
    }
  }
  else{
    int c;
    int line = 0; int col = 0;
    while ( (c = fgetc (mazeFile)) != EOF ){
      if (c >= '0' && c <= '1'){
        lines[line][col] = c;
        col++;
      }
      else if (c == '\n'){
        line++;
        col = 0;
      }
    }
  }
  fclose(mazeFile);
  putInMaze(mazeq, grid, lines);
}
void writeMazeFile(char* path, char** lines){
  FILE* mazeFile = fopen(path, "w");
  int i, j;
  for (i = 0; i < NBY; i++){
    for (j = 0; j < NBX; j++){
      if (j < NBX - 1){
        fprintf(mazeFile, "%c", lines[i][j]);
      }
      else{
        fprintf(mazeFile, "%c\n", lines[i][j]);
      }
    }
  }
  fclose(mazeFile);
  printf("Writing maze to file %s ...\n", path);
  printf("Done.\n");
}

void putInMaze(queue* mazeq, SDL_Rect** grid, char** lines){
  int i; int j;
  for (i = 0; i < NBY; i++){
    for (j = 0; j < NBX; j++){
      if (lines[i][j] == '1'){
        queueIn(mazeq, &grid[i][j]);
      }
    }
  }
}

void readScoreFile(char* path, int* allScores){
  FILE* file = fopen(path, "r");
  if (file == NULL){
    file = fopen(path, "w");
    printf("No %s file was found.\nCreating the file ...\n", path);
    fprintf(file, "0\n0\n0\n0\n");
    fclose(file);
    printf("Done.\n");
    file = fopen(path, "r");
  }
  int i = 0;
  int cpt = 0;
  while ( (fscanf (file, "%d", &i)) != EOF ){
    allScores[cpt] = i;
    cpt += 1;
  }
  fclose(file);
}

void writeScoreFile(char* path, int* currentHigh, int line, int newHighScore){
  FILE* file = fopen(path, "w");
  int i;
  for (i = 0; i < 4; i++){
    if (i != line){
      fprintf(file,"%d\n", currentHigh[i]);
    }
    else{
      fprintf(file, "%d\n", newHighScore);
    }
  }
  fclose(file);
}

int intLog(int n, int base){
  if (n < base){
    return 1;
  }
  return 1 + intLog(n / base, base);
}

char* formattedScore(int score, bool type){
  char* res = malloc(12 * sizeof(char));
  if (type){
    sprintf(res, "Score: ");
  }
  else{
    sprintf(res, "High: ");
  }
  int size = intLog(score, 10);
  int a;
  for (a = 0; a < 4 - size; a++){
    strcat(res, "0");
  }
  char* tmp = malloc(size * sizeof(char));
  sprintf(tmp, "%d", score);
  strcat(res, tmp);
  free(tmp);
  return res;
}
