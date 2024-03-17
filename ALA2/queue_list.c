#include <stdio.h>
#include <stdlib.h>

//Очередь Список
typedef struct Node {
    double data;
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
int enqueue(Queue *queue, double item) {
    Node *newNode = (Node *) malloc(sizeof(Node));
    if (!newNode) return 1;
    newNode->data = item;
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
double dequeue(Queue *queue) {
    double item;
    if (queue->front == queue->rear) {
        item = queue->front->data;
        free(queue->front);
        queue->front = NULL;
        queue->rear = NULL;
    } else {
        Node *temp = queue->front;
        item = temp->data;
        queue->front = temp->next;
        queue->rear->next = queue->front;
        free(temp);
    }
    return item;
}

// Распустить Очередь
void disperseQueue(Queue *queue) {
    while (queue->front != NULL) {
        dequeue(queue);
    }
    free(queue);
}