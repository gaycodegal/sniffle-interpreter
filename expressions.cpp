#include "expressions.h"

expression * makeList(expression * exp){
  exp->type = LIST_EXP;
  exp->data.list = makeSList();
  return exp;
}

expression * makeString(char * ptr, std::size_t length){
  char c = ptr[length];
  ptr[length] = 0;
  std::string * s = new std::string(ptr);
  expression * exp = new expression();
  exp->data.str = s;
  exp->type = STR_EXP;
  ptr[length] = c;
  return exp;
}

expression * makeInt(char * ptr, int length){
  char c = ptr[length];
  ptr[length] = 0;
  expression * exp = new expression();
  exp->data.num = atoi(ptr);
  exp->type = CONST_EXP;
  ptr[length] = c;
  return exp;
}

expression * makeInt(const int num){
  expression * exp = new expression();
  exp->data.num = num;
  exp->type = CONST_EXP;
  return exp;
}

expression * makeCFunc(cfunc fn){
  expression * exp = new expression();
  exp->type = CFUNC_EXP;
  exp->data.c_func = fn;
  return exp;
}

void deleteExpression(expression * any){
  slist * list;
  if(any->type == LIST_EXP){
    list = any->data.list;
    for (snode * iter = list->head; iter != NULL; iter = iter->next) {
      deleteExpression((expression *)(iter->elem));
    }
    freeList(list);
  }else if(any->type == STR_EXP){
    delete any->data.str;
  }
  delete any;
}


