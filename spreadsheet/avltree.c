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
void print_tree(struct AVLTree* tree){
    inorder(tree,tree->root);
}
void inorder(struct AVLTree* tree, struct Node* node){
    if (node == NULL) return;
    inorder(tree, node->left);
    //Print Node Info
    if(is_formula(node)){
        printf("node %d:%d val=%s formula=%s\n",node->x,node->y,node->val,node->formula);
    }else{
        printf("node %d:%d val=%s\n",node->x,node->y,node->val);
    }
    inorder(tree, node->right);
}
void update_formula_node(struct AVLTree* tree, struct Node* node){
    if (node == NULL) return;
    inorder(tree, node->left);
    if(is_formula(node)){
        calculate(tree,node);
    }
    inorder(tree, node->right);
}
struct Node* get_node(struct AVLTree* tree,char* x, char* y){
    // Map Position A1 -> x=0 y=0
    int i = -1;
    if(strlen(x) > 2 || strlen(x) < 1){
        printf("Wrong first position!\n");
        return NULL;
    }
    else if(strlen(x)== 1){
        i = x[0]-65;
    }
    else if(strlen(x)== 2){
        i = x[0]*26+x[1]-65;
    }
    int j = atoi(y)-1;
    
    struct Node* r = tree->root;
    while(r != NULL){
        if (r->x == i && r->y ==j) return r;
        int cmp = (i > r->x) || (i == r->x && j > r->y);
        if (cmp){
            r = r->right;
        }else{
            r = r->left;
        }
    }
    return NULL ;
    
}
struct Node* add_node(struct AVLTree* tree, char* val, char* formula, char* x, char* y){
    // Map Position A1 -> x=0 y=0
    int i = -1;
    if(strlen(x) > 2 || strlen(x) < 1){
        printf("Wrong first position!\n");
        return NULL;
    }
    else if(strlen(x)== 1){
        i = x[0]-65;
    }
    else if(strlen(x)== 2){
        i = x[0]*26+x[1]-65;
    }
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
        if (i == p->x && j == p->y){
            // Found Same Position!
            DEBUG_PRINT("Same Position %s:%s Here! edit val and formula\n",x,y);
            free(node);
            node = p;
            node->val = val;
            node->formula = formula;
        }else{
            int cmp = (i > p->x) || (i == p->x && j > p->y);
            if(cmp){
                p->right = node;
            }else{
                p->left = node;
            }
        }
    }
    if(is_formula(node)){
        calculate(tree,node);
    }
    rebalance(tree->root);
    tree->size++;
    return node;
    
    
}
void remove_node(struct AVLTree* tree, char* x, char* y){
    struct Node* node = get_node(tree, x, y);
    if(node != NULL){
        free(node);
        rebalance(tree->root);
        tree->size--;
    }
    
}

char* calculate(struct AVLTree* tree, struct Node* node){
    char* val = "INVALID";
    char* formula = node->formula ;
    // Find index of operator
    int i ;
    char op = NULL;
    for (i = 0 ; i < strlen(formula) ; i++){
        if(formula[i]=='+') op = '+';
        else if(formula[i]=='-') op = '-';
        else if(formula[i]=='*') op = '*';
        else if(formula[i]=='/') op = '/';
        
        if(op != NULL) break;
    }
    if (op == NULL) return NULL;
    // Read first operand (x)
    struct Node *node1,*node2;
    // find last index of X1
    char X1[3] ;
    char Y1[20] ;
    int iX1 ;
    for (iX1= 0 ; iX1 < i ; iX1++){
        if (node->formula[iX1] <= 90 || node->formula[iX1] >= 64){
            // X Position
            int X1len = (int) strlen(X1) ;
            if(X1len==0){
                X1[X1len] = node->formula[iX1];
                X1[X1len+1] = '\0' ;
            }else if(X1len==1){
                X1[X1len] = node->formula[iX1];
                X1[X1len+1] = '\0' ;
            }
        }else if (node->formula[iX1] <= 57 || node->formula[iX1] >= 48){
            // Y Position
            int Y1len = (int) strlen(Y1) ;
            Y1[Y1len] = node->formula[iX1];
            Y1[Y1len+1] = '\0';
        }
    }
    // find last index of X1
    char X2[3] ;
    char Y2[20] ;
    int iX2 ;
    for (iX2 = i+1 ; iX2 < i ; iX2++){
        if (node->formula[iX2] <= 90 || node->formula[iX2] >= 64){
            // X Position
            int X2len = (int) strlen(X2) ;
            if(X2len==0){
                X2[X2len] = node->formula[iX2];
                X2[X2len+1] = '\0' ;
            }else if(X2len==1){
                X2[X2len] = node->formula[iX2];
                X2[X2len+1] = '\0' ;
            }
        }else if (node->formula[iX2] <= 57 || node->formula[iX2] >= 48){
            // Y Position
            int Y2len = (int) strlen(Y2) ;
            Y2[Y2len] = node->formula[iX2];
            Y2[Y2len+1] = '\0';
        }
    }
    node1 = get_node(tree, X1, Y1);
    node2 = get_node(tree, X2, Y2);
    int result ;
    if(node1 == NULL || node2 == NULL) return NULL;
    switch (op) {
        case '+':
            result = atoi(node1->val)+atoi(node2->val);
            break;
        case '-':
            result = atoi(node1->val)-atoi(node2->val);
            break;
            
        case '*':
            result = atoi(node1->val)*atoi(node2->val);
            break;
            
        case '/':
            result = atoi(node1->val)/atoi(node2->val);
            break;
    }
    if(result)itoa(result, val, 10);
    return val;
}