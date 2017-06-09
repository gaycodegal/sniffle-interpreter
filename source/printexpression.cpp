#include "printexpression.h"

void printVar(expression * exp){
  if(exp->type == VAR_EXP){
    std::cout << *exp->data.str->s;
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

void printLambda(expression * exp){
  if(exp->type == FUNC_EXP){
    //LambdaFunc * lam = exp->data.func;
    std::cout << "lambda";
    /*for(snode * i
      std::cout << exp->data.num;*/
  }else{
    std::cout << "NOT_A_FUNC";    
  }
}

void printStr(expression * exp){
  if(exp->type == STR_EXP){
    std::cout << '"' << *exp->data.str->s << '"';
  }else{
    std::cout << "NOT_A_STR";    
  }
}

void printList(expression * exp){
  slist * list;
  expression * temp;

  if(exp->type == LIST_EXP){
    list = exp->data.list;
    std::cout << "(";
    for (snode * iter = list->head; iter != NULL; ) {
      temp = (expression *)(iter->elem);
      printAny(temp);
      iter = iter->next;
      if(iter != NULL)
	std::cout << ' ';
    }
    std::cout << ")";
  }else{
    std::cout << "NOT_A_LIST";
  }
}

void printAny(expression * any){
  if(any == NULL){
    std::cout << "NULL" << std::flush;
    return;
  }
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
  case CFUNC_EXP:
    std::cout << "CFUNC" << std::flush;
    break;
  case VAR_EXP:
    printVar(any);
    break;
  case FUNC_EXP:
    printLambda(any);
    break;
  default:
    printStr(any);
    break;
  }
}
