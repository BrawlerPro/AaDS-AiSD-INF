#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bitree.h"
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


int main() {
    Tree* tree = initTree();
    unsigned int info, size;
    char *key;
    int choice;

    do {
        printf("1. Добавить элемент\n");
        printf("2. Удалить элемент\n");
        printf("3. Найти элемент\n");
        printf("4. Найти ближайший элемент\n");
        printf("5. Вывести дерево\n");
        printf("6. Визуализировать дерево\n");
        printf("7. Добавить случайные элементы(Время + Кол-во)\n");
        printf("8. Удалить случайные элементы(Время + Кол-во)\n");
        printf("9. Найти случайные элементы(Время + Кол-во)\n");
        printf("10. Выйти\n");
        printf("Выберите действие: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1: {
                printf("Введите ключ:  ");
                key = readline();
                printf("Введите число:  ");
                scanf("%d", &info);
                getchar();
                if(insertNode(tree, key, info) == 0) {
                    printf("Success \n");
                }else printf("Not success \n");
                free(key);
                break;
            }
            case 2: {
                printf("Введите ключ для удаления: ");
                key = readline();
                if(deleteNode(tree, key) == 0) {
                    printf("Success\n");
                }else printf("Not success\n");
                free(key);
                break;
            }
            case 3: {
                printf("Введите ключ для поиска: ");
                key = readline();
                UNode *result = searchNode(tree, key);
                if (result != NULL) {
                    printf("Найден элемент: Ключ: %s, Число: %u\n", result->key, result->value);
                    free(result->key);
                    free(result);
                } else {
                    printf("Элемент с ключом '%s' не найден.\n", key);
                }
                free(key);
                break;
            }
            case 4: {
                printf("Введите ключ для поиска: ");
                key = readline();
                UNode *result = findClosestKey(tree, key);
                if (result != NULL) {
                    printf("Найденый ближайший элемент: Ключ: %s, Число: %u\n", result->key, result->value);
                    free(result->key);
                    free(result);
                } else {
                    printf("Элемент с ключом '%s' не найден.\n", key);
                }
                free(key);
                break;
            }
            case 5: {
                printf("Дерево:\n");
                preorderTraversal(tree);
                break;
            }
            case 6:{
                visualizeTree(tree);
                break;
            }
            case 7:{
                printf("Введите сколько элементов нужно добавить\n");
                scanf("%d", &size);
                getchar();
                insertRandomString(tree, size);
                break;
            }
            case 8: {
                printf("Введите сколько элементов нужно добавить\n");
                scanf("%d", &size);
                getchar();
                deleteRandomString(tree, size);
                break;
            }
            case 9:{
                printf("Введите сколько элементов нужно найти\n");
                scanf("%d", &size);
                getchar();
                searchRandomNode(tree, size);
                break;
            }
            case 10:
                break;

            default:
                printf("Ошибка: Некорректный выбор. Попробуйте снова.\n");
                break;
        }
    } while (choice != 10);
    deleteTree(tree);
    // Освобождение памяти и т.д.

    return 0;
}
