#include "lispinclude.h"

expression * addFunc(expression * arglist, environment * env, environment * args){
  expression * temp;
  slist * list = evalList(arglist->data.list, env, args);
  int sum = 0;
  for (snode * iter = list->head; iter != NULL; iter = iter->next) {
    temp = (expression *)(iter->elem);
    if(temp->type == CONST_EXP){
      sum += temp->data.num;
    }
  }
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
	return it->second;
	break;
      }
    }
    if(env != NULL){
      it = env->find(*(prog->data.str));
      if(it != env->end()){
	return it->second;
	break;
      }
    }
    return NULL;
    break;
  case LIST_EXP:
    temp = (expression *)carNode(prog->data.list);
    temp = evalAST(temp, env, args);
    if(temp != NULL){
      tlist = cdrNode(prog->data.list);
      temp2 = makeList(tlist);
      /*for(iter = tlist->head; iter != NULL; iter = iter->next){
	iter->elem = (void *)evalAst((expression *)(iter->elem), env, args);
	}
      */
      if(temp->type == CFUNC_EXP){
	ret = temp->data.c_func(temp2, env, args);
      }
      free(tlist);
      temp2->data.list = NULL;
      /*deleteExpression(temp);*/
      deleteExpression(temp2);
      return ret;
    }
    break;
  default:
    return prog;
    break;
  }
  return NULL;
}

int main(int argc, char ** argv){
  environment * ENV = new environment();
  std::size_t data_size;
  char * data = readFile(argv[1], data_size);
  expression * prog = parseList(data, data_size);
  snode * iter;
  expression * temp;
  (*ENV)["+"] = makeCFunc(&addFunc);
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
  delete ENV;
  delete [] data;
}
