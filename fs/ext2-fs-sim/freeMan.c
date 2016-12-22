/*Queue - Linked List implementation*/
#include<stdio.h>
#include<stdlib.h>
#include "freeMan.h"

struct Node {
	int block;
	struct Node* next;
};


// To Enqueue
void enqueue(struct queue *q, int b) {
	struct Node* temp = 
		(struct Node*)malloc(sizeof(struct Node));
	temp->block = b; 
	temp->next = NULL;
	if(q->front == NULL && q->rear == NULL){
		q->front = q->rear = temp;
		return;
	}
	q->rear->next = temp;
	q->rear = temp;
}

// To Dequeue.
void dequeue(struct queue *q) {
	struct Node* temp = q->front;
	if(q->front == NULL) {
		return;
	}
	if(q->front == q->rear) {
		q->front = q->rear = NULL;
	}
	else {
		q->front = q->front->next;
	}
	free(temp);
}

int front(struct queue *q) {
	if(q->front == NULL) {
		return 0;
	}
	return q->front->block;
}

int empty(struct queue *q){
    if(q->front == NULL){
        return 1;
    }
    return 0;
}

void print(struct queue *q){
    struct Node* temp = q->front;
    while (temp != NULL){
        printf("%d ", temp->block);
        temp = temp->next;
    }
}


