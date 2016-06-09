//
//  node.c
//  spreadsheet
//
//  Created by pawissakan.ch on 6/9/2559 BE.
//  Copyright © 2559 Throughwave. All rights reserved.
//

#include "node.h"
#include <stdlib.h>

struct Node* node_init(char* val, char* formula, int i, int j){
    struct Node* new_node = malloc(sizeof(struct Node));
    new_node->val = val;
    new_node->formula = formula;
    new_node->x = i;
    new_node->y = j;
    new_node->height = 0;
    new_node->right = NULL;
    new_node->left = NULL;
    return new_node;
}
int is_leaf(struct Node* node){
    if (node->left == NULL && node->right == NULL){
        return 1;
    }
    return 0;
}
int is_formula(struct Node* node){
    if(node->formula != NULL){
        return 1;
    }
    return 0;
}
int balance_val(struct Node* node){
    int h1 =0 , h2 = 0;
    if(node->right != NULL) h1 = (node->right)->height;
    if(node->left != NULL) h2= (node->left)->height;
    return h1-h2;
}
int set_height(struct Node* node){
    int h1 =0 , h2 = 0;
    if(node->right != NULL) h1 = (node->right)->height;
    if(node->left != NULL) h2= (node->left)->height;
    int max_height = h1 > h2 ? h1 : h2 ;
    node->height = 1 + max_height;
    return node->height ;
}
struct Node* rotate_left_child(struct Node *node){
    if(node ==NULL) return NULL;
    struct Node* new_root = node->left;
    node->left = new_root->right;
    new_root->right = node;
    set_height(new_root);
    set_height(new_root->right);
    return new_root;
}
struct Node* rotate_right_child(struct Node *node){
    if(node ==NULL) return NULL;
    struct Node* new_root = node->right;
    node->right = new_root->left;
    new_root->left = node;
    set_height(new_root);
    set_height(new_root->left);
    return new_root;
}
struct Node* rebalance(struct Node *node){
    if (node==NULL) return node;
    int balance = balance_val(node);
    if(balance==-2){
        int balance_left = balance_val(node->left);
        if(balance_left == 1){
            node->left = rotate_right_child(node->left);
        }
        node = rotate_left_child(node);
    }else if(balance==2){
        int balance_right = balance_val(node->right);
        if(balance_right ==-1){
            node->right = rotate_left_child(node);
        }
        node = rotate_right_child(node);
    }
    set_height(node);
    return node ;
}