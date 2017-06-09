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

expression * makeLambda(LambdaFunc * lam){
  expression * exp = new expression();
  exp->type = FUNC_EXP;
  exp->data.func = lam;
  return exp;
}

expression * makeString(char * ptr, std::size_t length){
  std::string * s = new std::string(ptr, length);
  expression * exp = new expression();
  sstr * temp = exp->data.str = new sstr();
  temp->refs = 1;
  temp->s = s;
  exp->type = STR_EXP;
  return exp;
}

void makeString(expression * exp, std::string * old){
  std::string * s = new std::string(*old);
  sstr * temp = exp->data.str = new sstr();
  temp->refs = 1;
  temp->s = s;
  exp->type = STR_EXP;
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
  if(any == NULL) return;
  slist * list;
  snode * iter;
  switch(any->type){
  case LIST_EXP:    
    list = any->data.list;
    if(list != NULL){
      if(--(any->data.list->refs) == 0){
	for (iter = list->head; iter != NULL; iter = iter->next) {
	  deleteExpression((expression *)(iter->elem));
	}
	freeList(list);
      }
    }
    break;
  case STR_EXP:
  case VAR_EXP:
    if(--(any->data.str->refs) == 0){
      delete any->data.str->s;
      delete any->data.str;
    }
    break;
  case FUNC_EXP:
    if(--(any->data.func->refs) == 0){
      delete any->data.func;
    }
    break;
  }
  delete any;
}

expression * copyExpression(expression * any){
  if(any == NULL)
    return NULL;
  expression * copy = new expression();
  slist * list;
  copy->type = any->type;
  switch(any->type){
  case LIST_EXP:    
    list = any->data.list;
    copy->data.list = list;
    ++(list->refs);
    break;
  case STR_EXP:
  case VAR_EXP:
    makeString(copy, any->data.str->s);
    break;
  case CONST_EXP:
    copy->data.num = any->data.num;
    break;
  case CFUNC_EXP:
    copy->data.c_func = any->data.c_func;
    break;
  case FUNC_EXP:
    copy->data.func = any->data.func;
    ++(any->data.func->refs);
    break;
  }
  return copy;
}

expression * trueCopyExpression(expression * any){
  if(any == NULL)
    return NULL;
  expression * copy = new expression();
  slist * list, *list2;
  snode * iter, *iter2;
  copy->type = any->type;
  switch(any->type){
  case LIST_EXP:    
    list = any->data.list;
    list2 = copyList(list);
    for (iter = list->head, iter2 = list2->head; iter != NULL; iter = iter->next, iter2 = iter2->next) {
      iter2->elem = (void*)copyExpression((expression *)(iter->elem));
    }
    copy->data.list = list2;
    break;
  case STR_EXP:
  case VAR_EXP:
    copy->data.str = any->data.str;
    ++(any->data.str->refs);
    break;
  case CONST_EXP:
    copy->data.num = any->data.num;
    break;
  case CFUNC_EXP:
    copy->data.c_func = any->data.c_func;
    break;
  case FUNC_EXP:
    copy->data.func = any->data.func;
    ++(any->data.func->refs);
    break;
  }
  return copy;
}
