#ifndef TABLE_H
#define TABLE_H

// Структуры данных
typedef struct KeySpace KeySpace;

// Таблица
typedef struct Table Table;

Table *initTable(int m_size);
int insertElement(Table *table, char *key, char *par, unsigned int info);
int deleteElement(Table *table, char *key);
Table *searchByParentKey(Table *table, char *parentKey);
void printTable(Table *table);
void freeTable(Table *table);

#endif /* TABLE_H */

