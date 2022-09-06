#include "stackChar.h"

#include <stdio.h>
#include <stdlib.h>

stackChar* newNode(char word) {
  stackChar* elem = (stackChar*)malloc(sizeof(stackChar) * 1);
  elem->prev = 0;
  elem->lexem = word;
  return elem;
}

void push(char lexem, stackChar** tail) {
  stackChar* nextElem = newNode(lexem);
  nextElem->prev = *tail;
  *tail = nextElem;
}

char pop(stackChar** tail) {
  // - удаление элемента списка
  char popped = -1;
  if (*tail) {
    stackChar* temp = *tail;
    *tail = (*tail)->prev;
    popped = temp->lexem;
    free(temp);
  }
  return popped;
}
