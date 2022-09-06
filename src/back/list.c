#include "list.h"

#include <stdlib.h>

listNode* initList(char word, double number) {
  listNode* elem = (listNode*)malloc(sizeof(listNode) * 1);
  elem->next = 0;
  elem->lexem = word;
  elem->num = number;
  return elem;
}

/**
 * @brief вставка нового элемента после elem
 *
 * @return listNode*
 */
listNode* add(char word, double number, listNode** elem) {
  listNode* res = NULL;
  if (!*elem) {
    *elem = initList(word, number);
    res = *elem;
  } else {
    listNode* nextElem = initList(word, number);
    nextElem->next = (*elem)->next;
    (*elem)->next = nextElem;
    res = nextElem;
  }
  return res;
}

void destroy(listNode* root) {
  //- освобождение памяти, занимаемой структурой списка)
  if (root) {
    while (root != NULL) {
      listNode* temp = root->next;
      free(root);
      root = temp;
    }
  }
}
