#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "htable.h"  // Подключаем нашу библиотеку с определениями таблицы

char *readline() {
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
    UsKeySP *test = NULL;
    Table *table = NULL;
    int choice;
    unsigned int key;
    char *info, *filename;

    table = initTable(3);

    while (1) {
        printf("\nВыберите действие:\n");
        printf("1. Вставить элемент\n");
        printf("2. Удалить элемент\n");
        printf("3. Найти элемент по ключу\n");
        printf("4. Вывести содержимое таблицы\n");
        printf("5. Импорт таблицы в бинарник\n");
        printf("6. Экспорт таблицы в бинарник\n");
        printf("7. Выход\n");
        printf("Выбор: ");
        scanf("%d", &choice);
        getchar();

        switch (choice) {
            case 1:
                printf("\nВведите ключ: ");
                scanf("%d", &key);
                getchar();
                printf("Введите информацию: ");
                info = readline();
                if (insertElement(table, key, info) == 0) {
                    printf("Элемент успешно добавлен в таблицу.\n");
                } else {
                    printf("Ошибка: не удалось добавить элемент в таблицу.\n");
                }
                free(info);
                break;
            case 2:
                printf("\nВведите ключ элемента для удаления: ");
                scanf("%d", &key);
                getchar();
                if (deleteElement(table, key) == 0) {
                    printf("Элемент успешно удален из таблицы.\n");
                } else {
                    printf("Ошибка: элемент не найден или не удалось удалить.\n");
                }
                break;
            case 3:
                printf("\nВведите ключ для поиска элемента: ");
                scanf("%d", &key);
                getchar();

                printf("Результаты поиска:\n");
                test = searchElement(table, key);
                printf("%d, %s", test->key, test->info);
                free(test->info);
                free(test);
                break;
            case 4:
                printTable(table);
                break;
            case 5:
                printf("\nВведите имя файла:\n");
                filename = readline();
                if (!importTable(table, filename)) {

                    printf("Таблица успешно импортирована\n");
                    printTable(table);
                } else {
                    printf("Ошибка чтения или файла не существует");
                }
                free(filename);
                break;
            case 6:
                printf("\nВведите имя файла:\n");
                filename = readline();
                if (!exportTable(table, filename)) {

                    printf("Таблица успешно Экспортировалась\n");
                } else {
                    printf("Ошибка создания");
                }
                free(filename);
                break;
            case 7:
                freeTable(table);
                return 0;
            default:
                printf("\nОшибка: неверный выбор. Пожалуйста, выберите действие из списка.\n");
        }
    }
}
