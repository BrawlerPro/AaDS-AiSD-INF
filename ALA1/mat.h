#ifndef MAT_H
#define MAT_H

typedef struct Row Row;
typedef struct Matrix Matrix;

Row* ArrToRow(int* arr, int size);
Matrix* readFromFile(const char* filename);
void writeToFile(const char* filename, Matrix* matrix);
void processMatrix(Matrix *matrix);
void printMatrix(Matrix* matrix);

#endif //SKIBIDITOILET_MAT_H
