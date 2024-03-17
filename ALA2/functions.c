#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "queue.h"

// Расчет значения синуса через ряд Маклорена
double calculateSin(double x) {
    x = x * 3.14159265359 / 180;
    double result = 0.0;
    double term = x;
    for (int n = 1; fabs(term) > 0.00000000000000000001; n++) {
        result += term;
        term *= (-x * x) / ((2 * n) * (2 * n + 1));
    }
    return result;
}

// C вероятностью p1 ведущий узел помещает задания для выполнения (значение угла) в очереди
// ведомых узлов
void processTasks(Queue *slave_queues[], int num_slaves, int angle, double p1) {
    if ((double) rand() / RAND_MAX < p1) {
        for (int i = 0; i < num_slaves; ++i) {
            enqueue(slave_queues[i], angle);
        }
    }
}

// С вероятностью p2 ведомые узлы вычитывают задания из своих очередей, вычисляют значение
// синуса угла при помощи ряда Маклорена: sin x = ∑(inf; n=1)((−1)^(n+1)*x^(2n−1)/(2n−1)!), и помещают значения
// вычислений в очереди ведущего узла
void calculateAndEnqueue(Queue *master_queue, Queue *slave_queues[], int num_slaves, double p2) {
    if ((double) rand() / RAND_MAX < p2) {
        for (int i = 0; i < num_slaves; ++i) {
            if (!isEmpty(slave_queues[i])) {
                double task = dequeue(slave_queues[i]);
                double result = calculateSin(task);
                enqueue(master_queue, result);
            }
        }
    }
}

// С вероятностью p3 ведущий узел вычитывает результаты выполнения заданий ведомыми узлами и добавляет
// их в итоговую таблицу.
void printResults(Queue *master_queue, int angle, double p3) {
    if ((double) rand() / RAND_MAX < p3) {
        while (!isEmpty(master_queue)) {
            double result = (double) dequeue(master_queue);
            printf("Угол: %d, sin: %lf\n", angle, result);
        }
    }
}


void processMain(Queue *master_queue, Queue *slave_queues[], int num_slaves, int start_angle, int end_angle, double p1, double p2, double p3) {
    for (int angle = start_angle; angle <= end_angle; ++angle) {
        processTasks(slave_queues, num_slaves, angle, p1);
        calculateAndEnqueue(master_queue, slave_queues, num_slaves, p2);
        printResults(master_queue, angle, p3);
    }
}

// Очистить память
void freeMemory(Queue *master_queue, Queue *slave_queues[], int num_slaves) {
    disperseQueue(master_queue);
    for (int i = 0; i < num_slaves; i++) {
        disperseQueue(slave_queues[i]);
    }
}
