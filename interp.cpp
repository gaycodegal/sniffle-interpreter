#include "lispinclude.h"

expression * printAnyFunc(expression * arglist, environment * env, environment * args){
  expression * temp;
  slist * list = evalList(arglist->data.list, env, args);
  for (snode * iter = list->head; iter != NULL; ) {
    temp = (expression *)(iter->elem);
    printAny(temp);
    iter = iter->next;
    if(iter != NULL)
      std::cout << " ";
    deleteExpression(temp);
  }
  std::cout << std::endl;
  freeList(list);
  return NULL;
}

expression * setFunc(expression * arglist, environment * env, environment * args){
  expression * var, *arg;
  slist * list = arglist->data.list;
  snode * temp = list->head;
  int type;
  if(list->len < 2)
    return NULL;
  var = (expression *)(temp->elem);
  if(var == NULL)
    return NULL;
  type = var->type;
  if(!(type == VAR_EXP || type == SYM_EXP || type == STR_EXP))
    return NULL;
  arg = evalAST((expression *)(temp->next->elem), env, args);
  environmentIterator it = env->find(*(var->data.str)), end = env->end();
  if(end != it){
    deleteExpression(it->second);
  }
  (*env)[*(var->data.str)] = arg;
  return NULL;
}

expression * setFunc2(expression * arglist, environment * env, environment * args){
  expression * var, *arg;
  slist * list = arglist->data.list;
  snode * temp = list->head;
  int type;
  if(list->len < 2)
    return NULL;
  var = (expression *)(temp->elem);
  if(var == NULL)
    return NULL;
  type = var->type;
  if(!(type == VAR_EXP || type == SYM_EXP || type == STR_EXP))
    return NULL;
  arg = evalAST((expression *)(temp->next->elem), env, args);
  environmentIterator it = args->find(*(var->data.str)), end = args->end();
  if(end != it){
    deleteExpression(it->second);
  }
  (*args)[*(var->data.str)] = arg;
  return NULL;
}


expression * lambdaMakerFunc(expression * arglist, environment * env, environment * args){
  expression * expr, * lamargs;
  slist * list = arglist->data.list;
  snode * temp = list->head, *iter;
  LambdaFunc * func;
  lamargs = (expression *)(temp->elem);
  if(lamargs->type != LIST_EXP)
    return NULL;
  for(iter = lamargs->data.list->head; iter != NULL; iter = iter->next){
    expr = (expression*)(iter->elem);
    if(expr == NULL || expr->type != VAR_EXP)
      return NULL;
  }
  lamargs = copyExpression(lamargs);
  func = new LambdaFunc(lamargs->data.list, copyExpression((expression *)(temp->next->elem)));
  lamargs->data.list = NULL;
  deleteExpression(lamargs);
  return makeLambda(func);
}

expression * orFunc(expression * arglist, environment * env, environment * args){
  expression * temp;
  slist * list = arglist->data.list;
  int sum = 0;
  for (snode * iter = list->head; sum == 0 && iter != NULL; iter = iter->next) {
    temp = evalAST((expression *)(iter->elem), env, args);
    if(temp != NULL){
      if(temp->type == CONST_EXP){
	sum = sum || temp->data.num;
      }else{
	sum = 1;
      }
    }
    deleteExpression(temp);
  }
  return makeInt(sum);
}

expression * ifFunc(expression * arglist, environment * env, environment * args){
  expression * temp;
  slist * list = arglist->data.list;
  snode * iter = list->head;
  int dofirst = 0;
  if(list->len < 3)
    return NULL;
  temp = evalAST((expression *)(iter->elem), env, args);
  iter = iter->next;
  if(temp != NULL){
    if(temp->type == CONST_EXP){
      dofirst = temp->data.num;
    }else{
      dofirst = 1;
    }
  }else{
    dofirst = 0;
  }
  deleteExpression(temp);
  if(dofirst == 0){
    iter = iter->next;
  }
  return evalAST((expression *)(iter->elem), env, args);
}

expression * andFunc(expression * arglist, environment * env, environment * args){
  expression * temp;
  slist * list = arglist->data.list;
  int sum = 1;
  for (snode * iter = list->head; sum == 1 && iter != NULL; iter = iter->next) {
    temp = evalAST((expression *)(iter->elem), env, args);
    if(temp != NULL){
      if(temp->type == CONST_EXP){
	sum = sum || temp->data.num;
      }else{
	sum = 1;
      }
    }else{
      sum = 0;
    }
    deleteExpression(temp);
  }
  return makeInt(sum);
}

expression * addFunc(expression * arglist, environment * env, environment * args){
  expression * temp;
  slist * list = evalList(arglist->data.list, env, args);
  int sum = 0;
  for (snode * iter = list->head; iter != NULL; iter = iter->next) {
    temp = (expression *)(iter->elem);
    if(temp != NULL && temp->type == CONST_EXP){
      sum += temp->data.num;
    }
    deleteExpression(temp);
  }
  freeList(list);
  return makeInt(sum);
}

expression * multiplyFunc(expression * arglist, environment * env, environment * args){
  expression * temp;
  slist * list = evalList(arglist->data.list, env, args);
  int sum = 1;
  for (snode * iter = list->head; iter != NULL; iter = iter->next) {
    temp = (expression *)(iter->elem);
    if(temp != NULL && temp->type == CONST_EXP){
      sum *= temp->data.num;
    }
    deleteExpression(temp);
  }
  freeList(list);
  return makeInt(sum);
}

expression * subtractFunc(expression * arglist, environment * env, environment * args){
  expression * temp;
  slist * list = evalList(arglist->data.list, env, args);
  int sum = 0;
  bool first = true;
  for (snode * iter = list->head; iter != NULL; iter = iter->next) {
    temp = (expression *)(iter->elem);
    if(temp != NULL && temp->type == CONST_EXP){
      if(first){
	first = false;
	sum += temp->data.num;
      }else{
	sum -= temp->data.num;
      }
    }
    deleteExpression(temp);
  }
  freeList(list);
  return makeInt(sum);
}

expression * divideFunc(expression * arglist, environment * env, environment * args){
  expression * temp;
  slist * list = evalList(arglist->data.list, env, args);
  int sum = 1;
  bool first = true;
  bool failed = false;
  int num = 0;
  for (snode * iter = list->head; iter != NULL; iter = iter->next) {
    temp = (expression *)(iter->elem);
    if(temp != NULL && temp->type == CONST_EXP){
      num = temp->data.num;
      if(first){
	first = false;
	sum *= num;
      }else if(num != 0){
	sum /= temp->data.num;
      }else{
	failed = true;
      }
    }
    deleteExpression(temp);
  }
  freeList(list);
  if(failed)
    return NULL;
  return makeInt(sum);
}

slist * evalList(slist * list, environment * env, environment * args){
  slist *copy = copyList(list);
  for(snode *iter = copy->head; iter != NULL; iter = iter->next){
    iter->elem = (void *)evalAST((expression *)iter->elem, env, args);
  }
  return copy;
}

expression * evalAST(expression * prog, environment * env, environment *args){
  expression * temp, *temp2, *ret;
  environmentIterator it;
  slist * tlist;
  if(prog == NULL) return NULL;
  switch(prog->type){
  case VAR_EXP:
    if(args != NULL){
      it = args->find(*(prog->data.str));
      if(it != args->end()){
	return copyExpression(it->second);
	break;
      }
    }
    if(env != NULL){
      it = env->find(*(prog->data.str));
      if(it != env->end()){
	return copyExpression(it->second);
	break;
      }
    }
    return NULL;
    break;
  case LIST_EXP:
    temp = (expression *)carNode(prog->data.list);
    if(temp == NULL)
      break;
    temp = evalAST(temp, env, args);
    if(temp != NULL){
      tlist = cdrNode(prog->data.list);
      temp2 = makeList(tlist);
      if(temp->type == CFUNC_EXP){
	ret = temp->data.c_func(temp2, env, args);
      }else if(temp->type == FUNC_EXP){
	ret = temp->data.func->exec(temp2, env);
      }else{
	ret = NULL;
      }
      free(tlist);
      temp2->data.list = NULL;
      deleteExpression(temp2);
      deleteExpression(temp);
      return ret;
    }
    break;
  default:
    return copyExpression(prog);
    break;
  }
  return NULL;
}

void deleteEnv(environment * env){
  environmentIterator it, end;
  for(it = env->begin(), end = env->end(); it != end; ++it){
    deleteExpression(it->second);
  }
  delete env;
}

int main(int argc, char ** argv){
  environment * ENV = new environment();
  std::size_t data_size;
  char * data = readFile(argv[1], data_size);
  expression * prog = parseList(data, data_size);
  snode * iter;
  expression * temp;
  (*ENV)["+"] = makeCFunc(&addFunc);
  (*ENV)["*"] = makeCFunc(&multiplyFunc);
  (*ENV)["-"] = makeCFunc(&subtractFunc);
  (*ENV)["/"] = makeCFunc(&divideFunc);
  (*ENV)["or"] = makeCFunc(&orFunc);
  (*ENV)["and"] = makeCFunc(&andFunc);
  (*ENV)["set"] = makeCFunc(&setFunc);
  (*ENV)["local"] = makeCFunc(&setFunc2);
  (*ENV)["lambda"] = makeCFunc(&lambdaMakerFunc);
  (*ENV)["pprint"] = makeCFunc(&printAnyFunc);
  (*ENV)["if"] = makeCFunc(&ifFunc);
  for(iter = prog->data.list->head; iter != NULL; iter = iter->next){
    temp = (expression *)(iter->elem);
    printAny(temp);
    std::cout << std::endl;
    temp = evalAST(temp, ENV, NULL);
    printAny(temp);
    std::cout << std::endl;
    deleteExpression(temp);
  }
  deleteExpression(prog);
  deleteEnv(ENV);
  delete [] data;
}
