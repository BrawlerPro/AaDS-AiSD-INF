#include <stdio.h>
#include <stdlib.h>

#define INITIAL_CAPACITY 100

// Очередь Вектор
typedef struct Queue {
    double *array;
    int *angles;
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
    queue->angles = (int *) malloc(INITIAL_CAPACITY * sizeof(int));
    if (!queue->array || !queue->angles) {
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
int enqueue(Queue *queue, double item, int angle) {
    if (queue->size == queue->capacity) return 1;
    queue->array[queue->rear] = item;
    queue->angles[queue->rear] = angle;
    queue->rear++;
    queue->rear %= queue->capacity;
    queue->size++;
    return 0;
}

// Удаление элемента из очереди
int dequeue(Queue *queue, double *item, int *angle) {
    if(isEmpty(queue)) return 1;
    (*item) = queue->array[queue->front];
    (*angle) = queue->angles[queue->front];
    queue->front++;
    queue->front %= queue->capacity;
    queue->size--;
    return 0;
}

//Распустить Очередь
void disperseQueue(Queue *queue) {
    free(queue->array);
    free(queue->angles);
    free(queue);
}
