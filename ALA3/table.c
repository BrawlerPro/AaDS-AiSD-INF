#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// Структуры данных
typedef struct KeySpace {
    // ненулевой ключ элемента
    char *key;
    // ключ родительского элемента, может быть нулевым
    char *par;
    // Информация
    unsigned int info;
} KeySpace;

// Таблица
typedef struct Table {
    KeySpace *ks;
    int m_size;
    int c_size;
} Table;

// Инициализация таблицы
Table *initTable(int m_size) {
    Table *table = (Table *) malloc(sizeof(Table));

    if (table == NULL) return NULL;

    table->ks = (KeySpace *) malloc(m_size * sizeof(KeySpace));

    if (table->ks == NULL){
        free(table);
        return NULL;
    }

    table->m_size = m_size;
    table->c_size = 0;
    return table;
}



// Вставка нового элемента с проверками на уникальность ключей и соответствие ключа родителя
int insertElement(Table *table, char *key, char *par, unsigned int info) {
    if (table->c_size >= table->m_size) return 1;

    // Проверка на уникальность ключа
    for (int i = 0; i < table->c_size; i++) {
        if (strcmp(table->ks[i].key, key) == 0) return 1;
    }

    // Проверка соответствия ключа родителя
    if (strcmp(par, "") != 0) {  // Если ключ родителя не пустой
        int found = 0;
        for (int i = 0; i < table->c_size; i++) {
            if (strcmp(table->ks[i].key, par) == 0) {
                found = 1;
                break;
            }
        }
        if (!found) return 1;
    }

    table->ks[table->c_size].key = strdup(key);
    table->ks[table->c_size].par = strdup(par);
    table->ks[table->c_size].info = info;
    table->c_size++;
    return 0;
}


// Удаление элемента по ключу с обновлением ключей родителей
int deleteElement(Table *table, char *key) {
    for (int i = 0; i < table->c_size; i++) {
        if (strcmp(table->ks[i].key, key) == 0) {
            // Удаление элемента
            free(table->ks[i].key);
            table->ks[i].key = NULL;
            // Обновление ключей родителей
            for (int j = 0; j < table->c_size; j++) {
                if (strcmp(table->ks[j].par, key) == 0) {
                    free(table->ks[j].par);
                    table->ks[j].par = strdup("");
                }
            }
            return 0;
        }
    }
    return 1;
}

// Поиск элемента по ключю
Table *searchByKey(Table *table, char *key){
    Table *resultTable = initTable(table->m_size);
    for (int i = 0; i < table->c_size; i++) {
        if (strcmp(table->ks[i].key, key) == 0) {
            insertElement(resultTable, table->ks[i].key, table->ks[i].par, table->ks[i].info);
        }
    }
    return resultTable;
}

// Поиск всех элементов с заданным значением ключа родителя
Table *searchByParentKey(Table *table, char *parentKey) {
    Table *resultTable = initTable(table->m_size);
    for (int i = 0; i < table->c_size; i++) {
        if (strcmp(table->ks[i].par, parentKey) == 0) {
            insertElement(resultTable, table->ks[i].key, table->ks[i].par, table->ks[i].info);
        }
    }
    return resultTable;
}

// Вывод содержимого таблицы
void printTable(Table *table) {
    for (int i = 0; i < table->c_size; i++) {
        printf("Элемент %d: ключ=%s, родительский ключ=%s\n", i + 1, table->ks[i].key, table->ks[i].par);
    }
}

// Освобождение памяти, выделенной под таблицу
void freeTable(Table *table) {
    if (table != NULL) {
        for (int i = 0; i < table->c_size; i++) {
            free(table->ks[i].key);
            free(table->ks[i].par);
        }
        free(table->ks);
        free(table);
    }
}
