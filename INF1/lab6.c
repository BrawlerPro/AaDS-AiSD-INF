#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#define MAX_SIZE 100

typedef struct Set{
    char* word;
    char items[MAX_SIZE];
    int count;
} Set;

void initialize(Set *set) {
    set->count = 0;
    memset(set->items, 0, sizeof(set->items));
}

bool contains(Set *set, char value) {
    for (int i = 0; i < set->count; i++) {
        if (set->items[i] == value) {
            return true;
        }
    }
    return false;
}

void add(Set *set, char *value) {
    set->word = value;

    for (int i = 0; i < strlen(value); i++) {
        if (!contains(set, value[i]) && set->count < MAX_SIZE) {
            set->items[set->count++] = value[i];
        }
    }
}

void removeElement(Set *set, char value) {
    int index = -1;
    for (int i = 0; i < set->count; i++) {
        if (set->items[i] == value) {
            index = i;
            break;
        }
    }
    if (index != -1) {
        for (int i = index; i < set->count - 1; i++) {
            set->items[i] = set->items[i + 1];
        }
        set->count--;
    }
}



Set intersection(Set *set1, Set *set2) {
    Set result;
    initialize(&result);

    for (int i = 0; i < set1->count; i++) {
        if (contains(set2, set1->items[i])) {
            add(&result, &set1->items[i]);
        }
    }
    result.word = set2->word;

    return result;
}

Set difference(Set *set1, Set *set2) {
    Set result;
    initialize(&result);

    for (int i = 0; i < set1->count; i++) {
        if (!contains(set2, set1->items[i])) {
            add(&result, &set1->items[i]);
        }
    }
    result.word = set2->word;
    return result;
}

Set unionSet(Set *set1, Set *set2) {
    Set result = *set1;

    for (int i = 0; i < set2->count; i++) {
        add(&result, &set2->items[i]);
    }
    result.word = set2->word;
    return result;
}

Set symmetricDifference(Set *set1, Set *set2) {
    Set diff1 = difference(set1, set2);
    Set diff2 = difference(set2, set1);
    Set result = unionSet(&diff1, &diff2);
    return result;
}

typedef struct Node {
    char data;
    struct Node* next;
} Node;

void append(Node** head_ref, char new_data) {
    Node* new_node = (Node*)malloc(sizeof(Node));
    Node* last = *head_ref;
    new_node->data = new_data;
    new_node->next = NULL;
    if (*head_ref == NULL) {
        *head_ref = new_node;
        return;
    }
    while (last->next != NULL)
        last = last->next;
    last->next = new_node;
}

Node* readInput() {
    Node* head = NULL;
    char c;
    while ((c = (char) getchar()) != EOF && c != '\n') {
        if (c == '\t') {
            c = ' '; // заменяем табуляцию на пробел
        }
        append(&head, c);
    }
    if (c == EOF) {
        return NULL; // возвращаем NULL для обозначения конца файла
    }
    return head;
}


char** node2stm(Node *str, int* size) {
    char** words = (char **)malloc(sizeof(char *) * 32);
    char* word = (char*) calloc(100, sizeof(char));
    Node* node = str;
    int count = 0;
    int count2 = 0;

    while(node != NULL){
        if(node->data == ' ' || node->data == '\t') {
            word[count2] = '\0';
            words[count] = strdup(word);
            count2 = 0;
            node = node->next;
            count++;
        }else{
            word[count2] = node->data;
            count2++;
            node = node->next;
        }
    }
    word[count2] = '\0';
    words[count] = strdup(word);
    free(word);
    free(node);
    count++;
    (*size) = count;
    return words;
}

int compareByFirstWord(const void* a, const void* b){
    return -(((Set *) a)->count - ((Set *) b)->count);
}

void freeList(Node* head) {
    Node* current = head;
    Node* next;
    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }
    head = NULL;
}

void processInput(Node* input) {
    int size = 0;
    char **words = node2stm(input, &size);
    Set *arr_set = (Set*) malloc(size * sizeof(Set));
    Set first;
    initialize(&first);
    add(&first, words[0]);

    arr_set[0] = first;
    for(int i = 1; i < size; i++){
        Set set2;
        initialize(&set2);
        add(&set2, words[i]);
        arr_set[i] = intersection(&first, &set2);
    }

    qsort(arr_set, size, sizeof(Set), compareByFirstWord);
    printf("\"");
    for(int i = 0; i < size; i++){
        printf("%s ", arr_set[i].word);
    }
    printf("\"\n");
    free(arr_set);
    for(int i = 0; i < size; i++){
        free(words[i]);
    }
    free(words);
}

int main() {
    Node* input = readInput();
    processInput(input);
    freeList(input);
//    Set set1, set2;
//    initialize(&set1);
//    initialize(&set2);
//
//
//    add(&set1, "apple");
//    add(&set1, "banana");
//    add(&set1, "orange");
//    add(&set2, "banana");
//    add(&set2, "grape");
//    add(&set2, "apple");
//
//    printf("Set 1: ");
//    printSet(&set1);
//    printf("Set 2: ");
//    printSet(&set2);
//
//    Set inter = intersection(&set1, &set2);
//    printf("Intersection: ");
//    printSet(&inter);
//
//    Set diff = difference(&set1, &set2);
//    printf("Difference of set1 - set2: ");
//    printSet(&diff);
//
//    Set symDiff = symmetricDifference(&set1, &set2);
//    printf("Symmetric Difference: ");
//    printSet(&symDiff);
//
//    Set union_Set = unionSet(&set1, &set2);
//    printf("Union: ");
//    printSet(&union_Set);
//    printf("%d", union_Set.count);
    return 0;
}
