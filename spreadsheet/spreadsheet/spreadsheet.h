//
//  spreadsheet.h
//  spreadsheet
//
//  Created by pawissakan-throughwave on 6/8/2559 BE.
//  Copyright © 2559 Throughwave. All rights reserved.
//
#ifndef spreadsheet_h
#define spreadsheet_h

#include <stdio.h>
#include "avltree.h"
struct Spreadsheet{
    struct AVLTree* tree;
    char* name;
    int row;
    int col;
    int n_cell;
};

struct Spreadsheet* spreadsheet_init(char* name,int row,int col);
char* sum_list(struct Spreadsheet* sheet, int list_len, char* pos_list[list_len]);
char* sum(struct Spreadsheet* sheet, char* x1, char* y1, char* x2, char* y2);
void print_sheet(struct Spreadsheet* sheet);
void interpret_pos(int** pos_int, char* x, char* y);
void interpret_pos2(int** pos_int, char* cord);
char* trimwhitespace(char *str);

#endif /* spreadsheet_h */
