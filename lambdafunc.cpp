#include "lambdafunc.h"

LambdaFunc::LambdaFunc(slist * args, expression * prog){
  this->args = args;
  this->prog = prog;
  refs = 1;
  data = new environment();
}

expression * LambdaFunc::exec(expression * arglist, environment * env){
  expression * temp, *arg;
  environmentIterator it, end = data->end();

  slist * list = evalList(arglist->data.list, env, data);

  snode * iter = list->head;
  snode * argi = args->head;
  for (; iter != NULL && argi != NULL; iter = iter->next, argi = argi->next) {
    temp = (expression *)(iter->elem);
    arg = (expression *)(argi->elem);
    it = data->find(*(arg->data.str->s));
    if(it != end){
      deleteExpression(it->second);
    }
    (*data)[*(arg->data.str->s)] = temp;
  }
  for (; iter != NULL; iter = iter->next) {
    deleteExpression((expression *)(iter->elem));
  }
  freeList(list);
  return evalAST(prog, env, data);
}

LambdaFunc::~LambdaFunc(){
  expression * temp;
  deleteExpression(prog);
  deleteEnv(data);
  temp = makeList(args);
  deleteExpression(temp);
}
