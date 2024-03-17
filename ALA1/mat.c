#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "matrix.h"

typedef struct Row{
    int* data;
    int size;
} Row;

typedef struct Matrix{
    Row **rows;
    int size;
} Matrix;

Row* ArrToRow(const int* arr, int size) {
    Row *row = (Row*) malloc(sizeof(Row));
    row->data = (int*)malloc(size * sizeof(int));
    row->size = size;
    for (int i = 0; i < size; ++i) {
        row->data[i] = arr[i];
    }
    return row;
}

void printMatrix(Matrix* matrix){
    for (int i = 0; i < matrix->size; ++i) {
        printf("Row %d: ", i + 1);
        for (int j = 0; j < matrix->rows[i]->size; ++j) {
            printf("%d ", matrix->rows[i]->data[j]);
        }
        printf("\n");
    }
}

void processMatrix(Matrix *matrix) {
    int maxPositivesRow = -1;
    int maxNegativesRow = -1;
    int maxPositivesCount = 0;
    int maxNegativesCount = 0;

    for (int i = 0; i < matrix->size; i++) {
        int positivesCount = 0;
        int negativeCount = 0;
        for (int j = 0; j < matrix->rows[i]->size; j++) {
            if (matrix->rows[i]->data[j] > 0) {
                positivesCount++;
            } else {
                negativeCount++;
            }
        }
        if(negativeCount > positivesCount){
            if(negativeCount > maxNegativesCount){
                maxNegativesCount =  negativeCount;
                maxNegativesRow = i;
            }
        } else {
            if(positivesCount > maxPositivesCount){
                maxPositivesCount = positivesCount;
                maxPositivesRow = i;
            }
        }
    }

    if((maxPositivesRow == matrix->size - 1) & (maxNegativesRow == 0)){
        swapRows(matrix, maxNegativesRow, maxPositivesRow);
        return;
    }
    if(maxNegativesRow == 0){
        swapRows(matrix, maxNegativesRow, matrix->size - 1);
        swapRows(matrix, maxPositivesRow, 0);
        return;
    } else {
        swapRows(matrix, maxPositivesRow, 0);
        swapRows(matrix, maxNegativesRow, matrix->size - 1);
    }


}


//void writeToFile(const char* filename, Matrix* matrix) {
//    FILE* file = fopen(filename, "w");
//    if (file == NULL) {
//        fprintf(stderr, "Error opening file %s\n", filename);
//        return;
//    }
//
//    for (int i = 0; i < matrix->size; ++i) {
//        for (int j = 0; j < matrix->rows[i].size; ++j) {
//            fprintf(file, "%d ", matrix->rows[i].data[j]);
//        }
//        fprintf(file, "\n");
//    }
//
//    fclose(file);
//}

//Matrix* readFromFile(const char* filename) {
//    FILE* file = fopen(filename, "r");
//    if (file == NULL) {
//        fprintf(stderr, "Error opening file %s\n", filename);
//        return NULL;
//    }
//
//    Matrix* matrix = (Matrix*)malloc(sizeof(Matrix));
//    matrix->rows = NULL;
//    matrix->size = 0;
//
//    char* line = NULL;
//    size_t len = 0;
//    ssize_t read;
//    while ((read = getline(&line, &len, file)) != -1) {
//        Row* row = (Row*)malloc(sizeof(Row));
//        row->data = NULL;
//        row->size = 0;
//
//        int value;
//        char* token = strtok(line, " \n");
//        while (token != NULL) {
//            value = atoi(token);
//            row->data = (int*)realloc(row->data, (row->size + 1) * sizeof(int));
//            row->data[row->size++] = value;
//            token = strtok(NULL, " \n");
//        }
//
//        matrix->rows = (Row*)realloc(matrix->rows, (matrix->size + 1) * sizeof(Row));
//        matrix->rows[matrix->size++] = *row;
//    }
//
//    free(line);
//    fclose(file);
//    return matrix;
//}

//int main() {
//    Matrix* matrix = readFromFile("data.txt");
//    if (matrix != NULL) {
//        printf("Matrix read from file:\n");
//        for (int i = 0; i < matrix->size; ++i) {
//            printf("Row %d: ", i + 1);
//            for (int j = 0; j < matrix->rows[i].size; ++j) {
//                printf("%d ", matrix->rows[i].data[j]);
//            }
//            printf("\n");
//        }
//
//        writeToFile("output.txt", matrix);
//        printf("Matrix written to file 'output.txt'\n");
//
//        free(matrix);
//    }
//
//    return 0;
//}
//
//int main() {
//    Matrix* matrix = readFromFile("ALA1/input.txt");
//    if (matrix != NULL) {
//        printf("Matrix read from file:\n");
//        for (int i = 0; i < matrix->size; ++i) {
//            printf("Row %d: ", i + 1);
//            for (int j = 0; j < matrix->rows[i].size; ++j) {
//                printf("%d ", matrix->rows[i].data[j]);
//            }
//            printf("\n");
//        }
//        free(matrix);
//    }
//
//    int arr[] = {1, 2, 3, 4, 5};
//    Row* row = ArrToRow(arr, sizeof(arr) / sizeof(arr[0]));
//    printf("Row from array: ");
//    for (int i = 0; i < row->size; ++i) {
//        printf("%d ", row->data[i]);
//    }
//    printf("\n");
//    free(row);
//
//    return 0;
//}
