#ifndef __DATASTRUCTURES_H__
#define __DATASTRUCTURES_H__

/**
   the data types an expression can take on.
 */
typedef union s_expdata{
  int num;
  std::string * str;
  std::list<struct s_expression *> * list;
} expdata;

/**
   a general expression
 */
typedef struct s_expression{
  int type;
  expdata data;
} expression;

#endif
