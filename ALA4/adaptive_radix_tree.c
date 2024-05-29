#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
//#include "ART.h"

#define NODE4 1
#define NODE16 2
#define NODE48 3
#define NODE256 4
#define LEAF 5

typedef struct node_base {
    int node_type;
    struct node_base* parent;
} node_base;

typedef struct inner_base {
    node_base base;
    int children_number;
    char compressed_keys[8];
    int compressed_keys_number;
} inner_base;

typedef struct leaf {
    node_base base;
    const char* key;
} leaf;

typedef struct node4 {
    inner_base base;
    char partial_keys[4];
    node_base* children[4];
} node4;

typedef struct node16 {
    inner_base base;
    char partial_keys[16];
    node_base* children[16];
} node16;

typedef struct node48 {
    inner_base base;
    char partial_keys[256];
    node_base* children[48];
} node48;

typedef struct node256 {
    inner_base base;
    char partial_keys[256];
    node_base* children[256];
} node256;

typedef struct Tree {
    void* root;
    int size;
} Tree;

typedef struct {
    node_base* node;
    node_base* parent;
} node_search_result;

node4* create_node4() {
    node4* new_node = (node4*)malloc(sizeof(node4));
    new_node->base.base.node_type = NODE4;
    new_node->base.base.parent = NULL;
    new_node->base.children_number = 0;
    memset(new_node->partial_keys, 0, 4);
    memset(new_node->children, 0, 4 * sizeof(node_base*));
    return new_node;
}

node16* create_node16() {
    node16* new_node = (node16*)malloc(sizeof(node16));
    new_node->base.base.node_type = NODE16;
    new_node->base.base.parent = NULL;
    new_node->base.children_number = 0;
    memset(new_node->partial_keys, 0, 16);
    memset(new_node->children, 0, 16 * sizeof(node_base*));
    return new_node;
}

node48* create_node48() {
    node48* new_node = (node48*)malloc(sizeof(node48));
    new_node->base.base.node_type = NODE48;
    new_node->base.base.parent = NULL;
    new_node->base.children_number = 0;
    memset(new_node->partial_keys, 0, 256);
    memset(new_node->children, 0, 48 * sizeof(node_base*));
    return new_node;
}
node256* create_node256() {
    node256* new_node = (node256*)malloc(sizeof(node256));
    new_node->base.base.node_type = NODE16;
    new_node->base.base.parent = NULL;
    new_node->base.children_number = 0;
    memset(new_node->partial_keys, 0, 256);
    memset(new_node->children, 0, 256 * sizeof(node_base*));
    return new_node;
}

node_base* find_child(node_base* node, char key) {
    int index;
    switch(node->node_type) {
        case NODE4:
            for (int i = 0; i < 4; i++) {
                if (((node4*)node)->partial_keys[i] == key) {
                    return ((node4*)node)->children[i];
                }
            }
            break;
        case NODE16:
            for (int i = 0; i < 16; i++) {
                if (((node16*)node)->partial_keys[i] == key) {
                    return ((node16*)node)->children[i];
                }
            }
            break;
        case NODE48:
            index = ((node48*)node)->partial_keys[(unsigned char)key];
            if (index != 0) {
                return ((node48*)node)->children[index - 1];
            }
            break;
        case NODE256:
            return ((node256*)node)->children[(unsigned char)key];
    }
    return NULL;
}

void insert(Tree* tree, const char *key) {
    node_base* current = (node_base*)tree->root;


    if (current == NULL) {
        leaf* new_leaf = (leaf*)malloc(sizeof(leaf));
        new_leaf->base.node_type = LEAF;
        new_leaf->base.parent = NULL;
        new_leaf->key = key;
        tree->root = new_leaf;
        tree->size = 1;
        return;
    }

    while (current->node_type != LEAF) {
        current = find_child(current, *key);
    }

    // Если ключ совпадает с существующим, обновляем или возвращаем
    leaf* current_leaf = (leaf*)current;
    if (strcmp(current_leaf->key, key) == 0) {
        return;
    }

    // Если не совпадает, то создаем новый узел и расширяем
    node4* new_node = create_node4();
    leaf* new_leaf = (leaf*)malloc(sizeof(leaf));
    new_leaf->base.node_type = LEAF;
    new_leaf->base.parent = new_node;
    new_leaf->key = key;

    // Заполняем новый узел значениями
    new_node->base.base.parent = current_leaf->base.parent;
    new_node->partial_keys[0] = *current_leaf->key;
    new_node->children[0] = (node_base*)current_leaf;
    new_node->partial_keys[1] = *key;
    new_node->children[1] = (node_base*)new_leaf;

    // Обновляем родительский узел
    if (current_leaf->base.parent == NULL) {
        tree->root = new_node;
    } else {
        node_base* parent = current_leaf->base.parent;
        for (int i = 0; i < 4; i++) {
            if (((node4*)parent)->children[i] == (node_base*)current_leaf) {
                ((node4*)parent)->children[i] = (node_base*)new_node;
                break;
            }
        }
    }

    current_leaf->base.parent = new_node;
    tree->size++;
}

leaf* search(Tree* tree, const char* key) {
    node_base* current = (node_base*)tree->root;
    if (current == NULL) {
        return NULL;
    }

    while (current->node_type != LEAF) {
        current = find_child(current, *key);
        if (current == NULL) {
            return NULL;
        }
    }

    leaf* current_leaf = (leaf*)current;
    if (strcmp(current_leaf->key, key) == 0) {
        return current_leaf;
    } else {
        return NULL;
    }
}

node_search_result find_node_and_parent(Tree* tree, const char *key) {
    node_base* current = (node_base*)tree->root;
    node_base* parent = NULL;

    while (current != NULL && current->node_type != LEAF) {
        parent = current;
        current = find_child(current, *key);
        if (current == NULL) {
            break;
        }
    }

    node_search_result result = {current, parent};
    return result;
}

int find_child_index(node_base* node, node_base* child) {
    int i;
    switch (node->node_type) {
        case NODE4:
            for (i = 0; i < 4; i++) {
                if (((node4*)node)->children[i] == child) {
                    return i;
                }
            }
            break;
        case NODE16:
            for (i = 0; i < 16; i++) {
                if (((node16*)node)->children[i] == child) {
                    return i;
                }
            }
            break;
        case NODE48:
            for (i = 0; i < 48; i++) {
                if (((node48*)node)->children[i] == child) {
                    return i;
                }
            }
            break;
        case NODE256:
            for (i = 0; i < 256; i++) {
                if (((node256*)node)->children[i] == child) {
                    return i;
                }
            }
            break;
    }
    return -1;
}

// Полная функция удаления
void delete(Tree* tree, const char* key) {
    node_search_result result = find_node_and_parent(tree, key);
    node_base* target = result.node;
    node_base* parent = result.parent;

    if (target == NULL || target->node_type != LEAF) {
        return; // Ключ не найден или целевой узел не является листом
    }

    leaf* target_leaf = (leaf*)target;

    // Удаляем лист
    if (parent == NULL) {
        free((void*)target_leaf->key);
        free(target_leaf);
        tree->root = NULL;
        tree->size = 0;
        return;
    }

    int child_index = find_child_index(parent, (node_base*)target_leaf);
    if (child_index == -1) {
        return; // Родительский узел не содержит ссылку на целевой лист
    }

    switch (parent->node_type) {
        case NODE4: {
            node4* parent_node = (node4*)parent;
            for (int i = child_index; i < 3; i++) {
                parent_node->partial_keys[i] = parent_node->partial_keys[i + 1];
                parent_node->children[i] = parent_node->children[i + 1];
            }
            parent_node->partial_keys[3] = 0;
            parent_node->children[3] = NULL;
            parent_node->base.children_number--;
            if (parent_node->base.children_number == 1) {
                // Сжатие узла
                node_base* single_child = parent_node->children[0];
                node_base* grandparent = parent_node->base.base.parent;
                if (grandparent == NULL) {
                    tree->root = single_child;
                    single_child->parent = NULL;
                } else {
                    int grandparent_index = find_child_index(grandparent, (node_base*)parent_node);
                    if (grandparent_index != -1) {
                        switch (grandparent->node_type) {
                            case NODE4:
                                ((node4*)grandparent)->children[grandparent_index] = single_child;
                                break;
                            case NODE16:
                                ((node16*)grandparent)->children[grandparent_index] = single_child;
                                break;
                            case NODE48:
                                ((node48*)grandparent)->children[grandparent_index] = single_child;
                                break;
                            case NODE256:
                                ((node256*)grandparent)->children[grandparent_index] = single_child;
                                break;
                        }
                        single_child->parent = grandparent;
                    }
                }
                free(parent_node);
            }
            break;
        }
        case NODE16: {
            node16* parent_node = (node16*)parent;
            for (int i = child_index; i < 15; i++) {
                parent_node->partial_keys[i] = parent_node->partial_keys[i + 1];
                parent_node->children[i] = parent_node->children[i + 1];
            }
            parent_node->partial_keys[15] = 0;
            parent_node->children[15] = NULL;
            parent_node->base.children_number--;
            if (parent_node->base.children_number == 3) {
                // Понижение узла до NODE4
                node4* new_node = create_node4();
                new_node->base.base.parent = parent_node->base.base.parent;
                for (int i = 0; i < 4; i++) {
                    new_node->partial_keys[i] = parent_node->partial_keys[i];
                    new_node->children[i] = parent_node->children[i];
                }
                new_node->base.children_number = 4;

                node_base* grandparent = parent_node->base.base.parent;
                if (grandparent == NULL) {
                    tree->root = new_node;
                } else {
                    int grandparent_index = find_child_index(grandparent, (node_base*)parent_node);
                    if (grandparent_index != -1) {
                        switch (grandparent->node_type) {
                            case NODE4:
                                ((node4*)grandparent)->children[grandparent_index] = (node_base*)new_node;
                                break;
                            case NODE16:
                                ((node16*)grandparent)->children[grandparent_index] = (node_base*)new_node;
                                break;
                            case NODE48:
                                ((node48*)grandparent)->children[grandparent_index] = (node_base*)new_node;
                                break;
                            case NODE256:
                                ((node256*)grandparent)->children[grandparent_index] = (node_base*)new_node;
                                break;
                        }
                        new_node->base.base.parent = grandparent;
                    }
                }
                free(parent_node);
            }
            break;
        }
        case NODE48: {
            node48* parent_node = (node48*)parent;
            parent_node->partial_keys[(unsigned char)*key] = 0;
            for (int i = child_index; i < 47; i++) {
                parent_node->children[i] = parent_node->children[i + 1];
            }
            parent_node->children[47] = NULL;
            parent_node->base.children_number--;
            if (parent_node->base.children_number == 12) {
                // Понижение узла до NODE16
                node16* new_node = create_node16();
                new_node->base.base.parent = parent_node->base.base.parent;
                int pos = 0;
                for (int i = 0; i < 256; i++) {
                    if (parent_node->partial_keys[i] != 0) {
                        new_node->partial_keys[pos] = i;
                        new_node->children[pos] = parent_node->children[pos];
                        pos++;
                    }
                }
                new_node->base.children_number = 16;

                node_base* grandparent = parent_node->base.base.parent;
                if (grandparent == NULL) {
                    tree->root = new_node;
                } else {
                    int grandparent_index = find_child_index(grandparent, (node_base*)parent_node);
                    if (grandparent_index != -1) {
                        switch (grandparent->node_type) {
                            case NODE4:
                                ((node4*)grandparent)->children[grandparent_index] = (node_base*)new_node;
                                break;
                            case NODE16:
                                ((node16*)grandparent)->children[grandparent_index] = (node_base*)new_node;
                                break;
                            case NODE48:
                                ((node48*)grandparent)->children[grandparent_index] = (node_base*)new_node;
                                break;
                            case NODE256:
                                ((node256*)grandparent)->children[grandparent_index] = (node_base*)new_node;
                                break;
                        }
                        new_node->base.base.parent = grandparent;
                    }
                }
                free(parent_node);
            }
            break;
        }
        case NODE256: {
            node256* parent_node = (node256*)parent;
            parent_node->children[(unsigned char)*key] = NULL;
            parent_node->base.children_number--;
            if (parent_node->base.children_number == 37) {
                // Понижение узла до NODE48
                node48* new_node = create_node48();
                new_node->base.base.parent = parent_node->base.base.parent;
                int pos = 0;
                for (int i = 0; i < 256; i++) {
                    if (parent_node->children[i] != NULL) {
                        new_node->partial_keys[i] = pos + 1;
                        new_node->children[pos] = parent_node->children[i];
                        pos++;
                    }
                }
                new_node->base.children_number = 48;

                node_base* grandparent = parent_node->base.base.parent;
                if (grandparent == NULL) {
                    tree->root = new_node;
                } else {
                    int grandparent_index = find_child_index(grandparent, (node_base*)parent_node);
                    if (grandparent_index != -1) {
                        switch (grandparent->node_type) {
                            case NODE4:
                                ((node4*)grandparent)->children[grandparent_index] = (node_base*)new_node;
                                break;
                            case NODE16:
                                ((node16*)grandparent)->children[grandparent_index] = (node_base*)new_node;
                                break;
                            case NODE48:
                                ((node48*)grandparent)->children[grandparent_index] = (node_base*)new_node;
                                break;
                            case NODE256:
                                ((node256*)grandparent)->children[grandparent_index] = (node_base*)new_node;
                                break;
                        }
                        new_node->base.base.parent = grandparent;
                    }
                }
                free(parent_node);
            }
            break;
        }
    }

    // Уменьшаем размер дерева и освобождаем память
    tree->size--;
    free((void*)target_leaf->key);
    free(target_leaf);
}