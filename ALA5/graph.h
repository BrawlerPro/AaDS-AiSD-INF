#ifndef SKIBIDITOILET_GRAPH_H
#define SKIBIDITOILET_GRAPH_H

typedef enum RoomType {ENTRANCE, EXIT, TRANSITION} RoomType;

typedef struct Graph Graph;

Graph* createGraph();
int addVertex(Graph* graph, const char* id, RoomType type);
int addEdge(Graph* graph, const char* src_id, const char* dest_id, int length);
int removeVertex(Graph* graph, const char* id);
int removeEdge(Graph* graph, const char* src_id, const char* dest_id);
int updateVertex(Graph* graph, const char* id, RoomType new_type);
int updateEdge(Graph* graph, const char* src_id, const char* dest_id, int new_length);
void printGraph(Graph* graph);
void visualizeGraph(Graph* graph);
int bfs(Graph* graph, const char* start_id, const char* end_id);
int findShortestPath(Graph* graph, const char* start_id, const char* end_id);
int findNearestExit(Graph* graph, const char* entrance_id);
void freeGraph(Graph* graph);

#endif //SKIBIDITOILET_GRAPH_H
