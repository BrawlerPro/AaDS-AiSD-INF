#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "table.h"  // Подключаем нашу библиотеку с определениями таблицы

int main() {
    Table *table = NULL;
    int choice, size;
    char key[100], parentKey[100];
    unsigned int info;

    printf("Введите начальный размер таблицы: ");
    scanf("%d", &size);
    table = initTable(size);

    while (1) {
        printf("\nВыберите действие:\n");
        printf("1. Вставить элемент\n");
        printf("2. Удалить элемент\n");
        printf("3. Найти элементы по ключу родителя\n");
        printf("4. Вывести содержимое таблицы\n");
        printf("5. Выход\n");
        printf("Выбор: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("\nВведите ключ: ");
                scanf("%s", key);
                printf("Введите родительский ключ (или нажмите Enter для пустого ключа): ");
                getchar();  // Очистка буфера ввода
                fgets(parentKey, sizeof(parentKey), stdin);
                parentKey[strcspn(parentKey, "\n")] = '\0';  // Удаление символа новой строки
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
                scanf("%s", parentKey);
                Table *searchResult = searchByParentKey(table, parentKey);
                printf("Результаты поиска:\n");
                printTable(searchResult);
                freeTable(searchResult);
                break;
            case 4:
                printf("\nСодержимое таблицы:\n");
                printTable(table);
                break;
            case 5:
                printf("\nВыход из программы.\n");
                freeTable(table);
                exit(0);
            default:
                printf("\nОшибка: неверный выбор. Пожалуйста, выберите действие из списка.\n");
        }
    }

    return 0;
}
