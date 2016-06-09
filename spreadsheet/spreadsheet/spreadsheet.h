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
#endif /* spreadsheet_h */
