#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>

// Структура элемента очереди с приоритетами
typedef struct PriorityQueueNode{
    int vertex;
    unsigned int distance;
} PriorityQueueNode;

// Структура очереди с приоритетами
typedef struct PriorityQueue{
    PriorityQueueNode* nodes;
    int size;
    int capacity;
} PriorityQueue;

PriorityQueue* createPriorityQueue(int capacity) {
    PriorityQueue* pq = (PriorityQueue*)malloc(sizeof(PriorityQueue));
    pq->nodes = (PriorityQueueNode*)malloc(capacity * sizeof(PriorityQueueNode));
    pq->size = 0;
    pq->capacity = capacity;
    return pq;
}

void swap(PriorityQueueNode* a, PriorityQueueNode* b) {
    PriorityQueueNode temp = *a;
    *a = *b;
    *b = temp;
}

void heapifyUp(PriorityQueue* pq, int index) {
    while (index > 0 && pq->nodes[index].distance < pq->nodes[(index - 1) / 2].distance) {
        swap(&pq->nodes[index], &pq->nodes[(index - 1) / 2]);
        index = (index - 1) / 2;
    }
}

void heapifyDown(PriorityQueue* pq, int index) {
    int smallest = index;
    int left = 2 * index + 1;
    int right = 2 * index + 2;

    if (left < pq->size && pq->nodes[left].distance < pq->nodes[smallest].distance) {
        smallest = left;
    }

    if (right < pq->size && pq->nodes[right].distance < pq->nodes[smallest].distance) {
        smallest = right;
    }

    if (smallest != index) {
        swap(&pq->nodes[index], &pq->nodes[smallest]);
        heapifyDown(pq, smallest);
    }
}

void push(PriorityQueue* pq, int vertex, unsigned int distance) {
    if (pq->size >= pq->capacity) return;
    pq->nodes[pq->size].vertex = vertex;
    pq->nodes[pq->size].distance = distance;
    pq->size++;
    heapifyUp(pq, pq->size - 1);
}

PriorityQueueNode pop(PriorityQueue* pq) {
    PriorityQueueNode root = pq->nodes[0];
    pq->nodes[0] = pq->nodes[pq->size - 1];
    pq->size--;
    heapifyDown(pq, 0);
    return root;
}

int isEmpty(PriorityQueue* pq) {
    return pq->size == 0;
}

void freeQueue(PriorityQueue* queue){
    free(queue->nodes);
    free(queue);
}
