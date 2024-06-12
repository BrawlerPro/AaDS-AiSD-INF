#ifndef SKIBIDITOILET_B_TREE_H
#define SKIBIDITOILET_B_TREE_H

typedef struct Tree Tree;

typedef struct Info {
    const char* key;
    unsigned int value;
} Info;

typedef struct SearchResult {
    Info *info;
    struct SearchResult *next;
} SearchResult;

Tree* createTree();
int getSize(Tree* tree);
void insert(Tree *tree, const char *key, unsigned int value);
void delete(Tree *tree, const char *key);
SearchResult* search(Tree* tree, const char *key);
void printTree(Tree *tree);
void visualizeTree(Tree *node);
void freeSearchRes(SearchResult* result);
void freeTree(Tree *tree);

#endif //SKIBIDITOILET_B_TREE_H

