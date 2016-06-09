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
#include "debug.h"

struct AVLTree {
    struct Node* root;
    int size;
};
struct AVLTree* avltree_init();
void print_tree(struct AVLTree* tree);
struct Node* get_node(struct AVLTree* tree, char* x, char* y);
struct Node* get_node_i(struct AVLTree* tree,int i, int j);
void inorder(struct AVLTree* tree, struct Node* node);
void update_formula_node(struct AVLTree* tree, struct Node* node);
struct Node* add_node(struct AVLTree* tree, char* val, char* formula, char* x, char* y);
void remove_node(struct AVLTree* tree, char* x, char* y);
char* calculate(struct AVLTree* tree, struct Node* node);

#endif /* avltree_h */
