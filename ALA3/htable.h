#ifndef HTABLE_H
#define HTABLE_H

typedef struct UsKeySP{
    unsigned int key;
    char *info;
}UsKeySP;

typedef struct Table Table;

Table *initTable(int capacity);

int insertElement(Table *table, unsigned int key, const char *info);

int deleteElement(Table *table, unsigned int key);

UsKeySP *searchElement(Table *table, unsigned int key);

void printTable(Table *table);

void freeTable(Table *table);

int exportTable(Table *table, const char *filename);

int importTable(Table *table, const char *filename);

#endif /* HTABLE_H */
