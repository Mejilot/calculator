#ifndef SRC_STACK1_H_
#define SRC_STACK1_H_
typedef struct stack {
  struct stack* prev;
  char lexem;
} stackChar;

void push(char lexem, stackChar** tail);
// stackChar* find(char lexem, stackChar* root);
char pop(stackChar** tail);
#endif  // SRC_STACK1_H_
