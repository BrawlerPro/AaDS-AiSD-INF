#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bitree.h"

typedef struct Node {
    UNode data;
    struct Node *parent;
    struct Node *left;
    struct Node *right;
} Node;

typedef struct Tree {
    Node *root;
    int size;
}Tree;

Tree* initTree(){
    Tree* tree = (Tree*) malloc(sizeof(Tree));
    if (tree != NULL)
        tree->size = 0;
    return tree;
}

int getSize(Tree* tree) {return tree->size;}

Node* createNode(const char *key, unsigned int value) {
    Node *newNode = (Node*) malloc(sizeof(Node));
    if (newNode != NULL) {
        newNode->data.key = strdup(key);
        newNode->data.value = value;
        newNode->left = NULL;
        newNode->right = NULL;
        newNode->parent = NULL;
    }
    return newNode;
}

Node* seaNode(Node* root, const char* key){
    while (root != NULL) {
        if (strcmp(root->data.key, key) == 0) {
            return root;
        }
        if (strcmp(key, root->data.key) < 0)
            root = root->left;
        else
            root = root->right;
    }
    return NULL;
}

Node* fCS(Node* root, const char* key){
    if (root == NULL) return NULL;

    Node *closestNode = NULL;
    Node *current = root;

    while (current != NULL) {
        closestNode = current;
        if (strcmp(key, current->data.key) < 0)
            current = current->left;
        else
            current = current->right;
    }

    if(closestNode == NULL) return NULL;

    return closestNode;
}

int insertNode(Tree *tree, const char *key, unsigned int value) {
    if (tree == NULL) return 1;

    Node* parent = fCS(tree->root, key);

    Node *newNode = createNode(key, value);
    if (newNode == NULL) return 1;

    newNode->parent = parent;
    if (parent == NULL)
        tree->root = newNode;
    else if (strcmp(key, parent->data.key) < 0)
        parent->left = newNode;
    else
        parent->right = newNode;

    tree->size++;
    return 0;
}

Node* findSuccessor(Node *node) {
    if (node == NULL) return NULL;

    node = node->right;
    while (node != NULL && node->left != NULL)
        node = node->left;

    return node;
}

int deleteNode(Tree *tree, const char *key) {
    if (tree == NULL || tree->root == NULL) return 1;

    Node *parent = NULL;
    Node *toDelete = NULL;

    toDelete = seaNode(tree->root, key);
    if (toDelete == NULL) return 1;
    parent = toDelete->parent;

    if (toDelete->left == NULL && toDelete->right == NULL) {
        if (parent == NULL)
            tree->root = NULL;
        else if (parent->left == toDelete)
            parent->left = NULL;
        else
            parent->right = NULL;
        free(toDelete->data.key);
        free(toDelete);
    }
    else if (toDelete->left == NULL || toDelete->right == NULL) {
        Node *child = (toDelete->left != NULL) ? toDelete->left : toDelete->right;
        if (parent == NULL)
            tree->root = child;
        else if (parent->left == toDelete)
            parent->left = child;
        else
            parent->right = child;
        child->parent = parent;
        free(toDelete->data.key);
        free(toDelete);
    }
    else {
        Node *successor = findSuccessor(toDelete);
        if (successor != NULL) {
            const char* k = strdup(successor->data.key);
            unsigned int v = successor->data.value;
            deleteNode(tree, successor->data.key);
            strcpy(toDelete->data.key, k);
            toDelete->data.value = v;
            free(k);
        }

    }
    tree->size--;
    return 0;
}

UNode* searchNode(Tree* tree, const char *key) {
    Node* find = seaNode(tree->root, key);
    if(find == NULL) return NULL;
    return &find->data;

}

UNode* findClosestKey(Tree* tree, const char *key) {
    Node* found = fCS(tree->root, key);
    if (found == NULL) return NULL;

    return &found->data;
}

void preOrderTraversal(Node *root) {
    if (root == NULL) return;
    int size = 100;

    Node **stack = (Node**) malloc(sizeof(Node*) * size);
    int top = -1;


    stack[++top] = root;

    while (top >= 0) {
        if(top > size){
            size *= 2;
            Node **buff = stack;
            if((stack = realloc(stack, sizeof(Node*) * size)) == NULL){
                free(buff);
                printf("Fatal error");
                return;
            }
        }

        Node *current = stack[top--];
        printf("%s: %d\n", current->data.key, current->data.value);

        if (current->right != NULL) stack[++top] = current->right;
        if (current->left != NULL) stack[++top] = current->left;
    }
    free(stack);
}

void preorderTraversal(Tree* tree){
    preOrderTraversal(tree->root);
    printf("Листьев в дереве %d\n", tree->size);
}

void exportGraphviz(Node *root, FILE *file) {
    if (root == NULL) return;
    int size = 100;

    Node **stack = (Node **)malloc(size * sizeof(Node *));
    int top = -1;

    stack[++top] = root;

    while (top >= 0) {
        if(top > size){
            size *= 2;
            Node **buff = stack;
            if((stack = realloc(stack, sizeof(Node*) * size)) == NULL){
                free(buff);
                printf("Fatal error");
                return;
            }
        }
        Node *current = stack[top--];

        fprintf(file, "node%p [label=\"%s: %u\"];\n", (void *)current, current->data.key, current->data.value);

        if (current->right != NULL) {
            stack[++top] = current->right;
            fprintf(file, "node%p -> node%p [label=\"Right\"];\n", (void *)current, (void *)(current->right));
        }
        if (current->left != NULL) {
            stack[++top] = current->left;
            fprintf(file, "node%p -> node%p [label=\"Left\"];\n", (void *)current, (void *)(current->left));
        }
    }

    free(stack);
}

void visualizeTree(Tree* tree) {
    FILE *file = fopen("tree.dot", "w");
    if (file != NULL) {
        fprintf(file, "digraph BinaryTree {\n");
        exportGraphviz(tree->root, file);
        fprintf(file, "}\n");
        fclose(file);

        system("dot -Tpng tree.dot -o tree.png");
        printf("Binary tree visualized successfully. Check tree.png for the visualization.\n");
    } else {
        printf("Error creating Graphviz DOT file.\n");
    }
}

void cutDownTree(Node *root) {
    if (root == NULL) return;
    int size = 100;

    Node **stack = (Node**) malloc(sizeof(Node*) * size);
    int top = -1;

    stack[++top] = root;

    while (top >= 0) {
        if(top > size){
            size *= 2;
            Node **buff = stack;
            if((stack = realloc(stack, sizeof(Node*) * size)) == NULL){
                free(buff);
                printf("Fatal error");
                return;
            }
        }

        Node *current = stack[top--];

        if (current->right != NULL) stack[++top] = current->right;
        if (current->left != NULL) stack[++top] = current->left;

        free(current->data.key);
        free(current);
    }
    free(stack);
}

void deleteTree(Tree *tree) {
    if (tree == NULL) return;

    cutDownTree(tree->root);

    free(tree);
}