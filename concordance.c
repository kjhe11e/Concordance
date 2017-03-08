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

wordNode *makeWordNode(char* w);
int getWord(char *w, FILE* inFile, int *lineNumber);
numberNode *makeNumberNode(int n);
void insertNumber(wordNode *w, int n);
wordNode *findWord(char *wordBuffer);
int validateToken(char* tok);

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

int getWord(char *w, FILE* inFile, int *lineNumber) {
  char c;
  // skip non-alphabetic chars, look for newlines to update lineNumber
  while((c = fgetc(inFile)) != EOF && !isalpha(c)) {
    if(c == '\n') {
      //printf("Line with no valid words: %d\n", *lineNumber);
      (*lineNumber)++;
    }
  }

  if(c == EOF) {
    return false;
  }
  else {
    *w++ = tolower(c);
    while((c = fgetc(inFile)) != EOF) {
      if (c == '\n') {
        //printf("Passed line %d\n", *lineNumber);
        (*lineNumber)++;
        break;
      }
      else if(isalpha(c)) {
        *w++ = tolower(c);
      }
      else if (c == ' ' || c == '\t' || c == '\v') {
        break;
      }
    }
    *w = '\0';  // null-terminate string
    return true;
  }
}

numberNode *makeNumberNode(int n) {
  numberNode *tmp;
  tmp = (numberNode *) malloc(sizeof(numberNode));
  tmp->number = n;
  tmp->link = NULL;
  return tmp;
}

void insertNumber(wordNode *w, int n) {
  if(w->numberHead == NULL) {
    w->numberHead = w->numberTail = makeNumberNode(n);
    //printf("Wordnode %s is on lineNumber %d\n\n", w->word, n);
  }
  else {
    w->numberTail->link = makeNumberNode(n);
    w->numberTail = w->numberTail->link;
  }
}

// Looks through list of words for the word specified by wordBuffer.
// If word is not found, then a new wordNode is created and inserted into
// the list of words. A pointer to the wordNode (newly created or already
// existing) is returned.
wordNode *findWord(char *wordBuffer) {
  // make lowercase
  for(int k = 0; k < strlen(wordBuffer); k++) {
    wordBuffer[k] = tolower(wordBuffer[k]);
  }
  if(concordance == NULL) {
    //printf("Concordance is NULL\n");
    wordNode *tmp;
    tmp = makeWordNode(wordBuffer);
    concordance = tmp;
    //printf("Concordance is now: %s\n", concordance->word);
    return concordance;
  }
  else {
    wordNode *tmp;
    int found = 0;
    for(tmp = concordance; tmp != NULL; tmp = tmp->link) {
      if(strcmp(tmp->word, wordBuffer) == 0) {
        //printf("Found wordNode: %s\n", tmp->word);
        found = 1;
        return tmp;
      }
    }
    if(found == 0) {
      //printf("%s not found. Creating...\n", wordBuffer);
      wordNode *tmp;
      tmp = concordance;
      while(tmp->link != NULL) {
        tmp = tmp->link;
      } // now tmp->link == NULL
      tmp->link = makeWordNode(wordBuffer);
      return tmp->link;
    }
  }
}

int validateToken(char* tok) {
  int i = 0;
  for(i = 0; i < strlen(tok) - 1; i++) {
    if(!isalpha(tok[i])){
      return 0;
    }
  }
  return 1;
}

int comp(const void *v1, const void *v2){
  const wordNode *p1 = (wordNode *)v1;
  const wordNode *p2 = (wordNode *)v2;
  printf("Comparing %s to %s\n", p1->word, p2->word);
  //if(p1->word < p2->word){
  if(strcmp(p1->word, p2->word) > 0) {
    printf("%s is < %s\n", p1->word, p2->word);
    return 1;
  }
  //else if (p1->word > p2->word) {
  else if(strcmp(p1->word, p2->word) < 0) {
    printf("%s is > %s\n", p1->word, p2->word);
    return -1;
  }
  else {
    return 0;
  }
}

void printConcordance(wordNode array[], int n) {
  for(int j = 0; j < n; j++){
    numberNode *p;
    printf("%s ", array[j].word);
    for( p = array[j].numberHead; p; p = p->link) {
      printf(" %d", p->number);
    }
    printf("\n");
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
    printf("======================================\n");

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
    //qsort( concordance, sizeof(wordNode)/sizeof(wordNode *), sizeof(concordance), comp);

    printf("Printing concordance...\n");
    wordNode array[count];
    counter = concordance;
    int n = 0;
    while(counter != NULL) {
      memcpy(&array[n], counter, sizeof(wordNode));
      counter = counter->link;
      n++;
    }
    for(int i = 0; i < n; i++) {
      printf("Token %s\n", array[i].word);
    }
    qsort(array, n, sizeof(wordNode), comp);
    for(int i = 0; i < n; i++) {
      printf("Token %s\n", array[i].word);
    }

    printConcordance(array, n);
    // for each wordNode:
    //  push to array
    // then sort array
    // then print

  }
  else {
    printf("Could not open file %s, please make sure the file exists.\n", inputFile);
  }

  return 0;
}
