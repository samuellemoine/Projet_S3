#include "headers.h"

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



void readScoreFile(char* str, char** highScore){
  FILE* sc;
  sc = fopen(str, "r");
  int c;
  int line = 0; int col = 0;
  while ( (c = fgetc (sc)) != EOF ){
    if (c != 10){  /* 10 is new line in ascii */
      highScore[line][col] = c;
      col++;
    }
    else{
      line++;
      col = 0;
    }
  }
  fclose(sc);
}

void writeScoreFile(char* str, char** currentHigh, int line, int newHighScore){
  FILE* sc;
  sc = fopen(str, "w");
  int i;
  for (i = 0; i < 4; i++){
    if (i != line){
      fprintf(sc,"%s\n", currentHigh[i]);
    }
    else{
      fprintf(sc, "%d\n", newHighScore);
    }
  }
  fclose(sc);
}
