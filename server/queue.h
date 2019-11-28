#ifndef __QUEUE__
#define __QUEUE__

#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>
  
#define MAX_PATH_LEN 4096

typedef struct _QElem
{
  char path[MAX_PATH_LEN];
} QElem;

typedef struct _QNode 
{ 
    QElem *elem; 
    struct _QNode *next; 
} QNode; 
  
typedef struct _Queue 
{ 
    struct _QNode *front, *rear; 
} Queue; 

QElem* gen_elem(const char *path);
QNode* gen_node(const QElem *elem); 
Queue* gen_queue();
void push(Queue *q, const QElem *elem); 
void pop(Queue *q); 
QElem* front(const Queue *q);
QElem* back(const Queue *q);

#endif
