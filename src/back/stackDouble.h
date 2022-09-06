#ifndef SRC_STACK2_H_
#define SRC_STACK2_H_
typedef struct stack1 {
  struct stack1* prev;
  double lexem;
} stackdouble;

void pushD(double lexem, stackdouble** tail);
// stackdouble* find(double lexem, stackdouble* root);
double popD(stackdouble** tail);
void destroyStackD(stackdouble** tail);
#endif  // SRC_STACK2_H_
