#include "queue.h"  

QElem* gen_elem(const char *path)
{
    QElem *new_elem = malloc(sizeof(QElem));
    memset(new_elem->path, 0, sizeof(char));
    strcpy(new_elem->path, path);
    return new_elem;
}

// A utility function to create a new linked list node. 
QNode* gen_node(const QElem *elem) 
{ 
    QNode *new_node = malloc(sizeof(QNode)); 
    new_node->elem = elem; 
    new_node->next = NULL; 
    return new_node; 
} 
  
// A utility function to create an empty queue 
Queue* gen_queue() 
{ 
    Queue *q = malloc(sizeof(Queue)); 
    q->front = q->rear = NULL; 
    return q; 
} 
  
// The function to add a key k to q 
void push(Queue *q, const QElem *elem) 
{ 
    // Create a new LL node 
    QNode *new_node = gen_node(elem); 
  
    // If queue is empty, then new node is front and rear both 
    if (q->rear == NULL) 
    { 
        q->front = q->rear = new_node; 
        return; 
    } 
  
    // Add the new node at the end of queue and change rear 
    q->rear->next = new_node; 
    q->rear = new_node; 
} 
  
// Function to remove a key from given queue q 
void pop(Queue *q) 
{ 
    // If queue is empty, return NULL. 
    if (q->front == NULL) 
        return; 
  
    // Store previous front and move front one node ahead 
    QNode *front_node = q->front; 
    free(front_node); 
  
    q->front = q->front->next; 
  
    // If front becomes NULL, then change rear also as NULL 
    if (q->front == NULL) 
        q->rear = NULL; 
} 

void destroy_q(Queue *q)
{
  while (!is_empty(q))
    pop(q);
}

void print_q(Queue *q)
{
  QNode *cur_node;
  for (cur_node = q->front; 
       cur_node->next; 
       cur_node = cur_node->next)
    printf("[%s]\n",cur_node->elem->path);

  printf("\n");
}

bool is_empty(Queue *q)
{
  return (q->front==NULL) ? true : false;
}

QElem* front(const Queue *q)
{
    return q->front->elem;
}

QElem* back(const Queue *q)
{
    return q->rear->elem;
}
