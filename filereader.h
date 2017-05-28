#ifndef __FILEREADER_H__
#define __FILEREADER_H__

#include "lispinclude.h"

/**
   read in a file, return a string and it's length
 */
char * readFile(char * file, std::size_t & size);

#endif
