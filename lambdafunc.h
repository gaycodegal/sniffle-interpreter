#ifndef __LAMBDAFUNC_H__
#define __LAMBDAFUNC_H__
#include "lispinclude.h"
class LambdaFunc{
public:
  int refs;
  expression * prog;
  slist * args;
  environment * data;
  LambdaFunc(slist * args, expression * prog);
  expression * exec(expression * arglist, environment * env);
  ~LambdaFunc();
};

#endif
