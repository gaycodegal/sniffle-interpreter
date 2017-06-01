#ifndef __PARSE_H__
#define __PARSE_H__

#include "lispinclude.h"

/**
   push a string into parent
 */
inline void pushStr(bool & inputhandled, expression * parent, char * data, const std::size_t i, const std::size_t lastpos, int type);

/**
   find the end of a string and return its len
 */
std::size_t seekEndOfStr(char * str);

/**
   push an int onto the parent.
 */
inline void pushInt(expression * parent, char * data, std::size_t & i);

/**
   parse a list.
 */
expression * parseList(char * data, std::size_t size);

#endif
