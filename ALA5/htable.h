#ifndef HTABLE_H
#define HTABLE_H
#include "graph.h"

typedef struct Edge {
    struct Vertex* to;
    unsigned int distance;
    struct Edge* next;
} Edge;

typedef struct Vertex{
    char* id;
    RoomType type;
    Edge* edges;
}Vertex;

typedef struct KeySpace {
    Vertex info;
    int busy;
} KeySpace;

typedef struct Table {
    KeySpace *ks;
    int size;
    int capacity;
} Table;

Table *initTable();

int getCapacity(Table* table);

int insertElement(Table *table, const char* id, RoomType type);

int deleteEdge(Edge** edges, const char* id);

int appendEdge(Table* table, const char* from, const char* to, unsigned int distance);

int deleteElement(Table *table, const char* id);

Vertex *searchElement(struct Table *table, const char* id, unsigned int* ind);

Edge* searchEdge(Edge* edges, const char* id);

void freeTable(Table *table);

#endif /* HTABLE_H */
