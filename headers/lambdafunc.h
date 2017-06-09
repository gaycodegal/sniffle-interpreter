#ifndef __LAMBDAFUNC_H__
#define __LAMBDAFUNC_H__
#include "lispinclude.h"

/**
   the lambda func data structure
 */
class LambdaFunc{
public:
  /**
     how many things point at this list.
     (to avoid deleting when not needed).
   */
  int refs;

  /**
     the program this will execute when called.
   */
  expression * prog;

  /**
     the arg vars passed into this.
   */
  slist * args;

  /**
     the environment locally held.
   */
  environment * data;

  /**
     the lambda function constructor
   */
  LambdaFunc(slist * args, expression * prog);

  /**
     delete args prog and data
   */
  ~LambdaFunc();

  /**
     run the func.
   */
  expression * exec(expression * arglist, environment * env);
};

#endif
