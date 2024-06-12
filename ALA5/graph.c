#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "graph.h"
#include "queue_prority.h"
#include "htable.h"


typedef struct Graph {
    Table* vertices;
} Graph;

Graph* createGraph() {
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    if(graph != NULL){
        graph->vertices = initTable();
        if(graph->vertices == NULL) {
            free(graph);
        }
    }
    return graph;
}



int addVertex(Graph* graph, const char* id, RoomType type) {
    if(graph == NULL) return 1;
    return insertElement(graph->vertices, id, type);
}

int removeVertex(Graph* graph, const char* id) {
    if(graph == NULL) return 1;
    return deleteElement(graph->vertices, id);
}

int updateVertex(Graph* graph, const char* id, const char *new_id, RoomType new_type) {
    unsigned int ind;
    Vertex* found = searchElement(graph->vertices, id, &ind);
    if(found != NULL){
        free(found->id);
        found->id = strdup(new_id);
        found->type = new_type;
        return 0;
    }
    return 1;
}

int addEdge(Graph* graph, const char* src_id, const char* dest_id, unsigned int length) {
    return appendEdge(graph->vertices, src_id, dest_id, length);
}

int removeEdge(Graph* graph, const char* src_id, const char* dest_id){
    unsigned int ind;
    Vertex* src = searchElement(graph->vertices, src_id, &ind);
    if(src != NULL){
        return deleteEdge(&src->edges, dest_id);
    }
    return 1;
}

int updateEdge(Graph* graph, const char* src_id, const char* dest_id, unsigned int new_length) {
    unsigned int ind;
    Vertex* tmp = searchElement(graph->vertices, src_id, &ind);
    if(tmp != NULL){
        Edge* found = searchEdge(tmp->edges, dest_id);
        if(found != NULL){
            found->distance = new_length;
            return 0;
        }
    }
    return 1;
}

void printGraph(Graph* graph) {
    if(graph == NULL) return;
    int cap = getCapacity(graph->vertices);
    for(int i = 0; i < cap; i++){
        if(graph->vertices->ks[i].busy == 1){
            printf("\t|%s", graph->vertices->ks[i].info.id);
        }
    }
    printf("\n");
    for(int i = 0; i < cap; i++){
        if(graph->vertices->ks[i].busy == 1){
            printf("%s\t", graph->vertices->ks[i].info.id);
            for(int j = 0; j < cap; j++){
                if(graph->vertices->ks[j].busy == 1){
                    Edge* found = searchEdge(graph->vertices->ks[i].info.edges, graph->vertices->ks[j].info.id);
                    if(found != NULL){
                        printf("|%d\t", found->distance);
                    } else {
                        printf("|%d\t", 0);
                    }
                }
            }
            printf("\n");
        }
    }

}

int exportGraphviz(Graph* graph) {
    FILE* file = fopen("graph.dot", "w");
    if (!file) {
        printf("Error opening file.\n");
        return 1;
    }
    fprintf(file, "digraph G {\n");
    for(int i = 0; i < getCapacity(graph->vertices); i++){
        if(graph->vertices->ks[i].busy == 1){
            fprintf(file, "    \"%s\";\n", graph->vertices->ks[i].info.id);
            if(graph->vertices->ks[i].info.edges != NULL){
                Edge* tmp = graph->vertices->ks[i].info.edges;
                while (tmp != NULL){
                    fprintf(file, "    \"%s\" -> \"%s\" [label=\"%d\"];\n", graph->vertices->ks[i].info.id, tmp->to->id, tmp->distance);
                    tmp = tmp->next;
                }
            }
        }
    }
    fprintf(file, "}\n");
    fclose(file);
    return 0;
}

void visualizeGraph(Graph* graph) {
    exportGraphviz(graph);
    system("dot -Tpng graph.dot -o graph.png");
}

int isReachable(Graph* graph, const char* startId, const char* endId) {
    if (!graph || !graph->vertices) return 0;

    unsigned int startIndex, endIndex;
    Vertex* startVertex = searchElement(graph->vertices, startId, &startIndex);
    Vertex* endVertex = searchElement(graph->vertices, endId, &endIndex);

    if (!startVertex || !endVertex) return 0;

    int* visited = (int*)calloc(graph->vertices->size, sizeof(int));
    int* queue = (int*)malloc(graph->vertices->size * sizeof(int));
    int front = 0, rear = 0;

    visited[startIndex] = 1;
    queue[rear++] = startIndex;

    while (front < rear) {
        int currentIndex = queue[front++];
        Vertex* currentVertex = &graph->vertices->ks[currentIndex].info;

        if (strcmp(currentVertex->id, endId) == 0) {
            free(visited);
            free(queue);
            return 1;
        }

        for (Edge* edge = currentVertex->edges; edge != NULL; edge = edge->next) {
            Vertex* neighbor = edge->to;
            unsigned int neighborIndex;
            searchElement(graph->vertices, neighbor->id, &neighborIndex);
            if (!visited[neighborIndex]) {
                visited[neighborIndex] = 1;
                queue[rear++] = neighborIndex;
            }
        }
    }
    free(visited);
    free(queue);
    return 0;
}

unsigned int shortestPath(Graph* graph, const char* src, const char* dest) {
    if (!graph || !graph->vertices) return UINT_MAX;

    unsigned int startIndex, endIndex;
    Vertex* startVertex = searchElement(graph->vertices, src, &startIndex);
    Vertex* endVertex = searchElement(graph->vertices, dest, &endIndex);

    if (!startVertex || !endVertex) return UINT_MAX;

    unsigned int* dist = (unsigned int*)malloc(graph->vertices->size * sizeof(unsigned int));
    int* prev = (int*)malloc(graph->vertices->size * sizeof(int));
    int* visited = (int*)calloc(graph->vertices->size, sizeof(int));

    for (int i = 0; i < graph->vertices->size; i++) {
        dist[i] = UINT_MAX;
        prev[i] = -1;
    }
    dist[startIndex] = 0;

    PriorityQueue* pq = createPriorityQueue(graph->vertices->size);
    push(pq, startIndex, 0);

    while (!isEmpty(pq)) {
        PriorityQueueNode current = pop(pq);
        int u = current.vertex;

        if (visited[u]) continue;
        visited[u] = 1;

        Vertex* uVertex = &graph->vertices->ks[u].info;
        for (Edge* edge = uVertex->edges; edge != NULL; edge = edge->next) {
            Vertex* vVertex = edge->to;
            unsigned int v;
            searchElement(graph->vertices, vVertex->id, &v);

            if (!visited[v] && dist[u] != UINT_MAX && dist[u] + edge->distance < dist[v]) {
                dist[v] = dist[u] + edge->distance;
                prev[v] = u;
                push(pq, v, dist[v]);
            }
        }
    }

    if (dist[endIndex] != UINT_MAX) {
        unsigned int min = dist[endIndex];
        int path[graph->vertices->size];
        int count = 0;
        for (int at = endIndex; at != -1; at = prev[at]) {
            path[count++] = at;
        }
        printf("Path: ");
        for (int i = count - 1; i >= 0; i--) {
            printf("%s ", graph->vertices->ks[path[i]].info.id);
        }
        printf("\n");
        free(dist);
        free(prev);
        free(visited);
        freeQueue(pq);
        return min;
    } else {
        free(dist);
        free(prev);
        free(visited);
        freeQueue(pq);
        return UINT_MAX;
    }
}

unsigned int** createADJ(Graph* graph){
    if(graph == NULL) return NULL;
    unsigned int** adj = (unsigned int**) malloc(getCapacity(graph->vertices)*sizeof(unsigned int*));
    if(adj == NULL) return NULL;
    for(int i = 0; i < getCapacity(graph->vertices); i++){
        adj[i] = (unsigned int*) calloc(getCapacity(graph->vertices), sizeof(unsigned int));
        if(adj[i] == NULL){
            for(int j = 0; j < i; j++){
                free(adj[j]);
            }
            free(adj);
            return NULL;
        }
    }

    for(int i = 0; i < getCapacity(graph->vertices); i++){
        if(graph->vertices->ks[i].busy == 1){
            for(int j = 0; j < getCapacity(graph->vertices); j++){
                if(graph->vertices->ks[j].busy == 1){
                    Edge* found = searchEdge(graph->vertices->ks[i].info.edges, graph->vertices->ks[j].info.id);
                    if(found != NULL){
                        adj[i][j] = found->distance;
                    }
                }
            }
        }
    }
    return adj;
}

void freeADJ(unsigned int** adj, int size){
    for(int i = 0; i<size;i++){
        free(adj[i]);
    }
    free(adj);
}

int getVertexIndex(Table* table, const char* id) {
    for (int i = 0; i < table->size; i++) {
        if (table->ks[i].busy && strcmp(table->ks[i].info.id, id) == 0) {
            return i;
        }
    }
    return -1;
}

unsigned int** floydWarshall(Graph* graph, int*** next) {
    int size = getCapacity(graph->vertices);
    unsigned int** dist = (unsigned int**)malloc(size * sizeof(unsigned int*));
    *next = (int**)malloc(size * sizeof(int*));
    for (int i = 0; i < size; i++) {
        dist[i] = (unsigned int*)malloc(size * sizeof(unsigned int));
        (*next)[i] = (int*)malloc(size * sizeof(int));
        for (int j = 0; j < size; j++) {
            if (i == j) {
                dist[i][j] = 0;
                (*next)[i][j] = -1;
            } else {
                dist[i][j] = UINT_MAX;
                (*next)[i][j] = -1;
            }
        }
    }

    for (int i = 0; i < size; i++) {
        if (graph->vertices->ks[i].busy) {
            Edge* edge = graph->vertices->ks[i].info.edges;
            while (edge != NULL) {
                int j = getVertexIndex(graph->vertices, edge->to->id);
                if (j != -1) {
                    dist[i][j] = edge->distance;
                    (*next)[i][j] = j;
                }
                edge = edge->next;
            }
        }
    }

    for (int k = 0; k < size; k++) {
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                if (dist[i][k] != UINT_MAX && dist[k][j] != UINT_MAX &&
                    dist[i][k] + dist[k][j] < dist[i][j]) {
                    dist[i][j] = dist[i][k] + dist[k][j];
                    (*next)[i][j] = (*next)[i][k];
                }
            }
        }
    }

    return dist;
}


unsigned int findNearestExit(Graph* graph, const char *src) {
    int start = getVertexIndex(graph->vertices, src);
    if (start == -1) {
        printf("Исходная вершина не найдена\n");
        return UINT_MAX;
    }

    int** next;
    unsigned int** dist = floydWarshall(graph, &next);

    unsigned int minDist = UINT_MAX;
    int exitIndex = -1;
    for (int i = 0; i < getCapacity(graph->vertices); i++) {
        if (graph->vertices->ks[i].busy && graph->vertices->ks[i].info.type == EXIT) {
            if (dist[start][i] < minDist) {
                minDist = dist[start][i];
                exitIndex = i;
            }
        }
    }

    if (exitIndex != -1) {
        printf("Ближайшее расстояние до выхода из вершины %s равно: %u\n", src, minDist);
        int u = start;
        int v =exitIndex;
        if (next[u][v] == -1) {
            printf("No path\n");
        }
        printf("Path: %s", graph->vertices->ks[u].info.id);
        while (u != v) {
            u = next[u][v];
            printf(" -> %s", graph->vertices->ks[u].info.id);
        }
        printf("\n");
    } else {
        printf("Не найден выход из вершины %s\n", src);
    }

    for (int i = 0; i < getCapacity(graph->vertices); i++) {
        free(dist[i]);
        free(next[i]);
    }
    free(dist);
    free(next);

    return minDist;
}


void freeGraph(Graph* graph) {
    if (!graph || !graph->vertices) return;
    freeTable(graph->vertices);
    free(graph);
}

