#ifndef __PARSE_H__
#define __PARSE_H__

#include "lispinclude.h"

inline void pushStr(bool & inputhandled, expression * parent, char * data, const std::size_t i, const std::size_t lastpos, int type);

std::size_t seekEndOfStr(char * str);

inline void pushInt(expression * parent, char * data, std::size_t & i);

expression * parseList(char * data, std::size_t size);

#endif
