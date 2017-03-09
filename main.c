/*
* Author: Kyle Helle
* Seattle, WA
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>  // isupper, tolower, etc...

#include "concordance.h"
#include "concordance.c"

int main(int argc, char *argv[]) {
  int i = 0;

  if(argc > 2) {
    //printf("Program only supports one input file at this time. Exiting\n");
    return 0;
  }

  FILE* inFile;
  char* inputFile = argv[1];

  inFile = fopen(inputFile, "r");
  if(inFile != NULL) {

    char wordBuffer[1024];
    int lineNumber = 0; // count words as they are read
    char lineBuffer[10000];
    char* token;
    const char delims[2] = " -";
    int j = 0;
    int nodeCount = 0;
    wordNode *tmp;

    while(fgets(lineBuffer, sizeof(lineBuffer), inFile)) {
      lineNumber++;

      token = strtok(lineBuffer, delims); // strtok has known issues... use carefully
      while(token != NULL) {
        char *newline = strchr( token, '\n' );
        if ( newline ) {
          *newline = 0;
        }
        j = validateToken(token);
        if(j == 1) {
          tmp = findWord(token);
          nodeCount++;
          insertNumber(tmp, lineNumber);
        }
        else {
          // token is invalid.
        }
        token = strtok(NULL, delims);
      }
    }

    wordNode *counter = concordance;
    int count = 0;
    while(counter != NULL){
      counter = counter->link;
      count++;
    }

    wordNode array[count];
    counter = concordance;
    int n = 0;
    while(counter != NULL) {
      memcpy(&array[n], counter, sizeof(wordNode));
      counter = counter->link;
      n++;
    }

    qsort(array, n, sizeof(wordNode), comp);
    printConcordance(array, n);
  }
  else {
    //printf("Could not open file %s, please make sure the file exists.\n", inputFile);
  }

  return 0;
}
