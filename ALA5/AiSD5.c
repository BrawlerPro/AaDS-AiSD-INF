#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "graph.h"
#include "from_file.h"

char* readline() {
    char *res = NULL;
    size_t len = 1;
    char buf[81] = {};

    int scan_res = 0;
    while ((scan_res = scanf("%80[^\n]s", buf)) != EOF && scan_res != 0) {
        len += 80;
        res = (char *) realloc(res, len * sizeof(char *));
        if (res == NULL) return NULL;
        strncpy(res + len - 81, buf, 80);
    }
    if (scan_res == 0 && res == NULL) res = (char *) calloc(1, sizeof(char));
    getchar();
    return res;
}

void print_menu() {
    printf("Меню операций с графом:\n");
    printf("1. Добавить вершину\n");
    printf("2. Добавить ребро\n");
    printf("3. Удалить вершину\n");
    printf("4. Удалить ребро\n");
    printf("5. Считать из файла Вершины\n");
    printf("6. Считать из файла Рёбра\n");
    printf("7. Вывести граф\n");
    printf("8. Визиаулизировать Граф\n");
    printf("9. Поиск в ширину (BFS)\n");
    printf("10. Поиск кратчайшего пути (Dijkstra)\n");
    printf("11. Найти ближайший выход (Floyd-Warshall)\n");
    printf("12. Выйти\n");
}

RoomType getRoomType() {
    int type;
    do{
        printf("Введите тип комнаты (0: ENTRANCE(Вход), 1: EXIT(Выход), 2: TRANSITION(Переход)): ");
        scanf("%d", &type);
        getchar();
        if(type == 0) return ENTRANCE;
        if(type == 1) return EXIT;
        if(type == 2) return TRANSITION;
    } while (1);
}

int main() {
    Graph* graph = createGraph();
    int choice = 0;
    char *id, *src_id, *dest_id, *new_id;
    RoomType type;
    int length, distance;

    while (choice != 12) {
        print_menu();
        printf("Введите ваш выбор: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1:
                printf("Введите ID вершины:");
                id = readline();
                type = getRoomType();
                addVertex(graph, id, type);

                free(id);
                break;
            case 2:
                printf("Введите ID исходной вершины: ");
                src_id = readline();
                printf("Введите ID конечной вершины: ");
                dest_id = readline();
                printf("Введите длину ребра: ");
                scanf("%d", &length);
                getchar();
                addEdge(graph, src_id, dest_id, length);
                free(src_id);
                free(dest_id);
                break;
            case 3:
                printf("Введите ID вершины для удаления: ");
                id = readline();
                removeVertex(graph, id);
                free(id);
                break;
            case 4:
                printf("Введите ID исходной вершины: ");
                src_id = readline();
                printf("Введите ID конечной вершины: ");
                dest_id = readline();
                removeEdge(graph, src_id, dest_id);
                free(src_id);
                free(dest_id);
                break;
            case 5:
                printf("Введите имя файла: ");
                id = readline();
                readVerticesFromFile(graph, id);
                free(id);
                break;
            case 6:
                printf("Введите имя файла: ");
                id = readline();
                readEdgesFromFile(graph, id);
                free(id);
                break;
            case 7:
                printGraph(graph);
                break;
            case 8:
                visualizeGraph(graph);
                break;
            case 9:
                printf("Введите ID начальной вершины: ");
                src_id = readline();
                printf("Введите ID конечной вершины: ");
                dest_id = readline();
                if (isReachable(graph, src_id, dest_id)) {
                    printf("Путь найден от %s до %s\n", src_id, dest_id);
                } else {
                    printf("Путь не найден от %s до %s\n", src_id, dest_id);
                }
                free(src_id);
                free(dest_id);
                break;
            case 10:
                printf("Введите ID начальной вершины: ");
                src_id = readline();
                printf("Введите ID конечной вершины: ");
                dest_id = readline();
                distance = shortestPath(graph, src_id, dest_id);
                if (distance == INT_MAX) {
                    printf("Путь не найден от %s до %s\n", src_id, dest_id);
                } else {
                    printf("Кратчайший путь от %s до %s составляет %d\n", src_id, dest_id, distance);
                }
                free(src_id);
                free(dest_id);
                break;
            case 11:
                printf("Введите ID начала лабиринта: ");
                src_id = readline();
                findNearestExit(graph, src_id);
                free(src_id);
            case 12:
                break;
            default:
                printf("Неверный выбор. Пожалуйста, попробуйте снова.\n");
        }
    }
    freeGraph(graph);
    return 0;
}
