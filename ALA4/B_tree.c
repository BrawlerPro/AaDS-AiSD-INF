#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "B_tree.h"

#define MAX_KEYS 3
#define MIN_KEYS 1

typedef struct Node {
    int numKeys;
    Info* keys;
    struct Node **children;
    struct Node *parent;
} Node;

typedef struct Tree {
    Node *root;
    int size;
} Tree;


Tree* createTree() {
    Tree *tree = (Tree *)malloc(sizeof(Tree));
    if(tree != NULL){
        tree->root = NULL;
        tree->size = 0;
    }
    return tree;
}

Node* createNode() {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if(newNode == NULL) return NULL;
    newNode->keys = (Info*)malloc(MAX_KEYS * sizeof(Info));
    if(newNode->keys == NULL){
        free(newNode);
        return NULL;
    }
    newNode->children = (Node**)malloc((MAX_KEYS + 1) * sizeof(Node*));
    if(newNode->children == NULL){
        free(newNode->keys);
        free(newNode);
        return NULL;
    }
    for (int i = 0; i <= MAX_KEYS; i++) {
        newNode->children[i] = NULL;
    }
    newNode->parent = NULL;
    newNode->numKeys = 0;
    return newNode;
}

int getSize(Tree* tree){
    return tree->size;
}

void splitChild(Node *parent, int index, Node *child) {
    Node *newChild = createNode();
    if(newChild == NULL) return;

    newChild->parent = parent;

    newChild->numKeys = MIN_KEYS;
    for (int j = 0; j < MIN_KEYS; j++) {
        newChild->keys[j] = child->keys[j + MIN_KEYS + 1];
    }

    if (child->children[0] != NULL) {
        for (int j = 0; j <= MIN_KEYS; j++) {
            newChild->children[j] = child->children[j + MIN_KEYS + 1];
            if (newChild->children[j] != NULL) {
                newChild->children[j]->parent = newChild;
            }
        }
    }
    child->numKeys = MIN_KEYS;

    for (int j = parent->numKeys; j > index; j--) {
        parent->children[j + 1] = parent->children[j];
    }
    parent->children[index + 1] = newChild;

    for (int j = parent->numKeys - 1; j >= index; j--) {
        parent->keys[j + 1] = parent->keys[j];
    }
    parent->keys[index] = child->keys[MIN_KEYS];
    parent->numKeys++;
}

void insertNonFull(Node *node, const char *key, unsigned int value) {
    int i = node->numKeys - 1;
    if (node->children[0] == NULL) {
        while (i >= 0 && strcmp(key, node->keys[i].key) < 0) {
            node->keys[i + 1] = node->keys[i];
            i--;
        }
        node->keys[i + 1].key = strdup(key);
        node->keys[i + 1].value = value;
        node->numKeys++;
    } else {
        while (i >= 0 && strcmp(key, node->keys[i].key) < 0) {
            i--;
        }
        i++;
        if (node->children[i]->numKeys == MAX_KEYS) {
            splitChild(node, i, node->children[i]);
            if (strcmp(key, node->keys[i].key) > 0) {
                i++;
            }
        }
        insertNonFull(node->children[i], key, value);
    }
}

void mergeNodes(Node *parent, int index) {
    Node *child = parent->children[index];
    Node *sibling = parent->children[index + 1];

    child->keys[MIN_KEYS] = parent->keys[index];

    for (int i = 0; i < sibling->numKeys; i++) {
        child->keys[i + MIN_KEYS + 1] = sibling->keys[i];
    }

    if (child->children[0] != NULL) {
        for (int i = 0; i <= sibling->numKeys; i++) {
            child->children[i + MIN_KEYS + 1] = sibling->children[i];
        }
    }

    for (int i = index + 1; i < parent->numKeys; i++) {
        parent->keys[i - 1] = parent->keys[i];
    }

    for (int i = index + 2; i <= parent->numKeys; i++) {
        parent->children[i - 1] = parent->children[i];
    }

    child->numKeys += sibling->numKeys + 1;
    parent->numKeys--;

    free(sibling->keys);
    free(sibling->children);
    free(sibling);
}

void borrowFromPrev(Node *parent, int index) {
    Node *child = parent->children[index];
    Node *sibling = parent->children[index - 1];

    for (int i = child->numKeys - 1; i >= 0; i--) {
        child->keys[i + 1] = child->keys[i];
    }

    if (child->children[0] != NULL) {
        for (int i = child->numKeys; i >= 0; i--) {
            child->children[i + 1] = child->children[i];
        }
    }

    child->keys[0] = parent->keys[index - 1];
    if (child->children[0] != NULL) {
        child->children[0] = sibling->children[sibling->numKeys];
    }

    parent->keys[index - 1] = sibling->keys[sibling->numKeys - 1];

    child->numKeys++;
    sibling->numKeys--;
}

void borrowFromNext(Node *parent, int index) {
    Node *child = parent->children[index];
    Node *sibling = parent->children[index + 1];

    child->keys[child->numKeys] = parent->keys[index];
    if (child->children[0] != NULL) {
        child->children[child->numKeys + 1] = sibling->children[0];
    }

    parent->keys[index] = sibling->keys[0];

    for (int i = 1; i < sibling->numKeys; i++) {
        sibling->keys[i - 1] = sibling->keys[i];
    }

    if (sibling->children[0] != NULL) {
        for (int i = 1; i <= sibling->numKeys; i++) {
            sibling->children[i - 1] = sibling->children[i];
        }
    }

    child->numKeys++;
    sibling->numKeys--;
}

void deleteKey(Node *root, const char *key) {
    Node *current = root;
    while (current != NULL) {
        int i = 0;
        while (i < current->numKeys && strcmp(key, current->keys[i].key) > 0) {
            i++;
        }

        if (i < current->numKeys && strcmp(key, current->keys[i].key) == 0) {
            if (current->children[0] == NULL) {
                for (int j = i; j < current->numKeys - 1; j++) {
                    current->keys[j] = current->keys[j + 1];
                }
                current->numKeys--;
                return;
            }
            else {
                Node *temp = current->children[i + 1];
                while (temp->children[0] != NULL) {
                    temp = temp->children[0];
                }
                current->keys[i] = temp->keys[0];
                deleteKey(temp, temp->keys[0].key);
                return;
            }
        }
        else {
            if (i < current->numKeys) {
                current = current->children[i];
            }
            else {
                current = current->children[current->numKeys];
            }
        }
    }
}


void insert(Tree *tree, const char *key, unsigned int value) {
    if (tree->root == NULL) {
        tree->root = createNode();
        tree->root->keys[0].key = strdup(key);
        tree->root->keys[0].value = value;
        tree->root->numKeys = 1;
        tree->size++;
    } else {
        if (tree->root->numKeys == MAX_KEYS) {
            Node *newRoot = createNode();
            newRoot->children[0] = tree->root;
            tree->root->parent = newRoot;
            splitChild(newRoot, 0, tree->root);
            insertNonFull(newRoot, key, value);
            tree->root = newRoot;
            tree->size++;
        } else {
            insertNonFull(tree->root, key, value);
            tree->size++;
        }
    }
}

void delete(Tree *tree, const char *key) {
    if (tree->root != NULL) {
        deleteKey(tree->root, key);
        if (tree->root->numKeys == 0) {
            Node *oldRoot = tree->root;
            if (tree->root->children[0] != NULL) {
                tree->root = tree->root->children[0];
            } else {
                tree->root = NULL;
            }
            free(oldRoot->keys);
            free(oldRoot->children);
            free(oldRoot);
        }
        tree->size--;
    }
}

void printT(Node *node, int level) {
    if (node == NULL) return;
    for (int i = 0; i < level; i++) printf("  ");
    for (int i = 0; i < node->numKeys; i++) printf("(%s: %u) ", node->keys[i].key, node->keys[i].value);
    printf("\n");
    for (int i = 0; i <= node->numKeys; i++) {
        printT(node->children[i], level + 1);
    }
}

void printTree(Tree *tree){
    printT(tree->root, 0);
}


void generateDOT(Node *node, FILE *file) {
    if (node != NULL) {
        fprintf(file, "  node%p [label=\"", (void*)node);
        for (int i = 0; i < node->numKeys; i++) {
            fprintf(file, "<f%d> %s: %u | ", i, node->keys[i].key, node->keys[i].value);
        }
        fprintf(file, "<f%d>\"];\n", node->numKeys);

        for (int i = 0; i <= node->numKeys; i++) {
            if (node->children[i] != NULL) {
                fprintf(file, "  \"node%p\":f%d -> \"node%p\";\n", (void*)node, i, (void*)node->children[i]);
                generateDOT(node->children[i], file);
            }
        }
    }
}

void visualizeTree(Tree *tree) {
    FILE *file = fopen("tree.dot", "w");
    if (file == NULL) {
        perror("Could not open file");
        return;
    }

    fprintf(file, "digraph BTree {\n");
    fprintf(file, "  node [shape=record];\n");
    if (tree->root != NULL) {
        generateDOT(tree->root, file);
    }
    fprintf(file, "}\n");

    fclose(file);
    system("dot -Tpng tree.dot -o TreePng.png");

//    remove("tree.dot");
}

SearchResult* createSearchResult(Info *info) {
    SearchResult *result = (SearchResult*)malloc(sizeof(SearchResult));
    result->info = info;
    result->next = NULL;
    return result;
}

SearchResult* appendSearchResult(SearchResult *list, Info *info) {
    SearchResult *newNode = createSearchResult(info);
    if (list == NULL) {
        return newNode;
    }
    SearchResult *current = list;
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = newNode;
    return list;
}

SearchResult* search(Tree* tree, const char *key) {
    SearchResult *result = NULL;
    int size = MAX_KEYS * 100;
    if (tree == NULL) {
        return result;
    }
    Node* root = tree->root;

    Node **stack = (Node**) malloc(sizeof(Node*) * size);
    if(stack == NULL) return NULL;
    int stackIndex = 0;

    stack[stackIndex++] = root;

    while (stackIndex > 0) {
        if(stackIndex > size){
            size *= 2;
            Node **buff = stack;
            if((stack = realloc(stack, sizeof(Node*) * size)) == NULL){
                free(buff);
                printf("Fatal error");
                return NULL;
            }
        }

        Node *node = stack[--stackIndex];

        int i = 0;
        while (i < node->numKeys && strcmp(key, node->keys[i].key) > 0) {
            i++;
        }

        if (i < node->numKeys && strcmp(key, node->keys[i].key) == 0) {
            result = appendSearchResult(result, &node->keys[i]);
            for (int j = i + 1; j < node->numKeys && strcmp(key, node->keys[j].key) == 0; j++) {
                result = appendSearchResult(result, &node->keys[j]);
            }
        }

        for (int j = node->numKeys; j >= 0; j--) {
            if (node->children[j] != NULL) {
                stack[stackIndex++] = node->children[j];
            }
        }
    }

    return result;
}

void freeSearchRes(SearchResult* current){
    SearchResult* prev = current;
    while(current != NULL){
        current = current->next;
        free(prev);
        prev = current;
    }
}

void freeNode(Node *root) {
    int size = MAX_KEYS * 100;
    if (root == NULL) {
        return;
    }

    Node **stack = (Node**) malloc(sizeof(Node*) * size);
    if(stack == NULL) return;
    int stackIndex = 0;

    stack[stackIndex++] = root;

    while (stackIndex > 0) {
        if(stackIndex > size){
            size *= 2;
            Node **buff = stack;
            if((stack = realloc(stack, sizeof(Node*) * size)) == NULL){
                free(buff);
                return;
            }
        }
        Node *node = stack[--stackIndex];
        for (int i = 0; i <= node->numKeys; i++) {
            if (node->children[i] != NULL) {
                stack[stackIndex++] = node->children[i];
            }
        }

        for (int i = 0; i < node->numKeys; i++) {
            free((char*) node->keys[i].key);
        }
        free(node->keys);
        free(node->children);
        free(node);
    }
    free(stack);
}

void freeTree(Tree *tree) {
    if (tree == NULL) {
        return;
    }
    freeNode(tree->root);
    free(tree);
}