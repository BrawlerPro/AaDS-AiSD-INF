#include <stdio.h>
#include <stdlib.h>

//Очередь Список
typedef struct Node {
    double data;
    int angle;
    struct Node *next;
} Node;

typedef struct {
    Node *front;
    Node *rear;
    int size;
} Queue;

// Инициализация очереди
Queue *createQueue() {
    Queue *queue = (Queue *) malloc(sizeof(Queue));
    if (!queue) return NULL;
    queue->front = NULL;
    queue->rear = NULL;
    return queue;
}

// Проверка на пустоту
int isEmpty(Queue* queue) {
    return (queue->front == NULL);
}

// Добавление элемента в очередь
int enqueue(Queue *queue, double item, int angle) {
    Node *newNode = (Node *) malloc(sizeof(Node));
    if (!newNode) return 1;
    newNode->data = item;
    newNode->angle = angle;
    newNode->next = NULL;
    if (isEmpty(queue)) {
        queue->front = newNode;
        queue->rear = newNode;
        newNode->next = newNode;
    } else {
        newNode->next = queue->front;
        queue->rear->next = newNode;
        queue->rear = newNode;
    }
    return 0;
}

// Удаление элемента из очереди
int dequeue(Queue *queue, double* item, int* angle) {
    if(isEmpty(queue)) return 1;
    if (queue->front == queue->rear) {
        (*item) = queue->front->data;
        (*angle) = queue->front->angle;
        free(queue->front);
        queue->front = NULL;
        queue->rear = NULL;
    } else {
        Node *temp = queue->front;
        (*item) = temp->data;
        (*angle) = temp->angle;
        queue->front = temp->next;
        queue->rear->next = queue->front;
        free(temp);
    }
    return 0;
}

// Распустить Очередь
void disperseQueue(Queue *queue) {
    double data;
    int angle;
    while (!dequeue(queue, &data, &angle));
    free(queue);
}