#ifndef SKIBIDITOILET_GRAPH_H
#define SKIBIDITOILET_GRAPH_H
#include <stdbool.h>

typedef enum RoomType {ENTRANCE, EXIT, TRANSITION} RoomType;

typedef struct Graph Graph;

Graph* createGraph();
int addVertex(Graph* graph, const char* id, RoomType type);
int updateVertex(Graph* graph, const char* id, const char *new_id, RoomType new_type);
int removeVertex(Graph* graph, const char* id);
int addEdge(Graph* graph, const char* src_id, const char* dest_id, unsigned int length);
int updateEdge(Graph* graph, const char* src_id, const char* dest_id, unsigned int new_length);
int removeEdge(Graph* graph, const char* src_id, const char* dest_id);
void printGraph(Graph* graph);
void visualizeGraph(Graph* graph);
int isReachable(Graph* graph, const char* startId, const char* endId) ;
unsigned int shortestPath(Graph* graph, const char* src, const char* dest);
unsigned int findNearestExit(Graph* graph, const char *src);
void freeGraph(Graph* graph);

#endif //SKIBIDITOILET_GRAPH_H
