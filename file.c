#include "headers.h"

void readMazeFile(char* str, char lines[NBX][NBY]){
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
}

void putInMaze(queue *mazeq, SDL_Rect grid[NBX][NBY], char lines[NBX][NBY]){
  for (int i = 0; i < NBX; i++){
    for (int j = 0; j < NBY; j++){
      if (lines[i][j] - '0' == 1){
        queueIn(mazeq, &grid[i][j]);
      }
    }
  }
}
