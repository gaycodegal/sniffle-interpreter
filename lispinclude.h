#ifndef __LISPINCLUDE_H__
#define __LISPINCLUDE_H__

#include <iostream>
#include <stack>
#include <unordered_map>
#include <fstream>
#include <list>
#include "slist.h"
#include "datastructures.h"
#include "printexpression.h"
#include "expressions.h"
#include "filereader.h"
#include "parse.h"

enum EXP_ENUM { LIST_EXP, STR_EXP, VAR_EXP, CONST_EXP, SYM_EXP, ENV_EXP, CFUNC_EXP };

#endif
