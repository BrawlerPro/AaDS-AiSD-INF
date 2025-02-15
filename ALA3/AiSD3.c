#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "table.h"  // Подключаем нашу библиотеку с определениями таблицы

char *readline() {
    char *res = NULL;
    size_t len = 1;
    char buf[81] = {};

    int scan_res = 0;
    while ((scan_res = scanf("%80[^\n]s", buf)) != EOF && scan_res != 0) {
        len += 80;
        res = (char *)realloc(res, len * sizeof(char *));
        if (res == NULL) return NULL;
        strncpy(res + len - 81, buf, 80);
    }
    if (scan_res == 0 && res == NULL) res = (char *)calloc(1, sizeof(char));
    getchar();
    return res;
}


int main() {
    Table *table = NULL;
    int choice, size;
    char *key, *parentKey;
    unsigned int info;

    printf("Введите начальный размер таблицы: ");
    scanf("%d", &size);
    table = initTable(size);

    while (1) {
        printf("\nВыберите действие:\n");
        printf("1. Вставить элемент\n");
        printf("2. Удалить элемент\n");
        printf("3. Найти элементы по ключу родителя\n");
        printf("4. Найти элементы по ключу\n");
        printf("5. Вывести содержимое таблицы\n");
        printf("6. Выход\n");
        printf("Выбор: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1:
                printf("\nВведите ключ: ");
                key = readline();
                printf("Введите родительский ключ (или нажмите Enter для пустого ключа): ");
                parentKey = readline();
                printf("Введите информацию: ");
                scanf("%u", &info);
                if (insertElement(table, key, parentKey, info) == 0) {
                    printf("Элемент успешно добавлен в таблицу.\n");
                } else {
                    printf("Ошибка: не удалось добавить элемент в таблицу.\n");
                }
                break;
            case 2:
                printf("\nВведите ключ элемента для удаления: ");
                scanf("%s", key);
                if (deleteElement(table, key) == 0) {
                    printf("Элемент успешно удален из таблицы.\n");
                } else {
                    printf("Ошибка: элемент не найден или не удалось удалить.\n");
                }
                break;
            case 3:
                printf("\nВведите ключ родителя для поиска элементов: ");
                parentKey = readline();
                Table *searchResult = searchByParentKey(table, parentKey);
                printf("Результаты поиска:\n");
                printTable(searchResult);
                freeTable(searchResult);
                break;
            case 4:
                printf("\nВведите ключ для поиска элементов: ");
                key = readline();
                Table *result = searchByKey(table, key);
                printf("Результаты поиска:\n");
                printTable(result);
                freeTable(result);
                break;
            case 5:
                printf("\nСодержимое таблицы:\n");
                printTable(table);
                break;
            case 6:
                printf("\nВыход из программы.\n");
                freeTable(table);
                return 0;
            default:
                printf("\nОшибка: неверный выбор. Пожалуйста, выберите действие из списка.\n");
        }
    }

    return 0;
}
