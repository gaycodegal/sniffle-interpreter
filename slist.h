#ifndef __SLIST_H__
#define __SLIST_H__

#include <stdlib.h>

typedef struct s_snode {
  void * elem;
  struct s_snode *next;
} snode;

typedef struct s_slist {
  snode * head;
  snode * tail;
  int len;
} slist;

slist * makeSList();

int getLen(slist * list);

snode * makeNode(void * elem);

void addElem(slist * list, void * elem);

void addBackElem(slist * list, void * elem);

void addNode(slist * list, snode * node);

void addBackNode(slist * list, snode * node);

void * carNode(slist * list);

slist * cdrNode(slist * list);

snode * copyNode(snode * node);

slist * copyList(slist * list);

slist * reverseList(slist * list);

void freeList(slist * list);

#endif
