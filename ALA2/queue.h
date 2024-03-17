#ifndef QUEUE_H
#define QUEUE_H

typedef struct Queue Queue;

// Инициализация очереди
Queue* createQueue();

// Проверка на пустоту
int isEmpty(Queue* queue);

// Добавление элемента в очередь
int enqueue(Queue* queue, double data);

// Удаление элемента из очереди
double dequeue(Queue* queue);

// Удалить Очередь(Распустить Очередь)
void disperseQueue(Queue* queue);

#endif
