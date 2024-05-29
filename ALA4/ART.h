#ifndef SKIBIDITOILET_ART_H
#define SKIBIDITOILET_ART_H

typedef struct Tree Tree;

Tree* treeInit();
int getSize(Tree* t);
int insert(Tree* t, const char *key);
char* delete(Tree* t, const char *key);
int search(Tree* t, const char *key);
int findClosestKey(Tree* t, const char *key);
void printTree(Tree* t);
int cutDownTree(Tree* t);
int destroyTree(Tree* t);

#endif //SKIBIDITOILET_ART_H
