#ifndef SKIBIDITOILET_BITREE_H
#define SKIBIDITOILET_BITREE_H

// Бинароне дерево

typedef struct UNode {
    const char *key;
    unsigned int value;
} UNode;

typedef struct Tree Tree;

Tree* initTree();
int getSize(Tree* tree);
int insertNode(Tree* tree, const char *key, unsigned int value);
int deleteNode(Tree* tree, const char *key);
UNode* searchNode(Tree* tree, const char *key);
UNode* findClosestKey(Tree* tree, const char *key);
void preorderTraversal(Tree *root);
void visualizeTree(Tree *tree);
void deleteTree(Tree *tree);

#endif //SKIBIDITOILET_BITREE_H
