#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

int validateInput() {
    char input[100];
    int validNumber = 0;

    while (!validNumber) {
        if (fgets(input, sizeof(input), stdin) == NULL) {
            exit(0);
        }
        
        // Удаление символа новой строки из введенной строки
        input[strcspn(input, "\n")] = '\0';

        // Проверка, является ли введенная строка числом
        int i = 0;
        int isNumber = 1;
        int isNegative = 0;

        if(input[i] == '-') {
            isNegative = 1;
            i++;
        }
        while (input[i] != '\0') {
            if (!isdigit(input[i])) {
                isNumber = 0;
                break;
            }
            i++;
        }
        if (isNumber || (isNegative && i > 1)) {
            validNumber = 1;
        } else {
            printf("Error: An incorrect number was entered. Try again.\n");
        }
    }

    // Введено корректное число
    return atoi(input);
}

void initArray(int** array, int size, int* psize) {
    if(*array == NULL){
        *array = (int*) calloc(size, sizeof(int));
        for (int i = 0; i < size; i++) {
            (*array)[i] = validateInput();
        }
        *psize = size;
    }else{
        *array = (int*) realloc(*array, (size) * sizeof(int));
        *psize = size;
    }
}

void insertElement(int** array, int* size, int index, int element) {
    if (index < 0){
        if ((index + *size) <= 0){
            index = 0;
        }else{
            index = abs(index + *size + 1);
        }
        
    }
    if (index >= *size) {
        (*size)++;
        *array = (int*) realloc(*array, (*size) * sizeof(int));
        (*array)[*size - 1] = element;
    } else {
        (*size)++;
        *array = (int*) realloc(*array, (*size) * sizeof(int));
        
        for (int i = *size - 1; i > index; i--) {
            (*array)[i] = (*array)[i - 1];
        }
         
        (*array)[index] = element;
    }
}

void deleteElement(int** array, int* size, int index) {
    if (index < 0){
        index = abs(index + *size);
    }
    if (index >= *size) {
        printf("Error: Index out of range.\n");
        return;
    }
    for (int i = index; i < *size - 1; i++) {
        (*array)[i] = (*array)[i + 1];
    }
    (*size)--;
    *array = (int*) realloc(*array, (*size) * sizeof(int));
}

int isPrime(int number) {
    if (number < 2) {
        return 0;
    }
    for (int i = 2; i*i <= number; i++) {
        if (number % i == 0) {
            return 0;
        }
    }
    return 1;
}

void processArray(int** array, int* size) {
    int* tempArray = (int*) malloc(*size * sizeof(int));
    int tempSize = 0;
    
    for (int i = 0; i < *size; i++) {
        if (isPrime((*array)[i])) {
            tempArray[tempSize++] = (*array)[i];
        }
    }
    
    free(*array);
    *array = tempArray;
    *size = tempSize;
}


void printArray(const int* array, int size) {
    printf("Your Array = [");
    for (int i = 0; i < size; i++) {
        if (i == size - 1){
            printf("%d", array[i]);
        }else{
            printf("%d, ", array[i]);
        }
        
    }
    printf("]\n");
}


int main() {
    int* array = NULL;
    int size = 0;
    int choice = 6;
    
    while (choice != 0) {
        printf("Menu:\n");
        printf("1. Initialize array\n");
        printf("2. Insert element\n");
        printf("3. Delete element\n");
        printf("4. Process array\n");
        printf("5. Print array\n");
        printf("0. Exit\n");
        printf("Enter your choice: ");
        choice = validateInput();
        
        if (choice == 1){
                int newSize;
                printf("Enter the size of the array: ");
                newSize = validateInput();
                if(newSize <= 0){
                        printf("Error: The size of the array mast be grater than 0");
                }else{
                        initArray(&array, newSize, &size);
                        printArray(array, size);
                }
        }else if (choice == 2){
                int index, element;
                printf("Enter the index: ");
                index = validateInput();
                printf("Enter the element: ");
                element = validateInput();
                insertElement(&array, &size, index, element);
                printArray(array, size);
        }else if (choice == 3){
                int index;
                printf("Enter the index of the element to delete: ");
                index = validateInput();
                deleteElement(&array, &size, index);
                printArray(array, size);
        }else if (choice == 4){
                processArray(&array, &size);
                printArray(array, size);
        }else if (choice == 5){
                printf("Array: ");
                printArray(array, size);
        }
        printf("\n");
    };
    
    free(array);
    
    return 0;
}


