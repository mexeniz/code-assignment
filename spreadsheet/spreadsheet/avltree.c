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
        printf("CELL %s%s Val=%s Formula=%s\n",node->posX,node->posY,node->val,node->formula);
    }else{
        printf("CELL %s%s Val=%s\n",node->posX,node->posY,node->val);
    }
    inorder(tree, node->right);
}
void update_formula_node(struct AVLTree* tree, struct Node* node){
    if (node == NULL) return;
    update_formula_node(tree, node->left);
    if(is_formula(node)){
        calculate(tree,node);
    }
    update_formula_node(tree, node->right);
}
struct Node* get_node_i(struct AVLTree* tree,int i, int j){
    // Map Position A1 -> x=0 y=0
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
    struct Node* node = node_init(val, formula,x,y, i, j);
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
    update_formula_node(tree, tree->root);
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
    int i,c ;
    i = -1;
    char op = NULL;
    for (c = 0 ; c < strlen(formula) ; c++){
        if(formula[c]=='+'){
            op = '+';
            i = c;
        }
        else if(formula[c]=='-'){
            op = '-';
            i = c;
        }
        else if(formula[c]=='*'){
            op = '*';
            i = c;
        }
        else if(formula[c]=='/'){
            op = '/';
            i = c;
        }
        if(op != NULL) break;
    }
    if (op == NULL) return val;
    
    struct Node *node1,*node2;
    char* pos1[2];
    char* pos2[2];
    
    if(i == -1) return NULL;
    split_pos_range(pos1,node->formula, 0,i);
    DEBUG_PRINT("opi=%d X1=%s Y1=%s\n",i,pos1[0],pos1[1]);
    split_pos_range(pos2,node->formula,i,(int)strlen(node->formula));
    DEBUG_PRINT("opi=%d X2=%s Y2=%s\n",i,pos2[0],pos2[1]);
    
    node1 = get_node(tree, pos1[0],pos1[1]);
    node2 = get_node(tree, pos2[0],pos2[1]);
    if(node1 == NULL || node2 == NULL) return NULL;
    else val = operation(op, node1->val,node2->val);
    node->val = val;
    return val;
}
char* operation(char op, char* opr1 ,char* opr2){
    int result ;
    if(opr1 == NULL || opr2 == NULL) return NULL;
    switch (op) {
        case '+':
            result = atoi(opr1)+atoi(opr2);
            break;
        case '-':
            result = atoi(opr1)-atoi(opr2);
            break;

        case '*':
            result = atoi(opr1)*atoi(opr2);
            break;

        case '/':
            if (atoi(opr2) == 0) {
                printf("Cannot divide by zero!\n");
                break;
            }
            result = atoi(opr1)/atoi(opr2);
            break;
    }
    char* buffer = malloc(sizeof(char)*10);
    sprintf(buffer, "%d", result);
    return buffer;
}
void split_pos(char** split,char* pos){
    char* x = malloc(sizeof(char)*3) ;
    char* y = malloc(sizeof(char)*20) ;
    int i ;
    for (i = 0 ; i < strlen(pos) ; i++){
        if (pos[i] <= 90 && pos[i] >= 64){
            // X Position
            int len = (int) strlen(x) ;
            if(len==0){
                x[len] = pos[i];
                x[len+1] = '\0' ;
            }else if(len==1){
                x[len] = pos[i];
                x[len+1] = '\0' ;
            }
        }else if (pos[i] <= 57 && pos[i] >= 48){
            // Y Position
            int len = (int) strlen(y) ;
            y[len] = pos[i];
            y[len+1] = '\0';
        }
    }
    // printf("Split X=%s Y=%s", x, y);
    split[0] = x ;
    split[1] = y ;
}
void split_pos_range(char** split,char* pos, int i, int j){
    char* x = malloc(sizeof(char)*3) ;
    char* y = malloc(sizeof(char)*20) ;
    int c ;
    for (c = i ; c < j ; c++){
        if (pos[c] <= 90 && pos[c] >= 64){
            // X Position
            int len = (int) strlen(x) ;
            if(len==0){
                x[len] = pos[c];
                x[len+1] = '\0' ;
            }else if(len==1){
                x[len] = pos[c];
                x[len+1] = '\0' ;
            }
        }else if (pos[c] <= 57 && pos[c] >= 48){
            // Y Position
            int len = (int) strlen(y) ;
            y[len] = pos[c];
            y[len+1] = '\0';
        }
    }
    // printf("Split X=%s Y=%s", x, y);
    split[0] = x ;
    split[1] = y ;
}
