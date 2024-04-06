#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct KeySpace {
    int busy;
    unsigned int key;
    char *info;
} KeySpace;

typedef struct Table {
    KeySpace *ks;
    int capacity;
    int size;
} Table;

// Инициализация таблицы
Table *initTable(int capacity) {
    Table *table = (Table *) malloc(sizeof(Table));

    if (table == NULL) return NULL;

    table->ks = (KeySpace *) malloc(capacity * sizeof(KeySpace));

    if (table->ks == NULL){
        free(table);
        return NULL;
    }

    table->capacity = capacity;
    table->size = 0;
    return table;
}

unsigned int hash1(unsigned int key, int capacity) {
    return key % capacity;
}

unsigned int hash2(unsigned int key, int capacity) {
    return 1 + (key % (capacity - 1));
}

int isFull(Table *table) {
    return table->size == table->capacity;
}

int getNextPrime(int n) {
    int i, j;
    for (i = n + 1;; i++) {
        for (j = 2; j * j <= i; j++) {
            if (i % j == 0) {
                break;
            }
        }
        if (j * j > i) {
            return i;
        }
    }
}

int resizeTable(Table *table) {
    if(table == NULL) return 1;

    int newSize = getNextPrime(table->capacity);
    Table newTable;
    newTable.capacity = newSize;
    newTable.size = 0;
    newTable.ks = malloc(newSize * sizeof(KeySpace));

    if(newTable.ks == NULL) return 1;

    for (int i = 0; i < newSize; i++) {
        newTable.ks[i].busy = 0;
        newTable.ks[i].key = 0;
        newTable.ks[i].info = NULL;
    }

    for (int i = 0; i < table->capacity; i++) {
        if (table->ks[i].busy == 1) {
            unsigned int key = table->ks[i].key;
            char *info = table->ks[i].info;
            int index = hash1(key, newSize);
            int step = hash2(key, newSize);
            for(int j = 0; j < newSize; j++){
                if (newTable.ks[index].busy == 0) {
                    newTable.ks[index].busy = 1;
                    newTable.ks[index].key = key;
                    newTable.ks[index].info = info;
                    newTable.size++;
                }
                index = (index + step) % newSize;
            }
        }
    }

    free(table->ks);
    table->ks = newTable.ks;
    table->capacity = newSize;
    return 0;
}

KeySpace* searchElement(struct Table *table, unsigned int key) {
    KeySpace *foundElement = malloc(sizeof(struct KeySpace));
    if(foundElement == NULL) return NULL;
    int index = hash1(key, table->capacity);
    int step = hash2(key, table->capacity);
    for(int i = 0; i < table->capacity; i++) {
        if (table->ks[index].key == key) {
            foundElement->busy = table->ks[index].busy;
            foundElement->key = table->ks[index].key;
            foundElement->info = table->ks[index].info;
            return foundElement;  // Элемент был найден
        }
        index = (index + step) % table->capacity;
    }
    free(foundElement);
    return NULL;  // Элемент не был найден
}

int insertElement(Table *table, unsigned int key, char *info) {
    if (isFull(table)) {
        resizeTable(table);
    }

    if(searchElement(table, key) != NULL) return 1;

    int index = hash1(key, table->capacity);
    int step = hash2(key, table->capacity);
    for(int i = 0; i < table->capacity; i++){
        if(table->ks[index].busy == 0){
            table->ks[index].busy = 1;
            table->ks[index].key = key;
            table->ks[index].info = info;
            table->size++;
            return 0;
        }
        index = (index + step) % table->capacity;
    }
    return 1;

}

int deleteElement(Table *table, unsigned int key) {
    int index = hash1(key, table->capacity);
    int step = hash2(key, table->capacity);
    for(int i = 0; i < table->capacity; i++) {
        if (table->ks[index].key == key) {
            table->ks[index].busy = 0;
            table->ks[index].key = 0;
            table->ks[index].info = NULL;
            table->size--;
            return 0;
        }
        index = (index + step) % table->capacity;
    }
    return 1;
}

void printTable(Table *table) {
    printf("Table Contents:\n");
    printf("Index\tKey\tInfo\n");
    for (int i = 0; i < table->size; i++) {
        if (table->ks[i].busy == 1) {
            printf("%d\t%u\t%s\n", i, table->ks[i].key, table->ks[i].info);
        } else {
            printf("%d\t-\t-\n", i);
        }
    }
}

// Функция для экспорта данных из таблицы в бинарный файл
int exportTable(Table *table, const char *filename) {
    FILE *file = fopen(filename, "wb");
    if (file == NULL) {
        return 1;  // Не удалось открыть файл
    }

    fwrite(&table->size, sizeof(int), 1, file);
    fwrite(&table->capacity, sizeof(int), 1, file);

    for (int i = 0; i < table->size; i++) {
        fwrite(&table->ks[i].busy, sizeof(int), 1, file);
        fwrite(&table->ks[i].key, sizeof(unsigned int), 1, file);

        // Запись длины информации и самой информации
        int infoLength = strlen(table->ks[i].info);
        fwrite(&infoLength, sizeof(int), 1, file);
        fwrite(table->ks[i].info, sizeof(char), infoLength, file);
    }

    fclose(file);
    return 0;  // Экспорт прошел успешно
}

// Функция для импорта данных из бинарного файла в таблицу
int importTable(Table *table, const char *filename) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) return 1;  // Не удалось открыть файл

    Table *newTable = (Table*) malloc(sizeof(Table));

    if(newTable == NULL) {
        fclose(file);
        return 1; // Не удалось выделить память
    }

    fread(&newTable->size, sizeof(int), 1, file);
    fread(&newTable->capacity, sizeof(int), 1, file);
    newTable->ks = (KeySpace*) malloc(newTable->capacity*sizeof(KeySpace));
    for (int i = 0; i < newTable ->size; i++) {
        fread(&newTable->ks[i].busy, sizeof(int), 1, file);
        fread(&newTable->ks[i].key, sizeof(unsigned int), 1, file);

        // Чтение длины информации
        int infoLength;
        fread(&infoLength, sizeof(int), 1, file);

        // Выделение памяти для информации и чтение самой информации
        newTable->ks[i].info = malloc((infoLength + 1) * sizeof(char));
        fread(newTable->ks[i].info, sizeof(char), infoLength, file);
        newTable->ks[i].info[infoLength] = '\0';
    }

    free(table->ks);
    table = newTable;
    fclose(file);
    return 0;  // Импорт прошел успешно
}


void freeTable(Table *table) {
    if (table == NULL) return;

    for (int i = 0; i < table->capacity; i++) {
        if (table->ks[i].busy == 1) {
            free(table->ks[i].info);
        }
    }

    free(table->ks);
    free(table);
}
