//
//  avltree.h
//  spreadsheet
//
//  Created by pawissakan.ch on 6/9/2559 BE.
//  Copyright © 2559 Throughwave. All rights reserved.
//

#ifndef avltree_h
#define avltree_h

#include <stdio.h>
#include <stdlib.h>
#include "node.h"
#include <string.h>

struct AVLTree {
    struct Node* root;
    int size;
};
struct AVLTree* avltree_init();
void inorder(struct AVLTree* tree);
struct Node* add_node(struct AVLTree* tree, char* val, char* formula, char* x, char* y);
void remove_node(struct AVLTree* tree, int x, int y);

#endif /* avltree_h */
