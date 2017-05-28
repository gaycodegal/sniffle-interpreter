#ifndef __EXPRESSIONS_H__
#define __EXPRESSIONS_H__

#include "lispinclude.h"

/**
   take an expression and initialize it as a list exp
 */
expression * makeList(expression * exp);

/**
   make a string expression
 */
expression * makeString(char * ptr, std::size_t length);

/**
   make an int expression
 */
expression * makeInt(char * ptr, int length);

/**
   delete any expression
 */
void deleteExpression(expression * any);

#endif
