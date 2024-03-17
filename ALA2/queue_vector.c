#include <stdio.h>
#include <stdlib.h>

#define INITIAL_CAPACITY 100

// Очередь Вектор
typedef struct Queue {
    double *array;
    int capacity;
    int rear;
    int front;
    int size;
} Queue;

// Инициализация очереди
Queue *createQueue() {
    Queue *queue = (Queue *) malloc(sizeof(Queue));
    if (!queue)
        return NULL;
    queue->array = (double *) malloc(INITIAL_CAPACITY * sizeof(double));
    if (!queue->array) {
        free(queue);
        return NULL;
    }
    queue->capacity = INITIAL_CAPACITY;
    queue->front = queue->rear = queue->size = 0;
    return (Queue *) queue;
}

// Проверка на пустоту
int isEmpty(Queue *queue) {
    return queue->size == 0;
}

// Добавление элемента в очередь
int enqueue(Queue *queue, double item) {
    if (queue->size == queue->capacity) return 1;
    queue->array[queue->rear++] = item;
    queue->rear %= queue->capacity;
    queue->size++;
    return 0;
}

// Удаление элемента из очереди
double dequeue(Queue *queue) {
    double item = queue->array[queue->front++];
    queue->front %= queue->capacity;
    queue->size--;
    return item;
}

//Распустить Очередь
void disperseQueue(Queue *queue) {
    free(queue->array);
    free(queue);
}
