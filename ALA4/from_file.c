#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "ffile.h"
#include "bitree.h"

char* randomString(int minLength, int maxLength) {
    int length = minLength + rand() % (maxLength - minLength + 1);
    char* str = (char*) malloc((length + 1) * sizeof(char));

    if (str == NULL) return NULL;

    const char charset[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    int charsetSize = sizeof(charset) - 1;

    for (int i = 0; i < length; ++i) {
        int index = rand() % charsetSize;
        str[i] = charset[index];
    }

    str[length] = '\0';
    return str;
}

char** createRandomStrings(unsigned int size){
    char** strings = (char**) malloc(sizeof(char*) * size);

    for(int i = 0; i < size; i++){
        strings[i] = randomString(5, 20);
    }
    return strings;
}

void freeStrings(char **strings, unsigned int size){
    if(strings == NULL) return;
    for(int i = 0; i < size; i++){
        if(strings[i] != NULL) {
            free(strings[i]);
        }
    }
    free(strings);
}

void insertRandomString(Tree* tree, unsigned int size) {
    clock_t start, end;
    double cpu_time_used;

    char** strings = createRandomStrings(size);

    start = clock();
    for(int i = 0; i < size; i++) {
        insertNode(tree, strings[i], 0);
    }
    end = clock();

    freeStrings(strings, size);

    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Время выполнения: %f, Кол-во элементов: %d\n", cpu_time_used, getSize(tree));
}

void deleteRandomString(Tree* tree, unsigned int size) {
    clock_t start, end;
    double cpu_time_used;

    char** strings = createRandomStrings(size);

    start = clock();
    for(int i = 0; i < size; i++) {
        deleteNode(tree, strings[i]);
    }
    end = clock();

    freeStrings(strings, size);

    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Время выполнения: %f, Кол-во элементов: %d\n", cpu_time_used, getSize(tree));
}

void searchRandomNode(Tree* tree, unsigned int size) {
    clock_t start, end;
    double cpu_time_used;

    char** strings = createRandomStrings(size);

    start = clock();
    for(int i = 0; i < size; i++) {
        searchNode(tree, strings[i]);
    }
    end = clock();

    freeStrings(strings, size);

    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("Время выполнения: %f, Кол-во элементов: %d\n", cpu_time_used, getSize(tree));
}