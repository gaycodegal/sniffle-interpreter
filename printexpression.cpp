#include "printexpression.h"

void printVar(expression * exp){
  if(exp->type == VAR_EXP){
    std::cout << *exp->data.str;
  }else{
    std::cout << "NOT_A_VAR";    
  }
}

void printConst(expression * exp){
  if(exp->type == CONST_EXP){
    std::cout << exp->data.num;
  }else{
    std::cout << "NOT_A_CONST";    
  }
}

void printSym(expression * exp){
  if(exp->type == SYM_EXP){
    std::cout << '\'' << *exp->data.str;
  }else{
    std::cout << "NOT_A_SYM";    
  }
}

void printStr(expression * exp){
  if(exp->type == STR_EXP){
    std::cout << '"' << *exp->data.str << '"';
  }else{
    std::cout << "NOT_A_STR";    
  }
}

void printList(expression * exp){
  std::list<expression *> * list;
  expression * temp;

  if(exp->type == LIST_EXP){
    list =  exp->data.list;
    std::cout << "(";
    for (std::list<expression *>::const_iterator iterator = list->begin(), end = list->end(); iterator != end; ) {
      temp = *iterator;
      printAny(temp);
      ++iterator;
      if(iterator != end)
	std::cout << ' ';
    }
    std::cout << ")";
  }else{
    std::cout << "NOT_A_LIST";
  }
}

void printAny(expression * any){
  switch (any->type){
  case LIST_EXP:
    printList(any);
    break;
  case STR_EXP:
    printStr(any);
    break;
  case CONST_EXP:
    printConst(any);
    break;
  case VAR_EXP:
    printVar(any);
    break;
  case SYM_EXP:
    printSym(any);
    break;
  default:
    printStr(any);
    break;
  }
}
