//
//  spreadsheet.h
//  mySpreadSheet
//
//  Created by Bankde on 7/6/16.
//  Copyright © 2016 Bankde. All rights reserved.
//

#ifndef spreadsheet_h
#define spreadsheet_h

#include "cell.h"

struct spreadsheet{
    struct row* firstRow;
    struct row* lastRow;
    int totalCell;
    int sizeX;
    int sizeY;
};

struct row{
    struct cell* firstCell;
    struct cell* lastCell;
    struct row* nextRow;
    struct row* prevRow;
    int pointY;
    int totalCell;
    int sizeX;
};

struct spreadsheet* spreadsheetInit();
struct row* rowInit();

int spreadSheetInsertCell(struct spreadsheet* inSheet, char* row, char* column, char* value);
struct cell* spreadSheetGetCell(struct spreadsheet* inSheet, int row, int column);
char* spreadSheetGetCellValue(struct spreadsheet* inSheet, char* row, char* column);
char* spreadSheetGetCellFormula(struct spreadsheet* inSheet, char* row, char* column);
int calculate(char* eq);
int colCharToInt(char* col);


#endif /* spreadsheet_h */
