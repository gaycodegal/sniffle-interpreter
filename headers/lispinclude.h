#ifndef __LISPINCLUDE_H__
#define __LISPINCLUDE_H__

#include <iostream>
#include <stack>
#include <unordered_map>
#include <fstream>
#include "slist.h"
#include "datastructures.h"
#include "printexpression.h"
#include "expressions.h"
#include "filereader.h"
#include "parse.h"
#include "interp.h"
#include "lambdafunc.h"

/**
   the types an expression can take on.
 */
enum EXP_ENUM { LIST_EXP, STR_EXP, VAR_EXP, CONST_EXP, CFUNC_EXP, FUNC_EXP, PTR_EXP };

#endif
