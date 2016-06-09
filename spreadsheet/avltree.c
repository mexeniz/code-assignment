//
//  avltree.c
//  spreadsheet
//
//  Created by pawissakan.ch on 6/9/2559 BE.
//  Copyright © 2559 Throughwave. All rights reserved.
//

#include "avltree.h"

struct AVLTree* avltree_init(){
    struct AVLTree* tree = malloc(sizeof(struct AVLTree));
    tree->root = NULL;
    tree->size = 0;
    return tree;
}
void inorder(struct AVLTree* tree);
struct Node* get_node(struct AVLTree* tree,int x, int y){
    struct Node* r = tree->root;
    while(r != NULL){
        if (r->x == x && r->y ==y) return r;
        int cmp = (i > r->x) || (i == r->x && j > r->y);
    }
    return NULL
    
}
struct Node* add_node(struct AVLTree* tree, char* val, char* formula, char* x, char* y){
    // Map Position A1 -> x=0 y=0
    int i = x[0]-65;
    int j = atoi(y)-1;
    struct Node* node = node_init(val, formula, i, j);
    if (tree->root == NULL){
        tree->root = node;
    }
    else{
        struct Node* p = NULL;
        struct Node* r = tree->root;
        while(r!=NULL){
            int cmp = (i > r->x) || (i == r->x && j > r->y);
            if(cmp){
                p = r;
                r = r->right;
            }else{
                p = r;
                r = r->left;
            }
        }
        int cmp = (i > p->x) || (i == p->x && j > p->y);
        if(cmp){
            p->right = node;
        }else{
            p->left = node;
        }
    }
    
    tree->size++;
    return node;
    
    
}
void remove_node(struct AVLTree* tree, int x, int y);
