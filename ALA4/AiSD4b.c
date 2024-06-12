#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "B_tree.h"
#include "ffile.h"

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

void menu() {
    printf("1. Добавить элемент\n");
    printf("2. Удалить элемент\n");
    printf("3. Вывод дерева\n");
    printf("4. Поиск элемента по ключу\n");
    printf("5. Визуализировать дерево\n");
    printf("6. Добавить в дерево n кол-во элементов\n");
    printf("7. Удалить из дерева n кол-во элементов\n");
    printf("8. Найти в дереве n кол-во элементов\n");
    printf("0. Выход\n");
}

int main() {
    Tree *tree = createTree();
    int choice = 10;
    char *key, *filename;
    unsigned int value;

    while (choice != 0) {
        menu();
        printf("Введите ваш выбор: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1:
                printf("Введите ключ: ");
                key = readline();
                printf("Введите значение: ");
                scanf("%u", &value);
                getchar();
                insert(tree, key, value);
                free(key);
                break;
            case 2:
                printf("Введите ключ: ");
                key = readline();
                delete(tree, key);
                free(key);
                break;
            case 3:
                printTree(tree);
                break;
            case 4:
                printf("Введите ключ: ");
                key = readline();
                SearchResult* fd = search(tree, key);
                SearchResult* found = fd;
                printf("Для ключа %s найденые значения:", found->info->key);
                free(key);
                while(found != NULL){
                    printf("%u ", found->info->value);
                    found = found->next;
                }
                printf("\n");
                freeSearchRes(fd);
                break;
            case 5:
                visualizeTree(tree);
                break;
            case 6:
                printf("Введите кол-во элементов: ");
                scanf("%u", &value);
                getchar();
                insertRandomString(tree, (int) value);
                break;
            case 7:
                printf("Введите кол-во элементов: ");
                scanf("%u", &value);
                getchar();
                deleteRandomString(tree, (int) value);
                break;
            case 8:
                printf("Введите кол-во элементов: ");
                scanf("%u", &value);
                getchar();
                searchRandomNode(tree, (int) value);
                break;
            case 0:
                break;
            default:
                printf("Неверный выбор\n");
        }
    }
    freeTree(tree);
    return 0;
}
