//
//  spreadsheet.c
//  spreadsheet
//
//  Created by pawissakan-throughwave on 6/8/2559 BE.
//  Copyright © 2559 Throughwave. All rights reserved.
//

#include "spreadsheet.h"


struct Spreadsheet* spreadsheet_init(char* name,int row,int col){
    struct Spreadsheet* spreadsheet = malloc(sizeof(struct Spreadsheet));
    spreadsheet->tree = avltree_init();
    spreadsheet->name = name;
    spreadsheet->row = 0;
    spreadsheet->col = 0;
    spreadsheet->n_cell = 0;
    
    return spreadsheet;
}
char* sum(struct Spreadsheet* sheet,char* x1, char* y1, char* x2, char* y2){
    int result = 0 ;
    int i1 = -1;
    if(strlen(x1) > 2 || strlen(x1) < 1){
        printf("Wrong first position!\n");
        return NULL;
    }
    else if(strlen(x1)== 1){
        i1 = x1[0]-65;
    }
    else if(strlen(x1)== 2){
        i1 = x1[0]*26+x1[1]-65;
    }
    int j1 = atoi(y1)-1;

    int i2 = -1;
    if(strlen(x2) > 2 || strlen(x2) < 1){
        printf("Wrong first position!\n");
        return NULL;
    }
    else if(strlen(x2)== 1){
        i2 = x2[0]-65;
    }
    else if(strlen(x2)== 2){
        i2 = x2[0]*26+x2[1]-65;
    }
    int j2 = atoi(y2)-1;
    
    if (i1 == i2)
    {
        // Vertical Sum
        if (j1 > j2){
            int tmp = j1;
            j1 = j2;
            j2 = tmp;
        }
        int a ;
        for (a = j1; a <= j2 ;a++){
            struct Node *node = get_node_i(sheet->tree, i1 , a);
            result += atoi(node->val);
        }
        
    }
    else if (j1 == j2){
        // Horizontal Sum
        if (i1 > i2){
            int tmp = i1;
            i1 = j2;
            i2 = tmp;
        }
        int a ;
        for (a = i1 ; a <= j2 ;a++){
            struct Node *node = get_node_i(sheet->tree, a , i1);
            result += atoi(node->val);
        }
    
    }
    else {
        printf("Position error");
    }
    char* buffer = malloc(sizeof(char)*20);
    sprintf(buffer, "%d", result);
    
    return buffer;
}