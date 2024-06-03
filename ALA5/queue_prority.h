#ifndef SKIBIDITOILET_QUEUE_PRORITY_H
#define SKIBIDITOILET_QUEUE_PRORITY_H

typedef struct {
    int vertex;
    unsigned int distance;
} PriorityQueueNode;

typedef struct PriorityQueue PriorityQueue;

PriorityQueue* createPriorityQueue(int capacity);
void push(PriorityQueue* pq, int vertex, int distance);
PriorityQueueNode pop(PriorityQueue* pq);
int isEmpty(PriorityQueue* pq);
void freeQueue(PriorityQueue* queue);

#endif //SKIBIDITOILET_QUEUE_PRORITY_H
