#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct TableElement{
    int key;
    char* info;
    struct TableElement* next;
} TableElement;

typedef struct Table{
    TableElement **elements;
    int capacity;
    int size;
} Table;

// Функция хеширования ключа
int hash(int key, int size) {
    return key % size;
}

Table *initTable(int capacity) {
    Table *table = (Table *) malloc(sizeof(Table));
    if (table != NULL){
        table->elements = (TableElement **) malloc(capacity * sizeof(TableElement*));
        if (table->elements == NULL) {
            free(table);
            return NULL;
        }
        table->size = 0;
        table->capacity = capacity;
    }
    return table;
}


// Функция вставки элемента в таблицу
int insertElement(Table *table, int key, const char *data) {
    int index = hash(key, table->capacity);
    if(table->elements[index] == NULL){
        TableElement* tmp = (TableElement*) malloc(sizeof(TableElement));
        tmp->key=key;
        tmp->info= strdup(data);
        table->elements[index] = tmp;
        table->size++;
        return 0;
    }else {
        TableElement* temp = table->elements[index];
        while (temp != NULL){
            if(temp->key == key){
                return 1;
            }
            temp = temp->next;
        }
        TableElement* tmp = (TableElement*) malloc(sizeof(TableElement));
        tmp->key = key;
        tmp->info = strdup(data);
        tmp->next = table->elements[index];
        table->elements[index] = tmp;
        table->size++;
        return 0;
    }
}

// Функция удаления элемента из таблицы
int deleteElement(Table *table, int key) {
    int index = hash(key, table->capacity);
    if(table->elements[index] == NULL){
        return 1;
    }else {
        TableElement* temp = table->elements[index];
        if(temp->key == key){
            table->elements[index] = temp->next;
            free(temp->info);
            free(temp);
            table->size--;
            return 0;
        }else{
            while (temp != NULL){
                if(temp->next->key == key){
                    TableElement *temp1 = temp->next;
                    free(temp1->info);
                    temp->next=temp1->next;
                    free(temp1);
                    table->size--;
                    return 0;
                }
                temp = temp->next;
            }
            return 1;
        }

    }
}