/*
* Author: Kyle Helle
* Seattle, WA
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>  // isupper, tolower, etc...

#define BUFFER_SIZE 1024
#define true 1
#define false 0

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

int tokenizeFile(FILE* inFile, char *word) {
  printf("Tokenizing file...\n");
  const char *delimiter_chars = " \n";
  char buffer[BUFFER_SIZE];
  char *lastToken;
  if(inFile == NULL) {
    printf("Input file is NULL... exiting.\n");
    return false;
  }
  else {
    while(fgets(buffer, BUFFER_SIZE, inFile) != NULL) {
      lastToken = strtok(buffer, delimiter_chars);
      while(lastToken != NULL) {
        printf("%s\n", lastToken);
        *word = *lastToken;
        lastToken = strtok(NULL, delimiter_chars);
      }
    }
    if(ferror(inFile)) {
      perror("The following error occurred\n");
      return false;
    }
    return true;
  }
}

int getWord(char *w, FILE* inFile) {

  char c;
  while((c = fgetc(inFile)) != EOF && !isalpha(c));
  if(c == EOF) {
    return false;
  }
  else {
    *w++ = tolower(c);
    while((c = fgetc(inFile)) != EOF && isalpha(c)) {
      *w++ = tolower(c);
    }
    *w = '\0';
    return true;
  }
}

// looks through list of words for the word specified by wordBuffer.
// if word is not found, then a new wordNode is created and inserted into
// the list of words. a pointer to the wordNode (newly created or already
// existing) is returned.
wordNode *findWord(char *wordBuffer) {
  wordNode* tmp = makeWordNode(wordBuffer);
  concordance = tmp;
  printf("Found word: %s\n", concordance->word);

  return tmp;
}

numberNode *makeNumberNode(int n) {
  // TODO
  numberNode *tmp;
  tmp = (numberNode *) malloc(sizeof(numberNode));
  tmp->number = n;
  tmp->link = NULL;
  return tmp;
}

void insertNumber(wordNode *w, int n) {
  if(w->numberHead) {
    w->numberTail->link = makeNumberNode(n);
    w->numberTail = w->numberTail->link;
  }
  else {
    w->numberHead = w->numberTail = makeNumberNode(n);
  }
}

void printWord(wordNode *w) {
  numberNode *n;
  printf("%s", w->word);
  for(n = w->numberHead; n; n = n->link) {
    printf(" %d", n->number);
  }
  printf("\n");
}

void printConcordance(void) {
  wordNode *w;
  for(w = concordance; w != NULL; w = w->link) {
    printWord(w);
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
    //tokenizeFile(inFile);

    printf("======================================\n");
    char wordBuffer[1024];
    int wordNumber = 0; // count words as they are read


    while(getWord(wordBuffer, inFile)) {
      wordNumber++;
      printf("WordNumber count is: %d\n", wordNumber);
      insertNumber(findWord(wordBuffer), wordNumber);
    }

    printf("Printing concordance...\n");
    printConcordance();
    fclose(inFile);
  }
  else {
    printf("Could not open file %s, please make sure the file exists.\n", inputFile);
  }

  return 0;
}
