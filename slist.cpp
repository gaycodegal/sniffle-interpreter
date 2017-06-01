#include "slist.h"

slist * makeSList(){
  slist * list = (slist *)malloc(sizeof(slist));
  list->head = NULL;
  list->tail = NULL;
  list->len = 0;
  return list;
}

int getLen(slist * list){
  return list->len;
}

snode * makeNode(void * elem){
  snode *node = (snode *)malloc(sizeof(snode));
  node->next = NULL;
  node->elem = elem;
  return node;
}

void addElem(slist * list, void * elem){
  snode * node;
  node = makeNode(elem);
  if(list->len == 0){
    list->tail = node;
  }else{
    node->next = list->head;
  }
  list->head = node;
  ++(list->len);
}

void addNode(slist * list, snode * node){
  if(list->len == 0){
    list->tail = node;
    node->next = NULL;
  }else{
    node->next = list->head;
  }
  list->head = node;
  ++(list->len);
}

void addBackElem(slist * list, void * elem){
  snode * node;
  node = makeNode(elem);
  if(list->len == 0){
    list->head = node;
    list->tail = node;
  }else{
    node->next = NULL;
    list->tail->next = node;
    list->tail = node;
  }
  ++(list->len);
}

void addBackNode(slist * list, snode * node){
  if(list->len == 0){
    list->head = node;
    list->tail = node;
  }else{
    node->next = NULL;
    list->tail->next = node;
    list->tail = node;
  }
  ++(list->len);
}

void * carNode(slist * list){
  if(list->len == 0) return NULL;
  return list->head->elem;
}

slist * cdrNode(slist * list){
  slist * copy;
  if(list->len == 0) return NULL;
  copy = makeSList();
  if(list->len == 1) return copy;
  copy->len = list->len - 1;
  copy->head = list->head->next;
  copy->tail = list->tail;
  return copy;
}

snode * copyNode(snode * node){
  snode * copy = makeNode(node->elem);
  copy->next = node->next;
  return copy;
}

slist * copyList(slist * list){
  slist * copy = makeSList();
  snode * curr, *next;
  if(list->len == 0)
    return copy;
  curr = copyNode(list->head);
  copy->len = list->len;
  copy->head = curr; 
  next = curr->next;
  while(next != NULL){
    next = copyNode(next);
    curr->next = next;
    curr = next;
    next = curr->next;
  }
  copy->tail = curr;
  return copy;
}

slist * reverseList(slist * list){
  slist * copy = makeSList();
  snode * curr = copyNode(list->head), *temp, *next;
  copy->len = list->len;
  copy->tail = curr; 
  next = curr->next;
  curr->next = NULL;
  while(next != NULL){
    temp = next->next;
    next = copyNode(next);
    next->next = curr;
    curr = next;
    next = temp;
  }
  copy->head = curr;
  return copy;
}

void freeList(slist * list){
  snode * head = list->head;
  snode * temp;
  free(list);
  if(list->len == 0) return;
  while(head != NULL){
    temp = head;
    head = temp->next;
    free(temp);
  }
}
