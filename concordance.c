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

void printWord(wordNode *w) {
  numberNode *n;
  printf("%s", w->word);
  for(n = w->numberHead; n; n = n->link) {
    printf(" %d", n->number);
  }
  printf("\n");
}

void printConcordanceNonAlphabetical(void) {
  wordNode *w;
  for(w = concordance; w != NULL; w = w->link) {
    printWord(w);
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

/*void merge(wordNode *output, int size1, int size2) {
  wordNode temp[size1 + size2];
  int ptr1 = 0, ptr2 = 0;
  while(ptr1 + ptr2 < size1 + size2) {
    if(ptr1 < size1 && output[ptr1] <= output[size1 + ptr2] || ptr1 < size1 && ptr2 >= size2) {
      temp[ptr1 + ptr2] = output[ptr++];
    }
    if(ptr2 < size2 && output[size1 + ptr2] < output[ptr1] || ptr2 < size2 && ptr1 >= size1) {
      output[ptr1 + ptr2] = output[size1 + ptr2++];
    }
  }
  for(int i = 0; i < size1 + size2; i++) {
    output[i] = temp[i];
  }
}*/

void mergeSort(wordNode* output, int size) {
  if(size == 1) {
    return;
  }
  int size1 = size/2, size2 = size-size1;
  mergeSort(output, size1);
  mergeSort(output + size1, size2);
//  merge(output, size1, size2);
}

void printSortedConcordance(wordNode *head, int size) {
  //wordNode output[size];
  int i = 0;
  wordNode *tmp;
  tmp = head;
  while(tmp->link != NULL) {
    //*output[i] = *tmp;
    tmp = tmp->link;
    i++;
    //printf("output at %d is %s\n", i, tmp->word);
  }

  //mergeSort(*output, i);
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
      //printf("LineNumber %d\n", lineNumber);

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
          //printf("Token %s is INVALID. Discarding.\n", token);
        }
        token = strtok(NULL, delims);
      }
    }

    printSortedConcordance(concordance, nodeCount);
    //printf("Nodecount is %d\n", nodeCount);

    printf("Printing concordance...\n");
    printConcordanceNonAlphabetical();
    fclose(inFile);
  }
  else {
    printf("Could not open file %s, please make sure the file exists.\n", inputFile);
  }

  return 0;
}
