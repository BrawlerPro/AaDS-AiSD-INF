#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "graph.h"

#define MAX_VERTICES 100

typedef struct Vertex {
    char* id;
    RoomType type;
    struct Vertex* next;
    struct Edge* edges;
} Vertex;

typedef struct Edge {
    struct Vertex* destination;
    unsigned int length;
    struct Edge* next;
} Edge;

typedef struct Graph {
    Vertex* vertices[MAX_VERTICES];
    int vertex_count;
} Graph;

Graph* createGraph() {
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    if(graph != NULL){
        graph->vertex_count = 0;
    }
    return graph;
}

Vertex* createVertex(const char* id, RoomType type) {
    Vertex* vertex = (Vertex*)malloc(sizeof(Vertex));
    if(vertex != NULL){
        vertex->id = strdup(id);
        vertex->type = type;
        vertex->next = NULL;
        vertex->edges = NULL;
    }
    return vertex;
}

Edge* createEdge(Vertex* destination, int length) {
    Edge* edge = (Edge*)malloc(sizeof(Edge));
    if(edge != NULL) {
        edge->destination = destination;
        edge->length = length;
        edge->next = NULL;
    }
    return edge;
}

int addVertex(Graph* graph, const char* id, RoomType type) {
    if(graph == NULL) return 1;

    if (graph->vertex_count >= MAX_VERTICES) {
        return 1;
    }
    Vertex* vertex = createVertex(id, type);
    if(vertex == NULL) return 1;
    graph->vertices[graph->vertex_count++] = vertex;
    return 0;
}

int addEdge(Graph* graph, const char* src_id, const char* dest_id, int length) {
    if(graph == NULL) return 1;

    Vertex *src = NULL, *dest = NULL;
    for (int i = 0; i < graph->vertex_count; i++) {
        if (strcmp(graph->vertices[i]->id, src_id) == 0) {
            src = graph->vertices[i];
        }
        if (strcmp(graph->vertices[i]->id, dest_id) == 0) {
            dest = graph->vertices[i];
        }
    }
    if (src && dest) {
        Edge* edge = createEdge(dest, length);
        edge->next = src->edges;
        src->edges = edge;
    } else {
        return 1;
    }
    return 0;
}

int removeVertex(Graph* graph, const char* id) {
    if(graph == NULL) return 1;

    for (int i = 0; i < graph->vertex_count; i++) {
        if (strcmp(graph->vertices[i]->id, id) == 0) {
            Vertex* vertex_to_remove = graph->vertices[i];
            for (int j = 0; j < graph->vertex_count; j++) {
                if (graph->vertices[j] != vertex_to_remove) {
                    Edge* prev = NULL;
                    Edge* edge = graph->vertices[j]->edges;
                    while (edge) {
                        if (edge->destination == vertex_to_remove) {
                            if (prev) {
                                prev->next = edge->next;
                            } else {
                                graph->vertices[j]->edges = edge->next;
                            }
                            free(edge);
                            break;
                        }
                        prev = edge;
                        edge = edge->next;
                    }
                }
            }

            free(vertex_to_remove);
            for (int j = i; j < graph->vertex_count - 1; j++) {
                graph->vertices[j] = graph->vertices[j + 1];
            }
            graph->vertices[--graph->vertex_count] = NULL;
            return 0;
        }
    }
    return 1;
}


int removeEdge(Graph* graph, const char* src_id, const char* dest_id) {
    Vertex *src = NULL, *dest = NULL;
    if(graph == NULL) return 1;
    for (int i = 0; i < graph->vertex_count; i++) {
        if (strcmp(graph->vertices[i]->id, src_id) == 0) {
            src = graph->vertices[i];
        }
        if (strcmp(graph->vertices[i]->id, dest_id) == 0) {
            dest = graph->vertices[i];
        }
    }
    if (src && dest) {
        Edge* prev = NULL;
        Edge* edge = src->edges;
        while (edge) {
            if (edge->destination == dest) {
                if (prev) {
                    prev->next = edge->next;
                } else {
                    src->edges = edge->next;
                }
                free(edge);
                return 0;
            }
            prev = edge;
            edge = edge->next;
        }
    } else {
        return 1;
    }
    return 1;
}


int updateVertex(Graph* graph, const char* id, RoomType new_type) {
    for (int i = 0; i < graph->vertex_count; i++) {
        if (strcmp(graph->vertices[i]->id, id) == 0) {
            graph->vertices[i]->type = new_type;
            return 0;
        }
    }
    return 1;
}


int updateEdge(Graph* graph, const char* src_id, const char* dest_id, int new_length) {
    Vertex *src = NULL, *dest = NULL;

    for (int i = 0; i < graph->vertex_count; i++) {
        if (strcmp(graph->vertices[i]->id, src_id) == 0) {
            src = graph->vertices[i];
        }
        if (strcmp(graph->vertices[i]->id, dest_id) == 0) {
            dest = graph->vertices[i];
        }
    }
    if (src && dest) {
        Edge* edge = src->edges;
        while (edge) {
            if (edge->destination == dest) {
                edge->length = new_length;
                return 0;
            }
            edge = edge->next;
        }
    } else {
        return 1;
    }
    return 1;
}


void printGraph(Graph* graph) {
    for (int i = 0; i < graph->vertex_count; i++) {
        Vertex* vertex = graph->vertices[i];
        printf("%s (%d): ", vertex->id, vertex->type);
        Edge* edge = vertex->edges;
        while (edge) {
            printf(" -> %s (%d)", edge->destination->id, edge->length);
            edge = edge->next;
        }
        printf("\n");
    }
}

int exportGraphviz(Graph* graph) {
    FILE* file = fopen("graph.dot", "w");
    if (!file) {
        printf("Error opening file.\n");
        return 1;
    }
    fprintf(file, "digraph G {\n");
    for (int i = 0; i < graph->vertex_count; i++) {
        Vertex* vertex = graph->vertices[i];
        Edge* edge = vertex->edges;
        while (edge) {
            fprintf(file, "    \"%s\" -> \"%s\" [label=\"%d\"];\n", vertex->id, edge->destination->id, edge->length);
            edge = edge->next;
        }
    }
    fprintf(file, "}\n");
    fclose(file);
    return 0;
}

void visualizeGraph(Graph* graph) {
    system("dot -Tpng graph.dot -o graph.png");

}

int bfs(Graph* graph, const char* start_id, const char* end_id) {
    int visited[MAX_VERTICES] = {0};
    Vertex* queue[MAX_VERTICES];
    int front = 0, rear = 0;

    Vertex* start_vertex = NULL;
    for (int i = 0; i < graph->vertex_count; i++) {
        if (strcmp(graph->vertices[i]->id, start_id) == 0) {
            start_vertex = graph->vertices[i];
            break;
        }
    }
    if (!start_vertex) {
        return 1;
    }

    queue[rear++] = start_vertex;
    visited[start_vertex - graph->vertices[0]] = 1;

    while (front < rear) {
        Vertex* current = queue[front++];
        if (current->type == EXIT && strcmp(current->id, end_id) == 0) {
            return 0;
        }
        Edge* edge = current->edges;
        while (edge) {
            if (!visited[edge->destination - graph->vertices[0]]) {
                queue[rear++] = edge->destination;
                visited[edge->destination - graph->vertices[0]] = 1;
            }
            edge = edge->next;
        }
    }
    return 1;
}

int dijkstra(Graph* graph, const char* start_id, const char* end_id) {
    int distances[MAX_VERTICES];
    int visited[MAX_VERTICES] = {0};
    Vertex* previous[MAX_VERTICES];

    for (int i = 0; i < graph->vertex_count; i++) {
        distances[i] = INT_MAX;
    }

    Vertex* start_vertex = NULL;
    Vertex* end_vertex = NULL;
    for (int i = 0; i < graph->vertex_count; i++) {
        if (strcmp(graph->vertices[i]->id, start_id) == 0) {
            start_vertex = graph->vertices[i];
            distances[i] = 0;
        }
        if (strcmp(graph->vertices[i]->id, end_id) == 0) {
            end_vertex = graph->vertices[i];
        }
    }
    if (!start_vertex || !end_vertex) {
        return INT_MAX;
    }

    for (int i = 0; i < graph->vertex_count; i++) {
        int min_distance = INT_MAX;
        int min_index = -1;
        for (int j = 0; j < graph->vertex_count; j++) {
            if (!visited[j] && distances[j] < min_distance) {
                min_distance = distances[j];
                min_index = j;
            }
        }
        if (min_index == -1) {
            break;
        }
        visited[min_index] = 1;
        Vertex* current = graph->vertices[min_index];
        Edge* edge = current->edges;
        while (edge) {
            int alt = distances[min_index] + edge->length;
            int neighbor_index = edge->destination - graph->vertices[0];
            if (alt < distances[neighbor_index]) {
                distances[neighbor_index] = alt;
                previous[neighbor_index] = current;
            }
            edge = edge->next;
        }
    }

    return distances[end_vertex - graph->vertices[0]];
}

int findNearestExit(Graph* graph, const char* entrance_id) {
    int dist[MAX_VERTICES][MAX_VERTICES];
    int next[MAX_VERTICES][MAX_VERTICES];
    int n = graph->vertex_count;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (i == j) {
                dist[i][j] = 0;
            } else {
                dist[i][j] = INT_MAX;
            }
            next[i][j] = -1;
        }
    }

    for (int i = 0; i < n; i++) {
        Vertex* vertex = graph->vertices[i];
        Edge* edge = vertex->edges;
        while (edge != NULL) {
            int u = i;
            int v = -1;
            for (int k = 0; k < n; k++) {
                if (graph->vertices[k] == edge->destination) {
                    v = k;
                    break;
                }
            }
            if (v != -1) {
                dist[u][v] = edge->length;
                next[u][v] = v;
            }
            edge = edge->next;
        }
    }

    for (int k = 0; k < n; k++) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (dist[i][k] != INT_MAX && dist[k][j] != INT_MAX && dist[i][k] + dist[k][j] < dist[i][j]) {
                    dist[i][j] = dist[i][k] + dist[k][j];
                    next[i][j] = next[i][k];
                }
            }
        }
    }

    int entrance_index = -1;
    for (int i = 0; i < n; i++) {
        if (strcmp(graph->vertices[i]->id, entrance_id) == 0) {
            entrance_index = i;
            break;
        }
    }

    if (entrance_index == -1) {
        return -1;
    }

    int nearest_exit_index = -1;
    int min_distance = INT_MAX;
    for (int i = 0; i < n; i++) {
        if (graph->vertices[i]->type == EXIT && dist[entrance_index][i] < min_distance) {
            min_distance = dist[entrance_index][i];
            nearest_exit_index = i;
        }
    }

    if (nearest_exit_index == -1) {
        return -1;
    } else {
        return min_distance;
    }
}


void freeGraph(Graph* graph) {
    for (int i = 0; i < graph->vertex_count; i++) {
        Vertex* vertex = graph->vertices[i];
        Edge* edge = vertex->edges;
        while (edge != NULL) {
            Edge* next_edge = edge->next;
            free(edge);
            edge = next_edge;
        }
        free(vertex);
    }
    free(graph);
}



//void visualize_graph(Graph* graph, const char* filename) {
//    FILE* dot_file = fopen("graph.dot", "w");
//    if (dot_file == NULL) {
//        printf("Ошибка при создании файла DOT.\n");
//        return;
//    }
//
//    fprintf(dot_file, "digraph G {\n");
//
//    for (int i = 0; i < graph->vertex_count; i++) {
//        Vertex* vertex = graph->vertices[i];
//        fprintf(dot_file, "  %s [label=\"%s\"];\n", vertex->id, vertex->id);
//
//        Edge* edge = vertex->edges;
//        while (edge != NULL) {
//            fprintf(dot_file, "  %s -> %s [label=\"%d\"];\n", vertex->id, edge->destination->id, edge->length);
//            edge = edge->next;
//        }
//    }
//
//    fprintf(dot_file, "}\n");
//
//    fclose(dot_file);
//
//    char command[100];
//    snprintf(command, sizeof(command), "dot -Tpng %s -o %s.png", filename, filename);
//    system(command);
//
//    printf("Граф визуализирован в файле %s.png\n", filename);
//}
