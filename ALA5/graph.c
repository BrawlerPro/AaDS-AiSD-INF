#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "graph.h"
#include "queue_prority.h"

#define ARRAY_SIZE 1136

static const int primeNum[ARRAY_SIZE] = {3, 7, 13, 19, 29, 37, 43, 53, 61, 71, 79, 89, 101, 107, 113, 131, 139, 151, 163, 173, 181, 193, 199, 223, 229, 239, 251, 263, 271, 281, 293, 311, 317, 337, 349, 359, 373, 383, 397, 409, 421, 433, 443, 457, 463, 479, 491, 503, 521, 541, 557, 569, 577, 593, 601, 613, 619, 641, 647, 659, 673, 683, 701, 719, 733, 743, 757, 769, 787, 809, 821, 827, 839, 857, 863, 881, 887, 911, 929, 941, 953, 971, 983, 997, 1013, 1021, 1033, 1049, 1061, 1069, 1091, 1097, 1109, 1123, 1151, 1163, 1181, 1193, 1213, 1223, 1231, 1249, 1277, 1283, 1291, 1301, 1307, 1321, 1361, 1373, 1399, 1423, 1429, 1439, 1451, 1459, 1481, 1487, 1493, 1511, 1531, 1549, 1559, 1571, 1583, 1601, 1609, 1619, 1627, 1657, 1667, 1693, 1699, 1721, 1733, 1747, 1759, 1783, 1789, 1811, 1831, 1861, 1871, 1877, 1889, 1907, 1931, 1949, 1973, 1987, 1997, 2003, 2017, 2029, 2053, 2069, 2083, 2089, 2111, 2129, 2137, 2143, 2161, 2203, 2213, 2237, 2243, 2267, 2273, 2287, 2297, 2311, 2339, 2347, 2357, 2377, 2383, 2393, 2411, 2423, 2441, 2459, 2473, 2503, 2531, 2543, 2551, 2579, 2593, 2617, 2633, 2657, 2663, 2677, 2687, 2693, 2707, 2713, 2729, 2741, 2753, 2777, 2791, 2801, 2819, 2837, 2851, 2861, 2887, 2903, 2917, 2939, 2957, 2969, 2999, 3011, 3023, 3041, 3061, 3079, 3089, 3119, 3137, 3167, 3181, 3191, 3209, 3221, 3251, 3257, 3271, 3301, 3313, 3323, 3331, 3347, 3361, 3373, 3391, 3413, 3449, 3461, 3467, 3491, 3511, 3527, 3533, 3541, 3557, 3571, 3583, 3607, 3617, 3631, 3643, 3671, 3677, 3697, 3709, 3727, 3739, 3767, 3779, 3797, 3821, 3833, 3851, 3863, 3881, 3907, 3917, 3923, 3931, 3947, 3989, 4003, 4013, 4021, 4049, 4057, 4079, 4093, 4111, 4129, 4139, 4157, 4177, 4211, 4219, 4231, 4243, 4259, 4271, 4283, 4297, 4337, 4349, 4363, 4391, 4409, 4423, 4447, 4457, 4481, 4493, 4513, 4519, 4547, 4561, 4583, 4597, 4621, 4639, 4649, 4657, 4673, 4691, 4721, 4729, 4751, 4783, 4789, 4799, 4813, 4831, 4871, 4889, 4909, 4931, 4937, 4951, 4967, 4973, 4993, 5003, 5011, 5023, 5051, 5077, 5087, 5101, 5113, 5147, 5167, 5179, 5197, 5227, 5233, 5261, 5279, 5297, 5309, 5333, 5351, 5387, 5399, 5413, 5419, 5437, 5443, 5471, 5479, 5501, 5507, 5521, 5531, 5563, 5573, 5591, 5639, 5647, 5653, 5659, 5683, 5693, 5711, 5737, 5743, 5779, 5791, 5807, 5821, 5839, 5849, 5857, 5867, 5879, 5897, 5923, 5939, 5981, 6007, 6029, 6043, 6053, 6073, 6089, 6101, 6121, 6133, 6151, 6173, 6199, 6211, 6221, 6247, 6263, 6271, 6287, 6301, 6317, 6329, 6343, 6359, 6367, 6379, 6397, 6427, 6451, 6473, 6491, 6529, 6551, 6563, 6571, 6581, 6607, 6637, 6659, 6673, 6689, 6701, 6709, 6733, 6761, 6779, 6791, 6803, 6827, 6833, 6857, 6869, 6883, 6907, 6917, 6949, 6961, 6971, 6983, 6997, 7013, 7027, 7043, 7069, 7103, 7121, 7129, 7159, 7187, 7207, 7213, 7229, 7243, 7253, 7297, 7309, 7331, 7349, 7369, 7411, 7433, 7457, 7477, 7487, 7499, 7517, 7529, 7541, 7549, 7561, 7577, 7589, 7603, 7621, 7643, 7669, 7681, 7691, 7703, 7723, 7741, 7757, 7789, 7817, 7829, 7853, 7873, 7879, 7901, 7919, 7933, 7949, 7963, 8009, 8017, 8053, 8069, 8087, 8093, 8111, 8123, 8161, 8171, 8191, 8219, 8231, 8237, 8263, 8273, 8291, 8297, 8317, 8353, 8369, 8387, 8419, 8429, 8443, 8461, 8501, 8521, 8537, 8543, 8573, 8597, 8609, 8627, 8641, 8663, 8677, 8689, 8699, 8713, 8731, 8741, 8753, 8779, 8803, 8819, 8831, 8839, 8861, 8867, 8893, 8929, 8941, 8963, 8971, 9001, 9011, 9029, 9043, 9059, 9091, 9109, 9133, 9151, 9161, 9181, 9199, 9209, 9227, 9241, 9277, 9283, 9311, 9323, 9341, 9349, 9377, 9397, 9413, 9421, 9433, 9439, 9463, 9473, 9491, 9511, 9533, 9547, 9587, 9613, 9623, 9631, 9649, 9677, 9689, 9719, 9733, 9743, 9767, 9781, 9791, 9811, 9829, 9839, 9857, 9871, 9887, 9907, 9929, 9941, 9967, 10007, 10037, 10061, 10069, 10091, 10099, 10111, 10139, 10151, 10163, 10177, 10193, 10223, 10247, 10259, 10271, 10289, 10303, 10321, 10333, 10343, 10369, 10399, 10429, 10453, 10459, 10477, 10499, 10513, 10531, 10567, 10597, 10607, 10627, 10639, 10657, 10667, 10691, 10711, 10729, 10739, 10771, 10789, 10831, 10847, 10859, 10867, 10889, 10903, 10937, 10949, 10973, 10987, 11003, 11047, 11059, 11071, 11087, 11113, 11119, 11149, 11161, 11173, 11197, 11239, 11251, 11261, 11279, 11299, 11317, 11329, 11353, 11383, 11399, 11423, 11443, 11467, 11483, 11491, 11503, 11527, 11551, 11587, 11597, 11621, 11657, 11681, 11699, 11717, 11731, 11777, 11783, 11801, 11813, 11827, 11833, 11863, 11887, 11903, 11923, 11933, 11941, 11959, 11971, 11987, 12011, 12041, 12049, 12073, 12101, 12109, 12119, 12149, 12161, 12197, 12211, 12239, 12251, 12263, 12277, 12289, 12323, 12343, 12373, 12379, 12401, 12413, 12433, 12451, 12473, 12487, 12497, 12511, 12527, 12541, 12553, 12577, 12589, 12611, 12619, 12641, 12653, 12671, 12697, 12713, 12739, 12757, 12781, 12799, 12821, 12829, 12853, 12893, 12907, 12917, 12923, 12953, 12967, 12979, 13001, 13007, 13033, 13043, 13063, 13099, 13109, 13127, 13151, 13163, 13177, 13187, 13219, 13241, 13259, 13291, 13309, 13327, 13337, 13367, 13397, 13411, 13421, 13451, 13463, 13477, 13499, 13523, 13553, 13577, 13597, 13619, 13633, 13669, 13681, 13691, 13697, 13711, 13723, 13751, 13759, 13781, 13799, 13829, 13841, 13873, 13879, 13901, 13907, 13921, 13933, 13967, 13999, 14011, 14033, 14057, 14081, 14087, 14143, 14153, 14173, 14197, 14221, 14249, 14281, 14303, 14323, 14341, 14369, 14389, 14407, 14419, 14431, 14447, 14461, 14489, 14519, 14537, 14549, 14557, 14563, 14593, 14627, 14633, 14653, 14669, 14699, 14717, 14731, 14741, 14753, 14767, 14779, 14797, 14821, 14831, 14851, 14869, 14887, 14897, 14929, 14947, 14957, 14983, 15017, 15053, 15073, 15083, 15101, 15121, 15137, 15149, 15173, 15193, 15217, 15233, 15259, 15269, 15277, 15289, 15307, 15319, 15331, 15359, 15373, 15383, 15401, 15427, 15443, 15461, 15473, 15497, 15527, 15551, 15569, 15583, 15607, 15629, 15643, 15649, 15667, 15679, 15727, 15733, 15739, 15761, 15773, 15791, 15803, 15817, 15859, 15881, 15889, 15907, 15919, 15937, 15971, 15991, 16007, 16057, 16063, 16069, 16087, 16097, 16111, 16139, 16183, 16189, 16217, 16229, 16249, 16267, 16301, 16333, 16349, 16363, 16381, 16417, 16427, 16447, 16453, 16481, 16493, 16529, 16553, 16567, 16603, 16619, 16633, 16651, 16661, 16691, 16699, 16729, 16747, 16763, 16811, 16829, 16843, 16879, 16889, 16903, 16927, 16937, 16963, 16981, 16993, 17021, 17029, 17041, 17053, 17093, 17107, 17123, 17159, 17183, 17191, 17207, 17231, 17257, 17293, 17317, 17327, 17341, 17359, 17383, 17389, 17401, 17419, 17443, 17467, 17477, 17489, 17497, 17519, 17551, 17573, 17581, 17599, 17623, 17657, 17669, 17683, 17713, 17737, 17749, 17783, 17791, 17827, 17839, 17863, 17891, 17909, 17921, 17929, 17957, 17971, 17981, 17989, 18041, 18047, 18059, 18077, 18097, 18121, 18131, 18143, 18169, 18191, 18211, 18223, 18233, 18253, 18269, 18289, 18307, 18313, 18341, 18367, 18379, 18401, 18427, 18439, 18451, 18461, 18493, 18517, 18523, 18541, 18583, 18593, 18637, 18671, 18691, 18713, 18731, 18749, 18773, 18793, 18803, 18859, 18899, 18913, 18919, 18959, 18979, 19009, 19031, 19051, 19073, 19081, 19121, 19141, 19163, 19183, 19211, 19219, 19237, 19259, 19273, 19301, 19319, 19373, 19381, 19391, 19417, 19423, 19429, 19441, 19457, 19469, 19477, 19489, 19507, 19541, 19553, 19571, 19583, 19603, 19661, 19687, 19699, 19717, 19739, 19753, 19763, 19793, 19813, 19841, 19853, 19867, 19891, 19919, 19937, 19961, 19973, 19991, 19997, 20021, 20029, 20051, 20071, 20101};


typedef struct Vertex{
    const char* id;
    RoomType type;
} Vertex;


typedef struct Graph {
    Vertex* vertices;
    unsigned int ** adj;
    int size;
    int capacity;
} Graph;

int getCapacity(int index){
    return primeNum[index];
}

Graph* createGraph() {
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    if(graph != NULL){
        graph->vertices = (Vertex*)malloc(getCapacity(0) * sizeof(Vertex));
        if(graph->vertices != NULL){
            graph->size = 0;
            graph->capacity = 0;
            graph->adj = (unsigned int**)malloc(getCapacity(0) * sizeof(unsigned int *));
            for (int i = 0; i < getCapacity(0); i++) {
                graph->adj[i] = (unsigned int *)malloc(getCapacity(0)* sizeof(unsigned int));
                for(int j = 0; j < getCapacity(0); j++){
                    graph->adj[i][j] = UINT_MAX;
                }
            }
        }else{
            free(graph);
        }
    }
    return graph;
}

int searchVertex(Graph *graph, const char* id){
    for(int i = 0; i < graph->size; i++){
        if(strcmp(graph->vertices[i].id, id) == 0){
            return i;
        }
    }
    return -1;
}

int addVertex(Graph* graph, const char* id, RoomType type) {
    if(graph == NULL) return 1;
    if(searchVertex(graph, id) != -1) return 1;
    if(graph->size == getCapacity(graph->capacity)){
        graph->capacity++;
        int cap = getCapacity(graph->capacity);
        graph->vertices = realloc(graph->vertices, cap * sizeof(Vertex));
        graph->adj = realloc(graph->adj, cap * sizeof(unsigned int*));
        for(int i = 0; i < graph->size; i++){
            graph->adj[i] = realloc(graph->adj[i], cap * sizeof(unsigned int));
            for(int j = graph->size; j < cap; j++){
                graph->adj[i][j] = UINT_MAX;
            }
        }
        for(int i = graph->size; i < cap; i++){
            graph->adj[i] = (unsigned int *)malloc(cap * sizeof(unsigned int));
            for(int j = 0; j < cap; j++){
                graph->adj[i][j] = UINT_MAX;
            }
        }
    }
    Vertex vertex;
    vertex.id = strdup(id);
    vertex.type = type;
    graph->vertices[graph->size++] = vertex;
    return 0;
}

int removeVertex(Graph* graph, const char* id) {
    if(graph == NULL) return 1;

    unsigned int ind = searchVertex(graph, id);
    if(ind == -1) return 1;

    for (int i = ind; i < graph->size - 1; i++) {
        graph->vertices[i] = graph->vertices[i + 1];
    }
    for (int j = 0; j < graph->size; j++) {
        graph->adj[j][ind] = UINT_MAX;
        graph->adj[ind][j] = UINT_MAX;
    }
    graph->size--;
    return 0;
}

int updateVertex(Graph* graph, const char* id, const char *new_id, RoomType new_type) {
    for(int i = 0;i < graph->size; i++){
        if(strcmp(graph->vertices[i].id, id) == 0){
            free(graph->vertices[i].id);
            graph->vertices[i].id = strdup(new_id);
            graph->vertices[i].type = new_type;
            return 0;
        }
    }
    return 1;
}

int pasteEdge(Graph* graph, const char* src_id, const char* dest_id, unsigned int value){
    unsigned int a = UINT_MAX;
    unsigned int b = UINT_MAX;
    for (int i = 0; i < graph->size; i++) {
        if(strcmp(src_id, graph->vertices[i].id) == 0) a = i;
        if(strcmp(dest_id, graph->vertices[i].id) == 0) b = i;
    }
    if(a == UINT_MAX || b == UINT_MAX) return 1;
    graph->adj[a][b] = value;
    return 0;
}

int addEdge(Graph* graph, const char* src_id, const char* dest_id, unsigned int length) {
    return pasteEdge(graph, src_id, dest_id, length);
}

int removeEdge(Graph* graph, const char* src_id, const char* dest_id) {
    return pasteEdge(graph, src_id, dest_id, UINT_MAX);
}

int updateEdge(Graph* graph, const char* src_id, const char* dest_id, unsigned int new_length) {
    return pasteEdge(graph, src_id, dest_id, new_length);
}


void printGraph(Graph* graph) {
    if(graph == NULL) return;
    for(int i = 0; i < graph->size; i++){
        printf("\t|%s", graph->vertices[i].id);
    }
    printf("\n");
    for(int i = 0; i < graph->size; i++){
        printf("%s\t", graph->vertices[i].id);
        for(int j = 0; j < graph->size; j++){
            if(graph->adj[i][j] == UINT_MAX){
                printf("|∞\t");
            } else{
                printf("|%d\t", graph->adj[i][j]);
            }

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
    for(int i = 0; i < graph->size; i++){
        fprintf(file, "    \"%s\";\n", graph->vertices[i].id);
    }
    for (int i = 0; i < graph->size; i++) {
        for(int j = 0; j < graph->size; j++){
            if(graph->adj[i][j] != UINT_MAX){
                fprintf(file, "    \"%s\" -> \"%s\" [label=\"%d\"];\n", graph->vertices[i].id, graph->vertices[j].id, graph->adj[i][j]);
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

int isReachable(Graph* graph, const char* src, const char* dest) {
    if(graph == NULL) return 1;
    int start = -1;
    int end = -1;
    for (int i = 0; i < graph->size; i++) {
        if(strcmp(src, graph->vertices[i].id) == 0) start = i;
        if(strcmp(dest, graph->vertices[i].id) == 0) end = i;
    }
    if(start == -1 || end == -1) return 1;

    int* visited = (int*)calloc(graph->size, sizeof(int));
    int* queue = (int*)malloc(graph->size * sizeof(int));
    int front = 0, rear = 0;

    visited[start] = 1;
    queue[rear++] = start;

    while (front < rear) {
        int current = queue[front++];
        if (current == end) {
            free(visited);
            free(queue);
            return 0;
        }
        for (int i = 0; i < graph->size; i++) {
            if (graph->adj[current][i] != UINT_MAX && !visited[i]) {
                visited[i] = 1;
                queue[rear++] = i;
            }
        }
    }
    free(visited);
    free(queue);
    return 1;
}


unsigned int shortestPath(Graph* graph, const char* src, const char* dest) {
    if(graph == NULL) return 1;

    int start = -1;
    int end = -1;
    for (int i = 0; i < graph->size; i++) {
        if(strcmp(src, graph->vertices[i].id) == 0) start = i;
        if(strcmp(dest, graph->vertices[i].id) == 0) end = i;
    }
    if(start == -1 || end == -1) return 1;

    unsigned int* dist = (unsigned int*)malloc(graph->size * sizeof(unsigned int));
    int* prev = (int*)malloc(graph->size * sizeof(int));
    int* visited = (int*)calloc(graph->size, sizeof(int));



    for (int i = 0; i < graph->size; i++) {
        dist[i] = UINT_MAX;
        prev[i] = -1;
    }
    dist[start] = 0;

    PriorityQueue* pq = createPriorityQueue(graph->size);
    push(pq, start, 0);

    while (!isEmpty(pq)) {
        PriorityQueueNode current = pop(pq);
        int u = current.vertex;

        if (visited[u]) continue;
        visited[u] = 1;

        for (int v = 0; v < graph->size; v++) {
            if (graph->adj[u][v] != UINT_MAX && !visited[v] && dist[u] != UINT_MAX && dist[u] + graph->adj[u][v] < dist[v]) {
                dist[v] = dist[u] + graph->adj[u][v];
                prev[v] = u;
                push(pq, v, dist[v]);
            }
        }
    }

    if (dist[end] != UINT_MAX) {
        unsigned int min = dist[end];
        int path[graph->size];
        int count = 0;
        for (int at = end; at != -1; at = prev[at]) {
            path[count++] = at;
        }
        printf("Path: ");
        for (int i = count - 1; i >= 0; i--) {
            printf("%s ", graph->vertices[path[i]].id);
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


void floydWarshall(Graph* graph, unsigned int** dist, int** next) {
    int V = graph->size;

    // Инициализация матриц dist и next
    for (int i = 0; i < V; i++) {
        for (int j = 0; j < V; j++) {
            if (i == j) {
                dist[i][j] = 0;
            } else if (graph->adj[i][j] != 0) {
                dist[i][j] = graph->adj[i][j];
            } else {
                dist[i][j] = UINT_MAX;
            }
            if (graph->adj[i][j] != 0) {
                next[i][j] = j;
            } else {
                next[i][j] = -1;
            }
        }
    }

    // Алгоритм Флойда-Уоршалла
    for (int k = 0; k < V; k++) {
        for (int i = 0; i < V; i++) {
            for (int j = 0; j < V; j++) {
                if (dist[i][k] != UINT_MAX && dist[k][j] != UINT_MAX &&
                    dist[i][k] + dist[k][j] < dist[i][j]) {
                    dist[i][j] = dist[i][k] + dist[k][j];
                    next[i][j] = next[i][k];
                }
            }
        }
    }
}


void findNearestExit(Graph* graph, const char *src) {
    if(graph == NULL) return;

    int start = -1;
    for (int i = 0; i < graph->size; i++) {
        if(strcmp(src, graph->vertices[i].id) == 0) start = i;
    }
    if(start == -1) return;

    int V = graph->size;

    // Создаем и инициализируем матрицы для алгоритма Флойда-Уоршалла
    unsigned int** dist = (unsigned int**)malloc(V * sizeof(unsigned int*));
    int** next = (int**)malloc(V * sizeof(int*));
    for (int i = 0; i < V; i++) {
        dist[i] = (unsigned int*)malloc(V * sizeof(unsigned int));
        next[i] = (int*)malloc(V * sizeof(int));
    }

    // Выполняем алгоритм Флойда-Уоршалла
    floydWarshall(graph, dist, next);

    // Поиск ближайшего выхода
    unsigned int minDist = UINT_MAX;
    int nearestExit = -1;
    for (int i = 0; i < V; i++) {
        if (graph->vertices[i].type == EXIT && dist[start][i] < minDist) {
            minDist = dist[start][i];
            nearestExit = i;
        }
    }

    if (nearestExit != -1) {
        printf("Nearest exit from %s is %s with distance %u\n", graph->vertices[start].id, graph->vertices[nearestExit].id, minDist);

        // Печать пути
        printf("Path: %s", graph->vertices[start].id);
        int u = start;
        while (u != nearestExit) {
            u = next[u][nearestExit];
            printf(" -> %s", graph->vertices[u].id);
        }
        printf("\n");
    } else {
        printf("No exit reachable from %s\n", graph->vertices[start].id);
    }

    // Освобождение памяти
    for (int i = 0; i < V; i++) {
        free(dist[i]);
        free(next[i]);
    }
    free(dist);
    free(next);
}


void freeGraph(Graph* graph) {
    for(int i = 0; i < graph->size; i++){
        free(graph->vertices[i].id);
        free(graph->adj[i]);
    }
    free(graph->vertices);
    free(graph->adj);
    free(graph);
}

