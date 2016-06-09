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