#ifndef SRC_LIST_H_
#define SRC_LIST_H_
typedef struct list {
  struct list* next;
  double num;
  char lexem;
} listNode;

listNode* initList(char word, double number);
listNode* add(char word, double number, listNode** elem);
void destroy(listNode* root);
#endif  // SRC_LIST_H_
