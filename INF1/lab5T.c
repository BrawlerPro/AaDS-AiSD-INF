#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>


typedef struct Voter {
    char *fcs;
    char nps[8];
    unsigned char age;
} Voter;


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

        if (input[i] == '-') {
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

char *readline(const char *prompt) {
    printf("%s", prompt);
    char *res = NULL;
    int len = 1;
    char buf[101];

    int scan_res = 0;
    while ((scan_res = scanf("%100[^\n]s", buf)) != EOF && scan_res != 0) {
        len += 100;
        res = (char *) realloc(res, len * sizeof(char *));
        if (res == NULL) return NULL;
        strncpy(res + len - 101, buf, 100);
    }

    if (scan_res == 0 && res == NULL) res = (char *) calloc(1, sizeof(char));
    if (scanf("%*c") == EOF) exit(0);
    return res;
}

Voter *readVoterArrayFromFile(const char *filename, int *numVoters) {
    FILE *file = fopen(filename, "rb");  // Открыть файл для чтения в бинарном режиме

    if (file) {
        // Определение начальной емкости массива.
        int capacity = 10;
        Voter *voters = (Voter *) malloc(capacity * sizeof(Voter));
        int count = 0;

        while (1) {
            if (count >= capacity) {
                // Увеличение емкости массива при необходимости
                capacity *= 2;
                voters = (Voter *) realloc(voters, capacity * sizeof(Voter));
            }

            size_t fcsLen;
            if (fread(&fcsLen, sizeof(size_t), 1, file) != 1) {
                break;  // Достигнут конец файла
            }

            voters[count].fcs = (char *) malloc(fcsLen + 1);  // Выделяем память для ФИО и символа конца строки
            fread(voters[count].fcs, sizeof(char), fcsLen, file);
            voters[count].fcs[fcsLen] = '\0';  // Добавляем символ конца строки
            fread(voters[count].nps, sizeof(char), 8, file);
            fread(&voters[count].age, sizeof(unsigned char), 1, file);

            count++;  // Увеличиваем счетчик структур
        }

        fclose(file);  // Закрываем файл

        *numVoters = count;  // Сохраняем количество структур в переменной, переданной по ссылке

        return voters;  // Возвращаем массив структур
    } else {
        printf("Не удалось открыть файл для чтения");
        return NULL;
    }
}

void writeVoterArrayToFile(const char *filename, Voter *voters, int numVoters) {
    FILE *file = fopen(filename, "wb");  // Открыть файл для записи в бинарном режиме

    if (file) {
        for (int i = 0; i < numVoters; i++) {
            size_t fcsLen = strlen(voters[i].fcs);
            fwrite(&fcsLen, sizeof(size_t), 1, file);  // Записать длину ФИО в бинарном формате
            fwrite(voters[i].fcs, sizeof(char), fcsLen, file);  // Записать ФИО в бинарном формате
            fwrite(voters[i].nps, sizeof(char), 8, file);  // Записать нпс в бинарном формате
            fwrite(&voters[i].age, sizeof(unsigned char), 1, file);  // Записать возраст в бинарном формате
        }

        fclose(file);  // Закрыть файл
    } else {
        printf("Не удалось открыть файл для записи");
    }
}

// Функция, которая читает файл и возвращает массив структур Voter
Voter *read_voters_file(const char *filename, int *num_voters, int num_lines_to_read) {
    // Открываем файл для чтения
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        // Не удалось открыть файл
        return NULL;
    }

    // Создаем массив структур Voter
    Voter *voters = NULL;
    // Считаем количество голосующих в файле
    int count = 0;
    // Буфер для хранения текущей строки
    char *buffer = NULL;
    // Размер буфера
    size_t size = 0;
    // Читаем файл построчно
    while ((num_lines_to_read <= 0 || count < num_lines_to_read) && getline(&buffer, &size, file) != -1) {
        // Увеличиваем количество голосующих
        count++;
        // Выделяем память для массива структур Voter
        voters = realloc(voters, count * sizeof(Voter));
        if (voters == NULL) {
            // Не удалось выделить память
            fclose(file);
            free(buffer);
            return NULL;
        }
        // Разбиваем текущую строку на три части по символу ';'
        char *fcs = strtok(buffer, ";"); // Фамилия, имя, отчество
        char *nps = strtok(NULL, ";"); // Номер
        char *age = strtok(NULL, "\n"); // Возраст
        // Копируем информацию в текущую структуру Voter
        voters[count - 1].fcs = malloc(strlen(fcs) + 1); // Выделяем память для фамилии, имени, отчества
        if (voters[count - 1].fcs == NULL) {
            // Не удалось выделить память
            fclose(file);
            free(buffer);
            return NULL;
        }
        strcpy(voters[count - 1].fcs, fcs); // Копируем фамилию, имя, отчество
        strcpy(voters[count - 1].nps, nps); // Копируем номер
        voters[count - 1].age = (unsigned char) atoi(age); // Преобразуем возраст в целое число и копируем его
    }
    // Освобождаем буфер
    free(buffer);
    *num_voters = count;
    // Закрываем файл
    fclose(file);
    // Возвращаем массив структур Voter
    return voters;
}

void write_voters_to_file(Voter *voters, int num_voters, const char *file_name, const char *mode) {
    FILE *file = fopen(file_name, mode);
    if (file == NULL) {
        fprintf(stderr, "Ошибка при открытии файла для записи\n");
        return;
    }
    for (int i = 0; i < num_voters; i++) {
        fprintf(file, "%s;%s;%d\n", voters[i].fcs, voters[i].nps, voters[i].age);
    }
    fclose(file);
}

Voter parseVoterData(const char *data) {
    Voter voter;
    voter.fcs = NULL;

    // Выделение памяти под fcs и копирование данных
    const char *delimiter = ";";
    char *dataCopy = strdup(data);
    char *token = strtok(dataCopy, delimiter);

    if (token != NULL) {
        voter.fcs = strdup(token);
        token = strtok(NULL, delimiter);
        if (token != NULL) {
            strncpy(voter.nps, token, 7);
            token = strtok(NULL, delimiter);
            if (token != NULL) {
                voter.age = (unsigned char) atoi(token);
            }
        }
    }
    free(token);
    free(dataCopy);
    return voter;
}

void reverse_array(void *array, size_t length, size_t size) {
    char *buffer = (char *) malloc(size);
    if (buffer == NULL) {
        fprintf(stderr, "Ошибка выделения памяти\n");
        return;
    }

    for (size_t i = 0; i < length / 2; i++) {
        char *left = (char *) array + i * size;
        char *right = (char *) array + (length - 1 - i) * size;
        memcpy(buffer, left, size);
        memcpy(left, right, size);
        memcpy(right, buffer, size);
    }

    free(buffer);
}

// Универсальная функция обмена значений
void swap(void *a, void *b, size_t size) {
    unsigned char *buffer = (unsigned char *) malloc(size);
    if (buffer) {
        memcpy(buffer, a, size);
        memcpy(a, b, size);
        memcpy(b, buffer, size);
        free(buffer);
    }
}

// Прототип универсальной функции сравнения
typedef int (*CompareFunc)(const void *, const void *);

// Функция для выполнения чётно-нечётной сортировки с универсальной функцией сравнения
void odd_even_sort(void *arr, size_t elem_count, size_t elem_size, CompareFunc compare_func) {
    unsigned char *array = (unsigned char *) arr;
    int sorted = 0;
    while (!sorted) {
        sorted = 1;
        for (size_t i = 0; i < elem_count - 1; i += 2) {
            if (compare_func(array + i * elem_size, array + (i + 1) * elem_size) > 0) {
                // Обмен значений в массиве
                swap(array + i * elem_size, array + (i + 1) * elem_size, elem_size);
                sorted = 0;
            }
        }
        for (size_t i = 1; i < elem_count - 1; i += 2) {
            if (compare_func(array + i * elem_size, array + (i + 1) * elem_size) > 0) {
                // Обмен значений в массиве
                swap(array + i * elem_size, array + (i + 1) * elem_size, elem_size);
                sorted = 0;
            }
        }

    }
}

// Функция для выполнения двухсторонней сортировки выбором с универсальной функцией сравнения
void double_selection_sort(void *arr, size_t elem_count, size_t elem_size, CompareFunc compare_func) {
    unsigned char *array = (unsigned char *) arr;
    int left = 0;
    int right = elem_count - 1;
    while (left < right) {
        int min_idx = left;
        int max_idx = right;
        for (int i = left; i <= right; i++) {
            if (compare_func(array + i * elem_size, array + min_idx * elem_size) < 0) {
                min_idx = i;
            }
            if (compare_func(array + i * elem_size, array + max_idx * elem_size) > 0) {
                max_idx = i;
            }
        }
        if (min_idx != left) {
            // Обмен значений в массиве
            swap(array + left * elem_size, array + min_idx * elem_size, elem_size);
        }
        if (max_idx == left) {
            max_idx = min_idx;
        }
        if (max_idx != right) {
            // Обмен значений в массиве
            swap(array + right * elem_size, array + max_idx * elem_size, elem_size);
        }
        left++;
        right--;
    }
}

// Пример универсальной функции сравнения для целых чисел
int compare_int(const void *a, const void *b) {
    return *(int *) a - *(int *) b;
}

// Пример универсальной функции сравнения для строк
int compare_str(const void *a, const void *b) {
    return strcmp(*(char **) a, *(char **) b);
}

// Пример универсальной функции для сравнения ФИО
int compareByFcs(const void *a, const void *b) {
    return strcmp(((Voter *) a)->fcs, ((Voter *) b)->fcs);
}

// Пример универсальной функции для сравнения по номеру избирательного участка
int compareByNps(const void *a, const void *b) {
    return strcmp(((Voter *) a)->nps, ((Voter *) b)->nps);
}

// Пример универсальной функции для сравнения по возрасту
int compareByAge(const void *a, const void *b) {
    return ((Voter *) a)->age - ((Voter *) b)->age;
}

void* returnNElements(void *arr, int n, int elementSize, int length) {
    if (n >= length) {
        return arr;
    }
    void *result = malloc(n * elementSize);
    if (result == NULL) {
        fprintf(stderr, "Memory allocation failed");
        exit(1);
    }
    memcpy(result, arr, n * elementSize);
    return result;
}

int main(int argc, char *argv[]) {
    char *file_name = NULL;
    int co_lines = 0;
    int choice = 0;
    Voter *voterArray = NULL;
    
    if(argc != 2){
        fprintf(stderr, "Вы не ввели имя файла\n");
        return 1;
    }else{
        voterArray = read_voters_file(argv[1], &co_lines);
        if(voterArray == NULL){
            fprintf(stderr, "Файл не тот\n");
        }
    }
    
    while (choice != 4) {
        printf("Выберите алгоритм сортировки:\n");
        printf("1. Odd-even sort\n");
        printf("2. Double selection sort\n");
        printf("3. Quick sort\n");
        printf("4. Завершить программу\n");
        printf("Введите номер: ");
        choice = validateInput();

        if (choice == 1) {
            clock_t start, end;
            printf("Выберите количество генерируемых массивов\n");
            int nar = validateInput();
            while(nar == 0){
                printf("Кол-во должно быть >0");
                nar = validateInput();
            }
            int obj[nar];
            printf("Количество элементов в генерируемых массивах\n");
            for(int i; i < nar; i++){
                obj[i] = validateInput();
            }
            printf("Выберите поле структуры, по которому будет осуществляется сортировка:\n");
            printf("1. ФИО\n");
            printf("2. Номер избирательного участка\n");
            printf("3. Возраст\n");
            choi = validateInput();
            if(choi == 1){
                for(int i; i < nar; i++){
                    start = clock();
                    odd_even_sort(returnNElements(voterArray, obj[i], sizeof(Voter), co_lines), obj[i], sizeof(Voter), compareByFcs);
                    end = clock();
                    printf("Кол-во элементов, Время выполнения: %d, %f", obj[i], ((end - start)/CLOCKS_PER_SEC));
                }
            }else if(choi == 2){
                for(int i; i < nar; i++){
                    start = clock();
                    odd_even_sort(returnNElements(voterArray, obj[i], sizeof(Voter), co_lines), obj[i], sizeof(Voter), compareByNps);
                    end = clock();
                    printf("Кол-во элементов, Время выполнения: %d, %f", obj[i], ((end - start)/CLOCKS_PER_SEC));
                }
            }else if(choi == 3){
                for(int i; i < nar; i++){
                    start = clock();
                    odd_even_sort(returnNElements(voterArray, obj[i], sizeof(Voter), co_lines), obj[i], sizeof(Voter), compareByAge);
                    end = clock();
                    printf("Кол-во элементов, Время выполнения: %d, %f", obj[i], ((end - start)/CLOCKS_PER_SEC));
                }
            }else{printf("СОРЕ НЕПОН ПЛС ТРАЙ АГАИН\n");}
        } else if (choice == 2) {
            clock_t start, end;
            printf("Выберите количество генерируемых массивов\n");
            int nar = validateInput();
            while(nar == 0){
                printf("Кол-во должно быть >0");
                nar = validateInput();
            }
            int obj[nar];
            printf("Количество элементов в генерируемых массивах\n");
            for(int i; i < nar; i++){
                obj[i] = validateInput();
            }
            printf("Выберите поле структуры, по которому будет осуществляется сортировка:\n");
            printf("1. ФИО\n");
            printf("2. Номер избирательного участка\n");
            printf("3. Возраст\n");
            choi = validateInput();
            if(choi == 1){
                for(int i; i < nar; i++){
                    start = clock();
                    double_selection_sort(returnNElements(voterArray, obj[i], sizeof(Voter), co_lines), obj[i], sizeof(Voter), compareByFcs);
                    end = clock();
                    printf("Кол-во элементов, Время выполнения: %d, %f", obj[i], ((end - start)/CLOCKS_PER_SEC));
                }
            }else if(choi == 2){
                for(int i; i < nar; i++){
                    start = clock();
                    double_selection_sort(returnNElements(voterArray, obj[i], sizeof(Voter), co_lines), obj[i], sizeof(Voter), compareByNps);
                    end = clock();
                    printf("Кол-во элементов, Время выполнения: %d, %f", obj[i], ((end - start)/CLOCKS_PER_SEC));
                }
            }else if(choi == 3){
                for(int i; i < nar; i++){
                    start = clock();
                    double_selection_sort(returnNElements(voterArray, obj[i], sizeof(Voter), co_lines), obj[i], sizeof(Voter), compareByAge);
                    end = clock();
                    printf("Кол-во элементов, Время выполнения: %d, %f", obj[i], ((end - start)/CLOCKS_PER_SEC));
                }
            }else{printf("СОРЕ НЕПОН ПЛС ТРАЙ АГАИН\n");}
        } else if (choice == 3) {
            clock_t start, end;
            printf("Выберите количество генерируемых массивов\n");
            int nar = validateInput();
            while(nar == 0){
                printf("Кол-во должно быть >0");
                nar = validateInput();
            }
            int obj[nar];
            printf("Количество элементов в генерируемых массивах\n");
            for(int i; i < nar; i++){
                obj[i] = validateInput();
            }
            printf("Выберите поле структуры, по которому будет осуществляется сортировка:\n");
            printf("1. ФИО\n");
            printf("2. Номер избирательного участка\n");
            printf("3. Возраст\n");
            choi = validateInput();
            if(choi == 1){
                for(int i; i < nar; i++){
                    start = clock();
                    qsort(returnNElements(voterArray, obj[i], sizeof(Voter), co_lines), obj[i], sizeof(Voter), compareByFcs);
                    end = clock();
                    printf("Кол-во элементов, Время выполнения: %d, %f", obj[i], ((end - start)/CLOCKS_PER_SEC));
                }
            }else if(choi == 2){
                for(int i; i < nar; i++){
                    start = clock();
                    qsort(returnNElements(voterArray, obj[i], sizeof(Voter), co_lines), obj[i], sizeof(Voter), compareByNps);
                    end = clock();
                    printf("Кол-во элементов, Время выполнения: %d, %f", obj[i], ((end - start)/CLOCKS_PER_SEC));
                }
            }else if(choi == 3){
                for(int i; i < nar; i++){
                    start = clock();
                    qsort(returnNElements(voterArray, obj[i], sizeof(Voter), co_lines), obj[i], sizeof(Voter), compareByAge);
                    end = clock();
                    printf("Кол-во элементов, Время выполнения: %d, %f", obj[i], ((end - start)/CLOCKS_PER_SEC));
                }
            }else{printf("СОРЕ НЕПОН ПЛС ТРАЙ АГАИН\n");}
        }
        printf("\n");
    };
    for (int i = 0; i < co_lines; i++) {
        free(voterArray[i].fcs);
    }
    free(voterArray);

    // Voter *voterArray = (Voter*)calloc(size, sizeof(Voter));
    // for (int ix = 0; ix < 3; ix++)
    // {
    //     arr[ix] = parseVoterData(readline("> "));
    // }
    // for (int ix = 0; ix < 3; ix++)
    // {
    //     printf("%s", arr[ix].fcs);
    // }
    return 0;
}