#include "lispinclude.h"
#include <iostream>
#include <string>
#include <sstream>

#define SNIFFLE_MAJOR 0
#define SNIFFLE_MINOR 1
#define SNIFFLE_SUB 1

int replrunning = 0;

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
  if(!(type == VAR_EXP || type == STR_EXP))
    return NULL;
  arg = evalAST((expression *)(temp->next->elem), env, args);
  environmentIterator it = env->find(*(var->data.str->s)), end = env->end();
  if(end != it){
    deleteExpression(it->second);
  }
  (*env)[*(var->data.str->s)] = arg;
  return NULL;
}

expression * quoteFunc(expression * arglist, environment * env, environment * args){
  slist * list = arglist->data.list;
  if(list->len != 1)
    return NULL;
  return trueCopyExpression((expression *)(list->head->elem));
}

expression * copyFunc(expression * arglist, environment * env, environment * args){
  slist * list = arglist->data.list;
  expression * temp, *ret;
  if(list->len != 1)
    return NULL;
  temp = evalAST((expression *)(list->head->elem), env, args);
  ret = trueCopyExpression(temp);
  deleteExpression(temp);
  return ret;
}

expression * carFunc(expression * arglist, environment * env, environment * args){
  slist * list = arglist->data.list;
  expression * temp, *ret;
  if(list->len != 1)
    return NULL;
  temp = evalAST((expression *)(list->head->elem), env, args);
  if(temp->type != LIST_EXP || temp->data.list->len <= 1){
    deleteExpression(temp);
    return NULL;
  }
  ret = copyExpression((expression *)(temp->data.list->head->elem));
  deleteExpression(temp);
  return ret;
}

expression * cdrFunc(expression * arglist, environment * env, environment * args){
  slist * list = arglist->data.list;
  snode * tnode;
  expression * temp, *ret;
  if(list->len != 1)
    return NULL;
  ret = evalAST((expression *)(list->head->elem), env, args);
  if(ret->type != LIST_EXP || ret->data.list->len <= 0){
    deleteExpression(ret);
    return NULL;
  }
  list = ret->data.list;
  tnode = list->head;
  temp = (expression *)(tnode->elem);
  deleteExpression(temp);
  list->head = tnode->next;
  free(tnode);
  --(list->len);
  return ret;
}

expression * letFunc(expression * arglist, environment * env, environment * args){
  slist * list = arglist->data.list, *dlist, *vlist;
  snode * tnode;
  expression * temp, *defs;
  environment * letenv;
  if(list->len != 2)
    return NULL;
  defs = (expression *)(list->head->elem);
  if(defs->type != LIST_EXP){
    return NULL;
  }
  dlist = defs->data.list;
  for(tnode = dlist->head; tnode != NULL; tnode = tnode->next){
    temp = (expression *)(tnode->elem);
    if(temp->type != LIST_EXP)
      return NULL;
    if(((expression *)(temp->data.list->head->elem))->type != VAR_EXP)
      return NULL;
  }
  letenv = new environment();
  for(tnode = dlist->head; tnode != NULL; tnode = tnode->next){
    temp = (expression *)(tnode->elem);
    vlist = temp->data.list;
    (*letenv)[*(((expression *)(vlist->head->elem))->data.str->s)] = evalAST((expression *)(vlist->head->next->elem), env, args);
  }
  temp = evalAST((expression *)(list->head->next->elem),env,letenv);
  deleteEnv(letenv);
  return temp;
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
  if(!(type == VAR_EXP || type == STR_EXP))
    return NULL;
  arg = evalAST((expression *)(temp->next->elem), env, args);
  environmentIterator it = args->find(*(var->data.str->s)), end = args->end();
  if(end != it){
    deleteExpression(it->second);
  }
  (*args)[*(var->data.str->s)] = arg;
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

expression * beginFunc(expression * arglist, environment * env, environment * args){
  expression * temp = NULL;
  slist * list = arglist->data.list;
  for (snode * iter = list->head; iter != NULL;) {
    temp = evalAST((expression *)(iter->elem), env, args);
    iter = iter->next;
    if(iter != NULL)
      deleteExpression(temp);
  }
  return temp;
}

expression * exitFunc(expression * arglist, environment * env, environment * args){
  replrunning = 0;
  return NULL;
}

expression * whileFunc(expression * arglist, environment * env, environment * args){
  expression * cond = NULL, *ret = NULL;
  slist * list = arglist->data.list;
  snode * iter;
  while (true){
    iter = list->head;
    cond = evalAST((expression *)(iter->elem), env, args);
    if(cond == NULL || (cond->type == CONST_EXP && cond->data.num == 0)){
      deleteExpression(cond);
      break;
    }
    deleteExpression(ret);
    for (iter = iter->next; iter != NULL;) {
      ret = evalAST((expression *)(iter->elem), env, args);
      iter = iter->next;
      if(iter != NULL)
	deleteExpression(ret);
    }
  }
  return ret;
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

expression * eqFunc(expression * arglist, environment * env, environment * args){
  expression * a, * b;
  slist * list = arglist->data.list;
  snode * head;
  int sum = 0, type;
  if(list->len != 2)
    return NULL;
  list = evalList(list, env, args);
  head = list->head;
  a = (expression *)(head->elem);
  b = (expression *)(head->next->elem);
  type = a->type;
  if(a->type == b->type){
    switch(type){
    case CONST_EXP:
      if(a->data.num == b->data.num)
	sum = 1;
      break;
    case STR_EXP:
      if(*(a->data.str->s) == *(b->data.str->s))
	sum = 1;
      break;
    }
    
  }
  
  deleteExpression(a);
  deleteExpression(b);
  
  freeList(list);
  return makeInt(sum);
}

expression * gtFunc(expression * arglist, environment * env, environment * args){
  expression * a, * b;
  slist * list = arglist->data.list;
  snode * head;
  int sum = 0;
  if(list->len != 2)
    return NULL;
  list = evalList(list, env, args);
  head = list->head;
  a = (expression *)(head->elem);
  b = (expression *)(head->next->elem);

  if(a->type == CONST_EXP && b->type == CONST_EXP){
    if(a->data.num > b->data.num)
      sum = 1;
  }
  
  deleteExpression(a);
  deleteExpression(b);
  
  freeList(list);
  return makeInt(sum);
}

expression * geFunc(expression * arglist, environment * env, environment * args){
  expression * a, * b;
  slist * list = arglist->data.list;
  snode * head;
  int sum = 0;
  if(list->len != 2)
    return NULL;
  list = evalList(list, env, args);
  head = list->head;
  a = (expression *)(head->elem);
  b = (expression *)(head->next->elem);

  if(a->type == CONST_EXP && b->type == CONST_EXP){
    if(a->data.num >= b->data.num)
      sum = 1;
  }
  
  deleteExpression(a);
  deleteExpression(b);
  
  freeList(list);
  return makeInt(sum);
}

expression * ltFunc(expression * arglist, environment * env, environment * args){
  expression * a, * b;
  slist * list = arglist->data.list;
  snode * head;
  int sum = 0;
  if(list->len != 2)
    return NULL;
  list = evalList(list, env, args);
  head = list->head;
  a = (expression *)(head->elem);
  b = (expression *)(head->next->elem);

  if(a->type == CONST_EXP && b->type == CONST_EXP){
    if(a->data.num < b->data.num)
      sum = 1;
  }
  
  deleteExpression(a);
  deleteExpression(b);
  
  freeList(list);
  return makeInt(sum);
}

expression * leFunc(expression * arglist, environment * env, environment * args){
  expression * a, * b;
  slist * list = arglist->data.list;
  snode * head;
  int sum = 0;
  if(list->len != 2)
    return NULL;
  list = evalList(list, env, args);
  head = list->head;
  a = (expression *)(head->elem);
  b = (expression *)(head->next->elem);

  if(a->type == CONST_EXP && b->type == CONST_EXP){
    if(a->data.num <= b->data.num)
      sum = 1;
  }
  
  deleteExpression(a);
  deleteExpression(b);
  
  freeList(list);
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

  if(list->len == 1)
    first = false;
  
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
      it = args->find(*(prog->data.str->s));
      if(it != args->end()){
	return copyExpression(it->second);
	break;
      }
    }
    if(env != NULL){
      it = env->find(*(prog->data.str->s));
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

int countParens(const char * p){
  char c;
  int parens = 0;
  int i = 0;
  while((c = p[i++])){
    if(c == '(')
      ++parens;
    else if(c == ')')
      --parens;
  }
  return parens;
}

void accquireAndGo(environment * env){
  int parens = 0;
  std::stringstream s;
  std::string inputstr;
  do{
    std::getline(std::cin, inputstr);
    parens += countParens(inputstr.c_str());
    s << inputstr << "\n";
  }while(parens != 0);
  expression * prog = parseList((char *)s.str().c_str(), s.str().size()), *temp;
  for(snode *iter = prog->data.list->head; iter != NULL; iter = iter->next){
    temp = (expression *)(iter->elem);
    temp = evalAST(temp, env, NULL);
    printAny(temp);
    std::cout << std::endl;
    deleteExpression(temp);
  }
  deleteExpression(prog);
}

void repl(environment * env){
  std::cout << "launching sniffle repl v" << SNIFFLE_MAJOR << "." << SNIFFLE_MINOR << "." << SNIFFLE_SUB << std::endl;
  replrunning = 1;
  while(replrunning){
    accquireAndGo(env);
  }
}

int main(int argc, char ** argv){
  environment * ENV = new environment();
  std::size_t data_size;
  char * data;
  expression * prog;
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
  (*ENV)["while"] = makeCFunc(&whileFunc);
  (*ENV)["begin"] = makeCFunc(&beginFunc);
  (*ENV)["pprint"] = makeCFunc(&printAnyFunc);
  (*ENV)[">"] = makeCFunc(&gtFunc);
  (*ENV)[">="] = makeCFunc(&geFunc);
  (*ENV)["<"] = makeCFunc(&ltFunc);
  (*ENV)["<="] = makeCFunc(&leFunc);
  (*ENV)["="] = makeCFunc(&eqFunc);
  (*ENV)["if"] = makeCFunc(&ifFunc);
  (*ENV)["exit"] = makeCFunc(&exitFunc);
  (*ENV)["quote"] = makeCFunc(&quoteFunc);
  (*ENV)["car"] = makeCFunc(&carFunc);
  (*ENV)["cdr"] = makeCFunc(&cdrFunc);
  (*ENV)["copy"] = makeCFunc(&copyFunc);
  (*ENV)["let"] = makeCFunc(&letFunc);

  if(argc == 2){
    data = readFile(argv[1], data_size);
    prog = parseList(data, data_size);
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
    delete [] data;
  }
  repl(ENV);
  deleteEnv(ENV);
}
