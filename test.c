#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_SIZE 100


typedef struct {
    char data;
} StackItem;


typedef struct {
    StackItem *items;
    int top;
    int capacity;
} Stack;


Stack* createStack(int capacity) {
    Stack *stack = (Stack*)malloc(sizeof(Stack));
    stack->items = (StackItem*)malloc(capacity * sizeof(StackItem));
    stack->top = -1;
    stack->capacity = capacity;
    return stack;
}


int isEmpty(Stack *stack) {
    return stack->top == -1;
}


void append(Stack *stack, int value) {
    if (stack->top == stack->capacity - 1) {
        printf("Стек переполнен\n");
        return;
    }
    StackItem newItem;
    newItem.data = value;
    stack->items[++stack->top] = newItem;
}


int pop(Stack *stack) {
    if (isEmpty(stack)) {
        printf("Стек пуст\n");
        return -1;
    }
    return stack->items[stack->top--].data;
}


char getLastEl(Stack *stack) {
    if (isEmpty(stack)) {
        printf("Стек пуст\n");
        return -1;
    }
    return stack->items[stack->top].data;
}


void freeStack(Stack *stack) {
    free(stack->items);
    free(stack);
}


char *readline(const char *prompt) {
    printf("%s", prompt);
    char *res = NULL;
    int len = 1;
    char buf[101];

    int scan_res = 0;
    while ((scan_res = scanf("%100[^\n]s", buf)) != EOF && scan_res != 0) {
        len += 100;
        res = (char *) realloc(res, len * sizeof(char*));
        if (res == NULL) return NULL;
        strncpy(res + len - 101, buf, 100);
    }

    if (scan_res == 0 && res == NULL) res = (char *) calloc(1, sizeof(char));
    if (scanf("%*c") == EOF) exit(0);
    return res;
}

int main(){
    char* line = readline(">_");
    if(line[0] == ']' || line[0] == '>' || line[0] == '}' || line[0] == ')'){
        printf("0");
        free(line);
    }else{
        Stack* stack = createStack(100);
        for(int i = 0; i < strlen(line); i++){
            if(line[i] == '[' || line[i] == '<' || line[i] == '{' || line[i] == '('){
                append(stack, line[i]);
            } else{
                if(line[i] == ')' && getLastEl(stack) == '('){
                    pop(stack);
                } else if(line[i] == ']' && getLastEl(stack) == '['){
                    pop(stack);
                } else if(line[i] == '}' && getLastEl(stack) == '{'){
                    pop(stack);
                } else if(line[i] == '>' && getLastEl(stack) == '<'){
                    pop(stack);
                }
            }
        }
        if(isEmpty(stack)){
            printf("1");
            freeStack(stack);
            free(line);
        } else{
            printf("0");
            freeStack(stack);
            free(line);
        }
    }
}