#include "lispinclude.h"

expression * addFunc(expression * arglist){
  expression * temp;
  
  int sum = 0;
  for (snode * iter = arglist->data.list->head; iter != NULL; iter = iter->next) {
    temp = (expression *)(iter->elem);
    if(temp->type == CONST_EXP){
      sum += temp->data.num;
    }
  }
  return makeInt(sum);
}

expression * evalAST(expression * prog, environment * env, environment *args){

  return NULL;
}

int main(int argc, char ** argv){
  //environment * ENV = new environment();
  std::size_t data_size;
  char * data = readFile(argv[1], data_size);
  expression * prog = parseList(data, data_size);
  //ENV["+"] = makeCFunc(&addFunc);
  printAny(prog);
  deleteExpression(prog);
  ///delete ENV;
  delete [] data;
}
