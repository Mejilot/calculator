#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "list.h"
#include "stackChar.h"
#include "stackDouble.h"

#define OPERAND_MISS 4
#define OPERATOR_MISS 5

int isNum(char *s) { return *s <= 57 && *s >= 48; }

int is_oper(char s) {
  return s == '+' || s == '-' || s == '/' || s == '*' || s == '^' || s == '%';
}

int operatorTurn(char op) {
  int turn;
  if (op == '+' || op == '-')
    turn = 1;
  else if (op == '*' || op == '/' || op == '%')
    turn = 2;
  else if (op == '^')
    turn = 4;
  else if (op == 'm' || op == 'p')
    turn = 5;
  else
    turn = 3;
  return turn;
}

char to_short_func_name(char *s, int *length) {
  char short_name = -1;
  char arr1[4];
  snprintf(arr1, 4, "%s", s);
  char *func1[] = {"sin", "cos", "tan", "log"};
  char short_func1[] = {'s', 'c', 't', 'l'};
  for (int i = 0; i < 4; i++) {
    if (!strcmp(arr1, func1[i])) {
      short_name = short_func1[i];
      *length = 3;
    }
  }
  char arr2[5];
  snprintf(arr2, 5, "%s", s);
  char arr3[3];
  snprintf(arr3, 3, "%s", s);
  char *func2[] = {"asin", "acos", "atan", "sqrt"};
  char short_func2[] = {'S', 'C', 'T', 'Q'};
  for (int i = 0; i < 4; i++) {
    if (!strcmp(arr2, func2[i])) {
      short_name = short_func2[i];
      *length = 4;
    }
  }
  if (!strcmp(arr3, "ln")) {
    short_name = 'L';
    *length = 2;
  }
  return short_name;
}

char is_func(char s) {
  return s == 's' || s == 'c' || s == 't' || s == 'S' || s == 'C' || s == 'T' ||
         s == 'Q' || s == 'L' || s == 'G';
}

int expressionToList(char *expression, listNode **list) {
  int is_start = 0, error = 0, unary_minus = 0, no_paranthesis_parameter = 0;
  listNode *first_item = add(0, 0, list);
  listNode *last_item = first_item;
  while (*expression) {
    if (*expression == ' ') expression++;
    if (isNum(expression) || *expression == 'x') {
      if (*expression != 'x') {
        double num = strtod(expression, &expression);
        last_item = add('n', num, &last_item);
      } else {
        last_item = add(*expression++, 0, &last_item);
      }
      while (unary_minus) {
        last_item = add(')', 0, &last_item);
        unary_minus--;
      }
      while (no_paranthesis_parameter) {
        last_item = add(')', 0, &last_item);
        no_paranthesis_parameter--;
      }
    } else {
      if (is_oper(*expression)) {
        if ((operatorTurn(*expression) == 1 &&
             (is_start == 0 || *(expression - 1) == '(')) ||
            (operatorTurn(*expression) == 1 && (last_item->lexem != ')') &&
             (last_item->lexem != 'n') && (last_item->lexem != 'x'))) {
          if (*expression == '-') {
            unary_minus += 1;
            last_item = add('(', 0, &last_item);
            last_item = add('n', 0, &last_item);
            last_item = add('-', 0, &last_item);
          }
        } else {
          last_item = add(*expression, 0, &last_item);
        }
      } else if (*expression == '(' || *expression == ')')
        last_item = add(*expression, 0, &last_item);
      else if (*expression != 0 && *expression != ' ') {
        int length = 0;
        last_item = add(to_short_func_name(expression, &length), 0, &last_item);
        if (length == 0) {
          error = 1;
          break;
        }
        for (; length > 1; length--) expression++;
        if (*(expression + 1) != '(') {
          last_item = add('(', 0, &last_item);
          no_paranthesis_parameter += 1;
        }
      }
      if (*expression) expression++;
    }
    is_start++;
  }
  *list = first_item->next;
  free(first_item);
  if (error) {
    destroy(*list);
  }
  if (!*list) {
    error = 1;
  }
  return error;
}

int to_rpn(listNode *expression, listNode **result) {
  int error_code = 0;
  stackChar *stack = NULL;
  *result = initList(0, 0);
  listNode *result_start = *result;
  listNode *last_item = *result;
  for (listNode *elem = expression; elem; elem = elem->next) {
    if (elem->lexem == 'n') {
      last_item = add('n', elem->num, &last_item);
      continue;
    }
    if (is_func(elem->lexem)) {
      push(elem->lexem, &stack);
      continue;
    }
    if (elem->lexem == 'x') {
      last_item = add('x', 0, &last_item);
      continue;
    }
    if (elem->lexem == '(') {
      push(elem->lexem, &stack);
      continue;
    }
    if (elem->lexem == ')') {
      while (stack && stack->lexem != '(') {
        last_item = add(stack->lexem, 0, &last_item);
        pop(&stack);
      }
      if (!stack || stack->lexem != '(') {
        destroy(result_start);
        error_code = 2;
        break;
      }  // пропущена скобка
      pop(&stack);
      if (stack && is_func(stack->lexem)) {
        last_item = add(stack->lexem, 0, &last_item);
        pop(&stack);
      }
      continue;
    }
    if (is_oper(elem->lexem)) {
      while (stack && is_oper(stack->lexem) &&
             (operatorTurn(stack->lexem) >= operatorTurn(elem->lexem))) {
        last_item = add(stack->lexem, 0, &last_item);
        pop(&stack);
      }
      push(elem->lexem, &stack);
      continue;
    }
  }
  if (error_code != 2) {
    while (stack) {
      if (stack->lexem == '(') {
        destroy(result_start->next);
        error_code = 2;
        break;
      }
      last_item = add(stack->lexem, 0, &last_item);
      pop(&stack);
    }
    *result = result_start->next;
    free(result_start);
  }
  return error_code;
}

void change_x_to_num(listNode *result, double number) {
  while (result) {
    if (result->lexem == 'x') {
      result->num = number;
    }
    result = result->next;
  }
}

double apply_operator(double x, double y, char op) {
  double result = 0;
  if (op == '+') result = x + y;
  if (op == '-') result = x - y;
  if (op == '*') result = x * y;
  if (op == '^') result = pow(x, y);
  if (op == '/') result = x / y;
  if (op == '%') result = fmod(x, y);
  if (op == 's') result = sin(x);
  if (op == 'c') result = cos(x);
  if (op == 't') result = tan(x);
  if (op == 'S') result = asin(x);
  if (op == 'C') result = acos(x);
  if (op == 'T') result = atan(x);
  if (op == 'Q') result = sqrt(x);
  if (op == 'L') result = log10(x);
  if (op == 'G') result = log(x);
  return result;
}

double calculate_rpn(listNode *polish, int *error_code) {
  stackdouble *stack = NULL;
  *error_code = 0;
  while (polish) {
    if (polish->lexem == 'n' || polish->lexem == 'x') {
      pushD(polish->num, &stack);
    } else if (is_func(polish->lexem)) {
      if (!stack) {
        *error_code = OPERAND_MISS;
        break;
      }
      double x = popD(&stack);
      pushD(apply_operator(x, 0, polish->lexem), &stack);
    } else {
      if (!stack) {
        *error_code = OPERAND_MISS;
        break;
      }
      double x = popD(&stack);
      if (!stack) {
        *error_code = OPERAND_MISS;
        break;
      }
      double y = popD(&stack);
      pushD(apply_operator(y, x, polish->lexem), &stack);
    }
    polish = polish->next;
  }
  double res = popD(&stack);
  if (stack != NULL) {
    destroyStackD(&stack);
    *error_code = OPERATOR_MISS;
  }
  return res;
}

void calculator(char *expression, double x, double *res, int *error_code) {
  listNode *list = NULL;
  *error_code = expressionToList(expression, &list);
  if (!*error_code) {
    listNode *polish = NULL;
    *error_code = to_rpn(list, &polish);
    if (!*error_code) {
      change_x_to_num(polish, x);
      *res = calculate_rpn(polish, error_code);
      destroy(polish);
    }
    destroy(list);
  }
}
