#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "queue.h"
#include "functions.h"


int main() {
    int num_slaves, start_angle, end_angle;
    double p1, p2, p3;

    // Ввод параметров
    printf("Введите количество ведомых узлов, начальный угол, конечный угол, вероятности p1, p2, p3: ");
    scanf("%d %d %d %lf %lf %lf", &num_slaves, &start_angle, &end_angle, &p1, &p2, &p3);

    srand(time(NULL));

    Queue *master_queue = createQueue();
    Queue *slave_queues[num_slaves];

    for (int i = 0; i < num_slaves; ++i) {
        slave_queues[i] = createQueue();
    }

    processMain(master_queue, slave_queues, num_slaves, start_angle, end_angle, p1, p2, p3);

    freeMemory(master_queue, slave_queues, num_slaves);

    return 0;
}
