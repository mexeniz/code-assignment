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
    int x;
    int y;
    struct Node* parent;
    struct Node* left;
    struct Node* right;
};

struct Node* node_init(char* val, char* formula, int i, int j);
char* calculate(struct Node* node);
int is_leaf(struct Node* node);
int is_formula(struct Node* node);
#endif /* node_h */
