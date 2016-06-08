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
struct Spreadsheet{
    struct Cell* root;
    char* name;
    int row;
    int col;
    int n_cell;
};
struct Cell{
    char* val;
    int x;
    int y;
};

#endif /* spreadsheet_h */
