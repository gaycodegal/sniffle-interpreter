#ifndef __DATASTRUCTURES_H__
#define __DATASTRUCTURES_H__

#include "lispinclude.h"

/**
   the current type for an environment.
 */
typedef std::unordered_map<std::string, struct s_expression*> environment;

/**
   the current type for an environment iterator.
 */
typedef std::unordered_map<std::string, struct s_expression*>::iterator environmentIterator;

/**
   the type of any funciton that can be bound as a c function
 */
typedef struct s_expression * (* cfunc)(struct s_expression * exp, environment * env, environment * args);

/**
   to prevent loading issues.
 */
class LambdaFunc;

typedef struct s_str{
  std::string * s;
  int refs;
} sstr;

/**
   the data types an expression can take on.
 */
typedef union u_expdata {
  int num;
  environment * env;
  sstr * str;
  slist * list;
  cfunc c_func;
  LambdaFunc * func;
} expdata;

/**
   a general expression
 */
typedef struct s_expression {
  int type;
  expdata data;
} expression;

#endif
