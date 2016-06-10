//
//  node.h
//  spreadsheet
//
//  Created by pawissakan.ch on 6/9/2559 BE.
//  Copyright © 2559 Throughwave. All rights reserved.
//

#ifndef node_h
#define node_h

#include <stdio.h>
struct Node{
    char* val;
    char* formula;
    char* posX;
    char* posY;
    int x;
    int y;
    int height;
    struct Node* left;
    struct Node* right;
};

struct Node* node_init(char* val, char* formula, char* posX, char* posY, int x, int y);
int is_leaf(struct Node* node);
int is_formula(struct Node *node);
struct Node* rotate_left_child(struct Node *node);
struct Node* rotate_right_child(struct Node *node);
struct Node* rebalance(struct Node *node);
int set_height(struct Node *node);
int balance_val(struct Node* node);
char* operation(char op, char* opr1 ,char* opr2);

#endif /* node_h */
