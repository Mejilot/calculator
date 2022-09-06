#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"
#include "stackChar.h"
#include "stackDouble.h"

int isNum(char *s);
int is_oper(char s);
int operatorTurn(char op);
char to_short_func_name(char *s);
char is_func(char s);
int expressionToList(char *expression, listNode **list);
int to_rpn(listNode *expression, listNode **result);
void change_x_to_num(listNode *result, double number);
double apply_operator(double x, double y, char op);
double calculate_rpn(listNode *polish, int *error_code);
void calculator(char *expression, double x, double *res, int *error_code);
