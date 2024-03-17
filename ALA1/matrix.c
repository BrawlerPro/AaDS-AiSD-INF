#include <stdio.h>
#include <stdlib.h>

typedef struct Row{
    int* data;
    int size;
} Row;

typedef struct{
    Row **rows;
    int size;
} Matrix;


Matrix *initMat() {
    Matrix *matrix = (Matrix*) malloc(sizeof(Matrix));
    if (matrix == NULL) {
        return NULL;
    }
    matrix->rows = (Row**) malloc (sizeof(Row*));
    matrix->size = 0;
    return matrix;
}

int isEmpty(Matrix* matrix){
    if(matrix->size != 0) {
        return 1;
    }
    return 0;
}

int appendRow(Matrix* matrix, Row* row) {
    if (matrix == NULL) {
        return 1;
    }
    matrix->rows = realloc(matrix->rows, (matrix->size + 1) * sizeof(Row*));
    if (matrix->rows == NULL) {
        return 1;
    }
    matrix->rows[matrix->size] = row;
    matrix->size++;
    return 0;
}

int setItem(Matrix* matrix, int row_pos, int col_pos, int value) {
    if ((row_pos < 0 || row_pos >= matrix->size) & (col_pos < 0 || col_pos >= matrix->rows[row_pos]->size)) {
        return 1;
    }else{
        matrix->rows[row_pos]->data[col_pos] = value;
    }
    return 0;
}


void swapRows(Matrix* matrix, int row1, int row2) {
    if (row1 < 0 || row1 >= matrix->size || row2 < 0 || row2 >= matrix->size)
        return;

    Row *temp = matrix->rows[row1];
    matrix->rows[row1] = matrix->rows[row2];
    matrix->rows[row2] = temp;
}


void freeMatrix(Matrix* matrix) {
    for (int i = 0; i < matrix->size; ++i) {
        free(matrix->rows[i]->data);
        free(matrix->rows[i]);
    }
    free(matrix->rows);
    free(matrix);
}

//int* popRow(Matrix* matrix, int row_pos) {
//    if (row_pos < 0 || row_pos >= matrix->size) {
//        printf("Invalid row position\n");
//        return NULL;
//    }
//    Row* removedRow = &(matrix->rows[row_pos]);
//    for (int i = row_pos; i < matrix->size - 1; ++i) {
//        matrix->rows[i] = matrix->rows[i + 1];
//    }
//    matrix->size--;
//    return removedRow->data;
//}
//
//int popItem(Matrix* matrix, int row_pos, int col_pos, int* item) {
//    if (!getItem(matrix, row_pos, col_pos, item))
//        return 0; // Возвращаем 0, если не удалось получить элемент
//
//    // Удаление элемента из строки
//
//    for (int i = col_pos; i < matrix->rows[row_pos].size - 1; ++i) {
//        matrix->rows[row_pos].data[i] = matrix->rows[row_pos].data[i + 1];
//    }
//    matrix->rows[row_pos].size--;
//
//    return 1; // Возвращаем 1 в случае успешного удаления элемента
//}
