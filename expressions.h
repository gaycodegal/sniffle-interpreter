#ifndef __EXPRESSIONS_H__
#define __EXPRESSIONS_H__

#include "lispinclude.h"

/**
   take an expression and initialize it as a list exp
 */
expression * makeList(expression * exp);

/**
   make a list expression
 */
expression * makeList(slist * list);

/**
   make a lambda expression
 */
expression * makeLambda(LambdaFunc * lambda);

/**
   make a string expression
 */
expression * makeString(char * ptr, std::size_t length);

/**
   make an int expression
 */
expression * makeInt(char * ptr, int length);

/**
   make an int expression given an int
 */
expression * makeInt(const int num);

/**
   make a c function expression from a c fuction
 */
expression * makeCFunc(cfunc fn);
  
/**
   delete any expression
 */
void deleteExpression(expression * any);

/**
   copy any expression
 */
expression * copyExpression(expression * any);

#endif
