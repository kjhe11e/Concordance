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

/*int tokenizeFile(FILE* inFile, char *word) {
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
}*/

int getWord(char *w, FILE* inFile, int *lineNumber) {
  char c;
  // skip non-alphabetic chars, look for newlines to update lineNumber
  while((c = fgetc(inFile)) != EOF && !isalpha(c)) {
    if(c == '\n') {
      printf("Line with no valid words: %d\n", *lineNumber);
      (*lineNumber)++;
    }
  }

  if(c == EOF) {
    return false;
  }
  else {
    *w++ = tolower(c);
    // get alphabetic chars
    // TODO - FIX THIS LINE TO CHECK FOR NEWLINE
    //while( (c = fgetc(inFile)) != EOF && (isalpha(c) || (c == '\n')) ) {
    while((c = fgetc(inFile)) != EOF) {
      //if(c != ' ' && c != '\n') {
      if (c == '\n') {
        printf("Passed line %d\n", *lineNumber);
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

void printConcordance(void) {
  wordNode *w;
  for(w = concordance; w != NULL; w = w->link) {
    printWord(w);
  }
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
  if(w->numberHead == NULL) {
    w->numberHead = w->numberTail = makeNumberNode(n);
    printf("Wordnode %s is on lineNumber %d\n\n", w->word, n);
  }
  else {
    w->numberTail->link = makeNumberNode(n);
    w->numberTail = w->numberTail->link;
  }
  /*if(w->numberHead != NULL) {
    w->numberTail->link = makeNumberNode(n);
    w->numberTail = w->numberTail->link;
  }
  else {
    w->numberHead = w->numberTail = makeNumberNode(n);
  }*/
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
    printf("Concordance is NULL\n");
    wordNode *tmp;
    tmp = makeWordNode(wordBuffer);
    concordance = tmp;
    printf("Concordance is now: %s\n", concordance->word);
    return concordance;
  }
  else {
    wordNode *tmp;
    int found = 0;
    for(tmp = concordance; tmp != NULL; tmp = tmp->link) {
      if(strcmp(tmp->word, wordBuffer) == 0) {
        printf("Found wordNode: %s\n", tmp->word);
        found = 1;
        return tmp;
      }
    }
    if(found == 0) {
      printf("%s not found. Creating...\n", wordBuffer);
      wordNode *tmp;
      tmp = concordance;
      while(tmp->link != NULL) {
        tmp = tmp->link;
      } // now tmp->link == NULL
      tmp->link = makeWordNode(wordBuffer);
      printf("made %s\n", tmp->link->word);
      return tmp->link;
    }
    //printConcordance();
    //return concordance;
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
    wordNode *tmp;

    //while(getWord(wordBuffer, inFile, &lineNumber)) {
    while(fgets(lineBuffer, sizeof(lineBuffer), inFile)) {
      lineNumber++;
      printf("LineNumber %d\n", lineNumber);

      //while((token = strtok(lineBuffer, delims)) != NULL)

      token = strtok(lineBuffer, delims); // strtok has known issues... use carefully
      while(token != NULL) {
        char *newline = strchr( token, '\n' );
        if ( newline ) {
          *newline = 0;
        }
        printf("TOKEN is '%s'\n", token);
        j = validateToken(token);
        if(j == 1) {
          printf("Token %s is valid on lineNum %d.\n", token, lineNumber);
          tmp = findWord(token);
          printf("TMP->word is %s\n\n", tmp->word);
          //insertNumber(findWord(token), lineNumber);
          printf("LiadsfasdfneNumber is %d for %s\n", lineNumber, tmp->word);
          insertNumber(tmp, lineNumber);
        }
        else {
          printf("Token %s is INVALID. Discarding.\n", token);
        }


        // *** NEED TO VALIDATE STRING HERE ***
        //insertNumber(findWord(token), lineNumber);

        token = strtok(NULL, delims);
      }

      //insertNumber(findWord(wordBuffer), &lineNumber);
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
