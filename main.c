/*
* Author: Kyle Helle
* Seattle, WA
*/

#include "concordance.h"
#include "concordance.c"

int main(int argc, char *argv[]) {

  if(argc > 2) {
    printf("Program only supports one input file at this time.\n");
    return 0;
  }

  FILE* inFile;
  char* inputFile = argv[1]; // input file is second-passed argument
  inFile = fopen(inputFile, "r"); // attempt to open the file for read

  if(inFile != NULL) {  // then file open was successful
    int lineNumber = 0; // count words as they are read
    char lineBuffer[10000];
    char* token;
    const char delims[2] = "-";  // delimiter chars are spaces and hyphens
    int j = 0;
    int nodeCount = 0;
    wordNode *tmp;

    while(fgets(lineBuffer, sizeof(lineBuffer), inFile)) {
      lineNumber++; // line is read, so increase line number

      token = strtok(lineBuffer, delims); // strtok has known limitations; modifies first arg; not thread safe
      while(token != NULL) {
        char *newline = strchr( token, '\n' );  // if last token in line, replace newline char with null char to null-terminate the string
        if ( newline ) {
          *newline = 0;
        }
        j = validateToken(token);
        if(j == 1) {
          tmp = findWord(token);  // Time complexity is O(n * m), where 'n' is size of concordance and 'm' is length of word
          nodeCount++;
          insertNumber(tmp, lineNumber);
        }
        else {
          // token is invalid. (Either alert user here or do nothing.)
        }
        token = strtok(NULL, delims); // get next token
      }
    }

    int concordanceSize = getConcordanceSize(concordance);  // Time complexity is O(n)
    wordNode array[concordanceSize];

    wordNode *counter = concordance;
    int n = 0;
    // Convert concordance linked-list into array (for sorting)
    while(counter != NULL) {
      memcpy(&array[n], counter, sizeof(wordNode)); // convert linked-list to array
      counter = counter->link;
      n++;  // track size
    }

    qsort(array, n, sizeof(wordNode), comp);  // leverage C standard library function 'qsort' to sort array
    printConcordance(array, n);
  }
  else {
    //printf("Could not open file %s, please make sure the file exists.\n", inputFile);
  }

  return 0;
}
