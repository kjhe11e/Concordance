#ifndef CONCORDANCE_H_
#define CONCORDANCE_H_

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>  // isupper, tolower, etc...

#define BUFFER_SIZE 1024
#define true 1
#define false 0

// struct to track line numbers for a given wordNode (i.e. word)
typedef struct numberNode {
  int number;
  struct numberNode *link;
} numberNode;

// struct representing a found word
typedef struct wordNode {
  char *word;
  numberNode *numberHead;
  numberNode *numberTail;
  struct wordNode *link;
} wordNode;

// the concordance, implemented as a linked-list
wordNode *concordance = NULL;

wordNode *makeWordNode(char* w);
int getWord(char *w, FILE* inFile, int *lineNumber);
numberNode *makeNumberNode(int n);
void insertNumber(wordNode *w, int n);
wordNode *findWord(char *wordBuffer);
int validateToken(char* tok);
int comp(const void *v1, const void *v2);
int getConcordanceSize(wordNode *counter);
void printConcordance(wordNode array[], int n);

#endif
