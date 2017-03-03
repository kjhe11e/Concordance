/*
* Author: Kyle Helle
* Seattle, WA
*/

#include <stdio.h>

int main(int argc, char *argv[]) {
  printf("Concordance program.\n");
  int i = 0;

  if(argc > 2) {
    printf("Program only supports one input file at this time. Exiting\n");
    return 0;
  }

  FILE* inFile;
  char* inputFile = argv[1];
  printf("Attempting to open file: %s\n", inputFile);

  inFile = fopen(inputFile, "r");
  if(inFile != NULL) {
    printf("Open successful\n");
    // call method(s) to do analysis here...
    fclose(inFile);
  }
  else {
    printf("Could not open file %s, please make sure the file exists.\n", inputFile);
  }

  return 0;
}
