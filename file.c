#include "headers.h"

int readFile(char* str, char lines[21][21]){
  FILE *file;
  file = fopen(str, "r");
  char currentLine[21];  //256 maximum characters to read
  int lineNb = 0;
  while (fgets(currentLine, sizeof(currentLine), file)) {
    strcpy(lines[lineNb], currentLine);
    lineNb += 1;
  }
  fclose(file);
  return lineNb;
}
