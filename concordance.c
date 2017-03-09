#include "concordance.h"

// Input: char* representing the word to create
// Returns: a pointer to a wordNode
// Create a wordNode given the input parameter char* w. Memory is allocated
// to create the wordNode, the C standard library function strcpy is used to
// set the word value based on the input param, the head and tail of the linked-list
// of line numbers are set to NULL, and the link to the next wordNode is set to NULL.
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

// Input: char*, an input file, and a pointer to a lineNumber
// Returns: an int representing whether the word was found
// This method returns 1 if the word represented by char *w is found,
// and 0 if not found. The lineNumber of the input file 'inFile' is also
// tracked by looking for the newline '\n' character.
int getWord(char *w, FILE* inFile, int *lineNumber) {
  char c;
  // skip non-alphabetic chars, look for newlines to update lineNumber
  while((c = fgetc(inFile)) != EOF && !isalpha(c)) {
    if(c == '\n') {
      (*lineNumber)++;
    }
  }

  if(c == EOF) {
    return false;
  }
  else {
    *w++ = tolower(c);  // application is case insensitive.
    while((c = fgetc(inFile)) != EOF) {
      if (c == '\n') {
        (*lineNumber)++;
        break;
      }
      else if(isalpha(c)) {
        *w++ = tolower(c);
      }
      else if (c == ' ' || c == '\t' || c == '\v') {  // this signifies end of word
        break;
      }
    }
    *w = '\0';  // null-terminate the string
    return true;
  }
}

// Input: int 'n'
// Returns: poinnter to numberNode
// This function creates a numberNode based on the input 'n' and assigns
// the numberNode's link to NULL.
numberNode *makeNumberNode(int n) {
  numberNode *tmp;
  tmp = (numberNode *) malloc(sizeof(numberNode));
  tmp->number = n;
  tmp->link = NULL;
  return tmp;
}

// Input: pointer to wordNode, and int 'n' for creating a numberNode
// Returns: void
// For a given word, this function adds the line number 'n' as a numberNode
// to the list of line numbers the word appears on.
void insertNumber(wordNode *w, int n) {
  if(w->numberHead == NULL) {
    w->numberHead = w->numberTail = makeNumberNode(n);
  }
  else {
    w->numberTail->link = makeNumberNode(n);
    w->numberTail = w->numberTail->link;
  }
}

// Input: string represented by char *
// Returns: pointer to wordNode
// Looks through list of words for the word specified by wordBuffer.
// If word is not found, then a new wordNode is created and inserted into
// the list of words. A pointer to the wordNode (newly created or already
// existing) is returned.
wordNode *findWord(char *wordBuffer) {
  // make word lowercase, application is case-insensitive
  for(int k = 0; k < strlen(wordBuffer); k++) {
    wordBuffer[k] = tolower(wordBuffer[k]);
  }
  if(concordance == NULL) {
    wordNode *tmp;
    tmp = makeWordNode(wordBuffer);
    concordance = tmp;
    return concordance;
  }
  else {
    wordNode *tmp;
    int found = 0;
    for(tmp = concordance; tmp != NULL; tmp = tmp->link) {
      if(strcmp(tmp->word, wordBuffer) == 0) {
        found = 1;
        return tmp;
      }
    }
    if(found == 0) {
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

// Input: char*
// Returns: int => 1 if true, 0 if false
// Validates whether the input parameter is a valid word by using the
// C standard library method isalpha to check whether each character is
// an alphabetical character.
int validateToken(char* tok) {
  int i = 0;
  for(i = 0; i < strlen(tok) - 1; i++) {
    if(!isalpha(tok[i])){
      return 0;
    }
  }
  return 1;
}

// Input: const void pointer *v1, const void pointer *v2
// Returns: int
// This method casts the two input parameters as wordNodes and compares
// the respective word values. The C standard library function strcmp is used to
// compare the word values, which returns either -1, 0, or 1 based on the result.
int comp(const void *v1, const void *v2) {
  const wordNode *p1 = (wordNode *)v1;
  const wordNode *p2 = (wordNode *)v2;

  if(strcmp(p1->word, p2->word) > 0) {
    return 1;
  }
  else if(strcmp(p1->word, p2->word) < 0) {
    return -1;
  }
  else {
    return 0;
  }
}

// Input: pointer to a wordNode
// Returns: int
// This method counts the length of the linked-list by traversing the
// wordNode links until the end of the linked-list is encountered.
int getConcordanceSize(wordNode *counter) {
  if(counter == NULL) {
    return 0;
  }
  else {
    int count = 0;
    while(counter != NULL){
      counter = counter->link;
      count++;
    }
    return count;
  }
}

// Input: an array of wordNodes and array size 'n'
// Returns: void
// For each wordNode in the array, this function prints the
// given word along with the line numbers it appears on in the text file.
void printConcordance(wordNode array[], int n) {
  for(int j = 0; j < n; j++){
    numberNode *p;
    printf("%s", array[j].word);
    for( p = array[j].numberHead; p; p = p->link) {
      printf(" %d", p->number);
    }
    printf("\n");
  }
}
