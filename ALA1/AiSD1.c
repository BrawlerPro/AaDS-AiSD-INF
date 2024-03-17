#include <stdio.h>
#include <stdlib.h>
#include "matrix.h"
#include "mat.h"

int main() {
    int rows;
    printf("Enter the number of rows: \n");
    scanf("%d", &rows);

    Matrix *matrix = initMat();
    for(int i = 0; i < rows; i++){
        int cols;
        printf("Enter the number of cols: \n");
        scanf("%d", &cols);
        int* data = (int*) malloc (cols*sizeof(int));
        printf("Enter elements row: \n");
        for(int j = 0; j < cols; j++){
            scanf("%d", &data[j]);
        }
        appendRow(matrix, ArrToRow(data, cols));
        free(data);
    }


    printf("Original matrix:\n");
    printMatrix(matrix);

    printf("Processed matrix:\n");
    processMatrix(matrix);
    printMatrix(matrix);

    freeMatrix(matrix);

    return 0;
}
