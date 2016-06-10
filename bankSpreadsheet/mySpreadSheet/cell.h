//
//  cell.h
//  mySpreadSheet
//
//  Created by Bankde on 7/6/16.
//  Copyright © 2016 Bankde. All rights reserved.
//

#ifndef cell_h
#define cell_h

struct spreadsheet;

struct cell{
    int pointX;
    int pointY;
    struct cell* nextCell;
    struct cell* prevCell;
    char* value;
    char* formula;
    struct cell** formUsing;
    struct cell** formUsed;
};

struct cell* cellInit(int pointX, int pointY);
int cellUpdateVal(struct spreadsheet* inSheet, struct cell* c, char* inVal);
struct cell* cellInitWithVal(struct spreadsheet* inSheet, int pointX, int pointY, char* inVal);

#endif /* cell_h */
