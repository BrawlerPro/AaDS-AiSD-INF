#include <stdio.h>
#include <stdlib.h>
#include "graph.h"

void readVerticesFromFile(Graph* graph, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Unable to open file");
        return;
    }

    char* line = NULL;
    size_t len = 0;
    ssize_t read;

    while ((read = getline(&line, &len, file)) != -1) {
        char id[256];
        int type;
        sscanf(line, "%s %d", id, &type);
        addVertex(graph, id, (RoomType)type);
    }

    free(line);
    fclose(file);
}


void readEdgesFromFile(Graph* graph, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Unable to open file");
        return;
    }

    char* line = NULL;
    size_t len = 0;
    ssize_t read;

    while ((read = getline(&line, &len, file)) != -1) {
        char from[256], to[256];
        unsigned int length;
        sscanf(line, "%s %s %u", from, to, &length);
        addEdge(graph, from, to, length);

    }

    free(line);
    fclose(file);
}