#include "expressions.h"

expression * makeList(expression * exp){
  exp->type = LIST_EXP;
  exp->data.list = new std::list<expression*>();
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

void deleteExpression(expression * any){
  std::list<expression *> * list;
  if(any->type == LIST_EXP){
    list = any->data.list;
    for (std::list<expression *>::const_iterator iterator = list->begin(), end = list->end(); iterator != end; ++iterator) {
      deleteExpression(*iterator);
    }
    delete list;
  }else if(any->type == STR_EXP){
    delete any->data.str;
  }
  delete any;
}


