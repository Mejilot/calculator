#include "stackDouble.h"

#include <stdio.h>
#include <stdlib.h>

stackdouble* newNodeD(double word) {
  stackdouble* elem = (stackdouble*)malloc(sizeof(stackdouble) * 1);
  elem->prev = 0;
  elem->lexem = word;
  return elem;
}

void pushD(double lexem, stackdouble** tail) {
  stackdouble* nextElem = newNodeD(lexem);
  nextElem->prev = *tail;
  *tail = nextElem;
}

double popD(stackdouble** tail) {
  // - удаление элемента списка
  double popped = -1;
  if (*tail) {
    stackdouble* temp = *tail;
    *tail = (*tail)->prev;
    popped = temp->lexem;
    free(temp);
  }
  return popped;
}

void destroyStackD(stackdouble** tail) {
  //- освобождение памяти, занимаемой структурой списка)
  while ((*tail)->prev) {
    popD(tail);
  }
  popD(tail);
}
