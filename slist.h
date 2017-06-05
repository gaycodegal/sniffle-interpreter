#ifndef __SLIST_H__
#define __SLIST_H__

#include <stdlib.h>

/**
   the node struct used in the linked list
 */
typedef struct s_snode {
  void * elem;
  struct s_snode *next;
} snode;

/**
   the linked list
 */
typedef struct s_slist {
  snode * head;
  snode * tail;
  int len;
  int refs;
} slist;

/**
   create a new linked list
 */
slist * makeSList();

/**
   make a new list node
 */
snode * makeNode(void * elem);

/**
   add an elemenent to the front of the list
 */
void addElem(slist * list, void * elem);

/**
add an element to the back of the list
 */
void addBackElem(slist * list, void * elem);

/**
add a node to the front of the list
 */
void addNode(slist * list, snode * node);

/**
add a node to the back of the list
 */
void addBackNode(slist * list, snode * node);

/**
   returns the front elem of the list
 */
void * carNode(slist * list);

/**
   create a shallow new list pointing to the the list without it's first elem.
   
   if len == 0 returns null
 */
slist * cdrNode(slist * list);

/**
   create a copy of a node
 */
snode * copyNode(snode * node);

/**
   create a copy of the list.
 */
slist * copyList(slist * list);

/**
   create a copy of the list that is reversed.
 */
slist * reverseList(slist * list);

/**
   free a list (not what it holds though).
 */
void freeList(slist * list);

#endif
