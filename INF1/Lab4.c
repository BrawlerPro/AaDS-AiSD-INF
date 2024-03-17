#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *readline(const char *prompt) {
    printf("%s", prompt);
    char *res = NULL;
    int len = 1;
    char buf[101] = {};

    int scan_res = 0;
    while ((scan_res = scanf("%100[^\n]s", buf)) != EOF && scan_res != 0) {
        len += 100;
        res = (char *)realloc(res, len * sizeof(char *));
        if (res == NULL) return NULL;
        strncpy(res + len - 101, buf, 100);
    }
    
    if (scan_res == 0 && res == NULL) res = (char *)calloc(1, sizeof(char));
    if(scanf("%*c") == EOF) exit(0);
    return res;
}
 
void myProcessAndPrint(char *input) {
        char *start = strchr(input, '.');
        char *end = strrchr(input, ';');
        if((start != NULL && end != NULL) && (start - input <= end - input)){
                memmove(input, start + 1, end - start);
                input[end - start - 1] = '\0';
        }
        return ;


}

int main() {
    char *input = NULL;
    while (1) {
        input = readline("> ");
        myProcessAndPrint(input);
        printf("Отредактированная строка: \"%s\"\n", input);
        free(input);
    }
    return 0;
}

