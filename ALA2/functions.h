#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include "queue.h"

double calculateSin(double x);

void processTasks(Queue *slave_queues[], int num_slaves, int angle, double p1);

void calculateAndEnqueue(Queue *master_queue, Queue *slave_queues[], int num_slaves, double p2);

void printResults(Queue *master_queue, int angle, double p3);

void processMain(Queue *master_queue, Queue *slave_queues[], int num_slaves, int start_angle, int end_angle, double p1, double p2, double p3);

void freeMemory(Queue *master_queue, Queue *slave_queues[], int num_slaves);

#endif
