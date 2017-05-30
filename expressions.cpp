#include "expressions.h"

expression * makeList(expression * exp){
  exp->type = LIST_EXP;
  exp->data.list = makeSList();
  return exp;
}

expression * makeList(slist * lst){
  expression * exp = new expression();
  exp->type = LIST_EXP;
  exp->data.list = lst;
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
//enum EXP_ENUM { LIST_EXP, STR_EXP, VAR_EXP, CONST_EXP, SYM_EXP, ENV_EXP, CFUNC_EXP };

void deleteExpression(expression * any){
  slist * list;
  snode * iter;
  switch(any->type){
  case LIST_EXP:    
    list = any->data.list;
    if(list != NULL){
      for (iter = list->head; iter != NULL; iter = iter->next) {
	deleteExpression((expression *)(iter->elem));
      }
      freeList(list);
    }
    break;
  case STR_EXP:
  case VAR_EXP:
  case SYM_EXP:
    delete any->data.str;
    break;
  case ENV_EXP:
    delete any->data.env;
    break;
  }
  delete any;
}
