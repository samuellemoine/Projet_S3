#include "headers.h"

void loadMaze(int* maze, queue* mazeq, SDL_Rect** grid, char** lines, SDL_Rect* food, bool* started){
  *started = true;
  switch (*maze){
    case 1:
      readMazeFile("mazeWalls.txt", mazeq, grid, lines);
    break;
    case 2:
      randMaze(NBY/2 + rand()%(NBX/4), mazeq, grid, lines, "lastRandom.txt");
    break;
    case 3:
      randMaze(NBY + rand()%NBX/3, mazeq, grid, lines, "lastHardRandom.txt");
    break;
    case 4:
      readMazeFile("mazeWalls.txt", mazeq, grid, lines);
      randMaze(NBY/3 + rand()%(NBX/3), mazeq, grid, lines, "lastWallsRandom.txt");
    break;
  }
  *food = randFood(NULL, mazeq);   /* food position randomizer needs to be called after the maze map has been loaded */
}

SDL_Texture* loadTTFTexture(SDL_Renderer* screen, TTF_Font* font, SDL_Color* color, char* word){
  SDL_Surface* surf = TTF_RenderText_Solid(font, word, *color);
  SDL_Texture* text = SDL_CreateTextureFromSurface(screen, surf);
  SDL_FreeSurface(surf);
  return text;
}

SDL_Texture* loadBMPSurface(SDL_Renderer* screen, char* path){
  SDL_Surface* surf = SDL_LoadBMP(path);
  SDL_SetColorKey(surf, SDL_TRUE, SDL_MapRGB(surf->format, 249, 162, 255));

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
      if (line > NBY - 1 || col > NBX) {
        printf("The input file %s has too many inputs at line %d : %d ! \nExiting the program ...\n", path, line + 1, col + 1);
        exit(EXIT_FAILURE);
      }
      if (c >= '0' && c <= '1'){
        lines[line][col] = c;
        col++;
      }
      else if (c == '\n'){
        line++;
        col = 0;
      }
      else{
         printf("The input file %s contains illegal characters ! \nExiting the program ...\n", path);
         exit(EXIT_FAILURE);
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
}

void putInMaze(queue* mazeq, SDL_Rect** grid, char** lines){
  int i; int j;
  for (i = 0; i < NBY; i++){
    for (j = 0; j < NBX; j++){
      if (lines[i][j] == '1'){
        queueIn(mazeq, &grid[i][j], NULL);
      }
    }
  }
}

void readScoreFile(char* path, int* allScores){
  FILE* file = fopen(path, "r");
  if (file == NULL){
    file = fopen(path, "w");
    printf("No %s file was found.\nCreating the file ...\n", path);
    fprintf(file, "0\n0\n0\n0\n0\n");
    fclose(file);
    printf("Done.\n");
    file = fopen(path, "r");
  }

  int i = 0;
  int line = 0;
  while ( (fscanf (file, "%d", &i)) != EOF ){
    if (line == 5){
      printf("The input file %s has too many inputs or contains illegal characters at line %d ! \nExiting the program ...\n", path, line + 1);
      exit(EXIT_FAILURE);
    }
    allScores[line] = i;
    line += 1;
  }
  fclose(file);
}

void writeScoreFile(char* path, int* currentHigh, int line, int newHighScore){
  FILE* file = fopen(path, "w");
  int i;
  for (i = 0; i < 5; i++){
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
  char* res = malloc(13 * sizeof(char));
  if (type) sprintf(res, "Score: ");
  else sprintf(res, "High: ");
  int size = intLog(score, 10);
  int a;
  for (a = 0; a < 5 - size; a++) strcat(res, "0");
  char* tmp = malloc(size * sizeof(char));
  sprintf(tmp, "%d", score);
  strcat(res, tmp);
  free(tmp);
  return res;
}
