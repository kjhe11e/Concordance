/*
* Author: Kyle Helle
* Seattle, WA
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>  // isupper, tolower, etc...

#define BUFFER_SIZE 1024

typedef struct numberNode {
  int number;
  struct numberNode *link;
} numberNode;

typedef struct wordNode {
  char *word;
  numberNode *numberHead;
  numberNode *numberTail;
  struct wordNode *link;
} wordNode;

wordNode *concordance = NULL;

wordNode *makeWordNode(char* w) {
  wordNode *tmp;
  tmp = (wordNode *) malloc(sizeof(wordNode));
  tmp->word = (char *) malloc(strlen(w));
  strcpy(tmp->word, w);
  tmp->numberHead = NULL;
  tmp->numberTail = NULL;
  tmp->link = NULL;
  return tmp;
}

void tokenizeFile(FILE* inFile) {
  printf("Tokenizing file...\n");
  const char *delimiter_chars = " \n";
  char buffer[BUFFER_SIZE];
  char *lastToken;
  if(inFile == NULL) {
    printf("Input file is NULL... exiting.\n");
  }
  else {
    while(fgets(buffer, BUFFER_SIZE, inFile) != NULL) {
      lastToken = strtok(buffer, delimiter_chars);
      while(lastToken != NULL) {
        printf("%s\n", lastToken);
        lastToken = strtok(NULL, delimiter_chars);
      }
    }
    if(ferror(inFile)) {
      perror("The following error occurred\n");
    }
  }
}

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
    tokenizeFile(inFile);
    fclose(inFile);
  }
  else {
    printf("Could not open file %s, please make sure the file exists.\n", inputFile);
  }

  return 0;
}
