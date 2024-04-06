#ifndef QUEUE_H
#define QUEUE_H

typedef struct Queue Queue;

// Инициализация очереди
Queue* createQueue();

// Проверка на пустоту
int isEmpty(Queue* queue);

// Добавление элемента в очередь
int enqueue(Queue* queue, double data, int angle);

// Удаление элемента из очереди
int dequeue(Queue* queue, double *data, int* angle);

// Удалить Очередь(Распустить Очередь)
void disperseQueue(Queue* queue);

#endif
