#ifndef MATRIX_H
#define MATRIX_H

typedef struct Row Row;
typedef struct Matrix Matrix;

Matrix *initMat();
int isEmpty(Matrix* matrix);
int appendRow(Matrix* matrix, Row* row);
int setItem(Matrix* matrix, int row_pos, int col_pos, int value);
void swapRows(Matrix* matrix, int row1, int row2);
void freeMatrix(Matrix* matrix);



#endif /* MATRIX_H */
