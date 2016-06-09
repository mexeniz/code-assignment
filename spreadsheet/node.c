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
    new_node->parent = NULL;
    new_node->right = NULL;
    new_node->left = NULL;
//    // Cell Position A1
//    int x
//    if (i[0] == 'A'){
//        x = 0;
//    }else if (x[])
//    int y = atoi(j);
//    
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