/***
 * Filename:        freeMan.h
 * Date:            Dec 21 2016
 * Last Edited by:  Gengshan Yang
 * Description:     Free space management
 ***/

/*Define queue struct, for free space management */
typedef struct queue{
    struct Node* front;
    struct Node* rear;
} queue;

void print(struct queue *q);
int empty(struct queue *q);
void dequeue(struct queue *q);
int front(struct queue *q);
void enqueue(struct queue *q,int b);
