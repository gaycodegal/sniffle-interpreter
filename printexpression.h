#ifndef __PRINTEXPRESSION_H__
#define __PRINTEXPRESSION_H__

#include "lispinclude.h"

/**
   print out a var exp
 */
void printVar(expression * exp);

/**
   print out a const exp
 */
void printConst(expression * exp);

/**
   print out a sym exp
 */
void printSym(expression * exp);

/**
   print out a str exp
 */
void printStr(expression * exp);

/**
   print out a list exp
 */
void printList(expression * exp);

/**
   print out any expression
 */
void printAny(expression * any);

#endif
