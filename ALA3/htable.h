#ifndef HTABLE_H
#define HTABLE_H

typedef struct KeySpace KeySpace;

typedef struct Table Table;


Table *initTable(int capacity);
int insertElement(Table *table, unsigned int key, const char *info);
int deleteElement(Table *table, unsigned int key);
KeySpace *searchElement(Table *table, unsigned int key);
void printTable(Table *table);
int exportTable(Table *table, const char *filename);
int importTable(Table *table, const char *filename);
void freeTable(Table *table);

#endif /* HTABLE_H */
