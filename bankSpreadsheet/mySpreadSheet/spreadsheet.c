//
//  spreadsheet.c
//  mySpreadSheet
//
//  Created by Bankde on 7/6/16.
//  Copyright © 2016 Bankde. All rights reserved.
//

#include "spreadsheet.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "debug.h"
#include "myUtils.h"
#include "cell.h"

struct spreadsheet* spreadsheetInit()
{
    struct spreadsheet* new = (struct spreadsheet*)malloc(sizeof(struct spreadsheet));
    new->firstRow = NULL;
    new->lastRow = NULL;
    new->totalCell = 0;
    new->sizeX = 0;
    new->sizeY = 0;
    return new;
}

struct row* rowInit(int pointY)
{
    struct row* new = (struct row*)malloc(sizeof(struct row));
    new->pointY = pointY;
    new->firstCell = NULL;
    new->lastCell = NULL;
    new->nextRow = NULL;
    new->prevRow = NULL;
    new->sizeX = 0;
    new->totalCell = 0;
    return new;
}

int spreadSheetInsertCell(struct spreadsheet* inSheet, char* row, char* column, char* cellInput)
{
    int colNum, rowNum;
    struct row* r;
    struct cell* c;
    struct row* newRow;
    struct cell* newCell;
    
    rowNum = atoi(row) - 1;
    
    if (strnlen(column,5) > 4 || rowNum > 10000000)
    {
        fprintf(stderr, "Row or Column is exceeding appropriate value (row > 4 or column > 10m)");
        return -1;
    }
    // I know this one is shit but it's best way to prevent overflow for now.
    if (strnlen(cellInput,100) > 100)
    {
        fprintf(stderr, "Cell value is too long (must be 0-100 chars)");
        return -1;
    }
    colNum = colCharToInt(column);
    DEBUG_PRINT("row: %d\tcol: %d\n", rowNum, colNum);
    
    if (!inSheet->firstRow)
    {
        newRow = rowInit(rowNum);
        newCell = cellInitWithVal(inSheet, colNum, rowNum, cellInput);
        newRow->firstCell = newCell;
        newRow->lastCell = newCell;
        newRow->sizeX = colNum+1;
        newRow->totalCell++;
        
        inSheet->firstRow = newRow;
        inSheet->lastRow = newRow;
        inSheet->sizeY = rowNum+1;
        inSheet->sizeX = colNum+1;
        inSheet->totalCell++;
        
        return 0;
    }
    
    r = inSheet->firstRow;
    while (r)
    {
        if (r->pointY == rowNum)
        {
            c = r->firstCell;
            while(c)
            {
                if (c->pointX == colNum && c->pointY == rowNum)
                {
                    // Need check wheter it's formula or value
                    cellUpdateVal(inSheet, c, cellInput);
                    return 0;
                }
                c = c->nextCell;
            }
            // Need sort here, will be done later
            newCell = cellInitWithVal(inSheet, colNum, rowNum, cellInput);
            r->lastCell->nextCell = newCell;
            newCell->prevCell = r->lastCell;
            r->lastCell = newCell;
            r->totalCell++;
            if (r->sizeX < colNum+1)
                r->sizeX = colNum+1;
            if (inSheet->sizeX < colNum+1)
                inSheet->sizeX = colNum+1;
            inSheet->totalCell++;
            
            return 0;
        }
        r = r->nextRow;
    }
    newRow = rowInit(rowNum);
    inSheet->lastRow->nextRow = newRow;
    newRow->prevRow = inSheet->lastRow;
    inSheet->lastRow = newRow;
    
    newCell = cellInitWithVal(inSheet, colNum, rowNum, cellInput);
    newRow->firstCell = newCell;
    newRow->lastCell = newCell;
    newRow->sizeX = colNum+1;
    
    newRow->totalCell++;
    inSheet->totalCell++;
    
    if (inSheet->sizeX < colNum+1)
        inSheet->sizeX = colNum+1;
    if (inSheet->sizeY < rowNum+1)
        inSheet->sizeY = rowNum+1;

    return 0;
}

struct cell* spreadSheetGetCell(struct spreadsheet* inSheet, int row, int column)
{
    struct row* r;
    struct cell* c;
    
    r = inSheet->firstRow;
    while (r)
    {
        if (r->pointY == row)
        {
            c = r->firstCell;
            while(c)
            {
                if (c->pointX == column && c->pointY == row)
                {
                    return c;
                }
                c = c->nextCell;
            }
        }
        r = r->nextRow;
    }
    return NULL;
}

char* spreadSheetGetCellValue(struct spreadsheet* inSheet, char* row, char* column)
{
    int colNum = 0;
    int rowNum = atoi(row) - 1;
    struct cell* c;
    char temp[125];
    
    if (strnlen(column,5) > 4 || rowNum > 10000000)
    {
        fprintf(stderr, "Row or Column is exceeding appropriate value (row > 4 or column > 10m)");
        return NULL;
    }
    colNum = colCharToInt(column);

    
    if ((c = spreadSheetGetCell(inSheet, rowNum, colNum)))
    {
        // I haven't support dynamically update yet, have to do this.
        if (c->formula)
        {
            temp[0] = '=';
            temp[1] = '\0';
            strcat(temp,c->formula);
            cellUpdateVal(inSheet, c, temp);
        }
        DEBUG_PRINT("GetCellValue %s%s: %s\n", column, row, c->value);
        return c->value;
    }
    else
    {
        DEBUG_PRINT("GetCellValue %s%s not found.\n", column, row);
        return NULL;
    }
}

char* spreadSheetGetCellFormula(struct spreadsheet* inSheet, char* row, char* column)
{
    struct cell* c;
    int colNum = 0;
    int rowNum = 0;
    
    if (strnlen(column,5) > 4 || rowNum > 10000000)
    {
        fprintf(stderr, "Row or Column is exceeding appropriate value (row > 4 or column > 10m)");
        return NULL;
    }
    colNum = colCharToInt(column);
    
    
    if ((c = spreadSheetGetCell(inSheet, rowNum, colNum)))
    {
        DEBUG_PRINT("GetCellFormula %s%s: %s\n", column, row, c->formula);
        return c->value;
    }
    else
    {
        DEBUG_PRINT("GetCellValue %s%s not found.\n", column, row);
        return NULL;
    }
}

int calculate(char* eq)
{
    char* ptr = eq;
    char* searchPtr = NULL;
    char x[20];
    char y[20];
    // I know this will eat lot of RAM but it's safe from overflow by lot of multiplication
    char newEq[strlen(eq)*2+1];
    char strFromInt[20];
    
    char* strtokStore;
    char tempEq[strlen(eq)*2+1];
    char delimit[] = "*/+-";
    char *remain = NULL;
    int a, b;
    char *tempChar1 = NULL;
    char *tempChar2 = NULL;
    char cmd;
    
    strcpy(tempEq, eq);
    
    tempChar1 = strtok_r(tempEq, delimit, &strtokStore);
    cmd = eq[strlen(tempChar1)];
    tempChar2 = strtok_r(NULL, delimit, &strtokStore);
    remain = eq + strlen(tempChar1) + strlen(tempChar2) + 1;
    
    DEBUG_PRINT("Eq receieved: %s %c %s %s\n", tempChar1, cmd, tempChar2, remain);
    if (tempChar1 && !cmd && !tempChar2 && !*remain)
        return atoi(tempChar1);
    else if (tempChar1 && cmd && tempChar2 && !*remain)
    {
        DEBUG_PRINT("End of eq\n");
        a = atoi(tempChar1); // I have to duplicate code here because detecting if char is not null is possible, however not possible with integer
        b = atoi(tempChar2);
        if(cmd == '*')
            return a*b;
        if(cmd == '/')
            return a/b;
        if(cmd == '+')
            return a+b;
        if(cmd == '-')
            return a-b;
    }
    else if (tempChar1 && cmd && tempChar2 && *remain)
    {
        a = atoi(tempChar1);
        b = atoi(tempChar2);
        while(*ptr)
        {
            if(*ptr == '*' || *ptr == '/')
            {
                searchPtr = ptr-1;
                // I will need to fix here in order to accept something like 5+-4 or 4*-3
                while(searchPtr >= eq && *searchPtr != '+' && *searchPtr != '-' && *searchPtr != '*' && *searchPtr != '/')
                    searchPtr--;
                strncpy(x, searchPtr+1, ptr-searchPtr-1);
                x[ptr-searchPtr-1] = '\0';
                strncpy(newEq, eq, searchPtr-eq+1);
                newEq[searchPtr-eq+1] = '\0';
                
                searchPtr = ptr+1;
                while(*searchPtr != '+' && *searchPtr != '-' && *searchPtr != '*' && *searchPtr != '/' && *searchPtr != '\0')
                    searchPtr++;
                strncpy(y, ptr+1, searchPtr-ptr-1);
                y[searchPtr-ptr-1] = '\0';

                if (*ptr == '*')
                    sprintf(strFromInt, "%d", atoi(x)*atoi(y));
                else
                    sprintf(strFromInt, "%d", atoi(x)/atoi(y));
                strcat(newEq, strFromInt);
                sprintf(newEq, "%s%s", newEq, searchPtr);
//                    strncat(newEq, searchPtr, strlen(eq)-(searchPtr-eq));
                
                DEBUG_PRINT("NextEq: %s\n", newEq);
                return calculate(newEq);
                
            }
            ptr++;
        }
        ptr = eq;
        while(*ptr)
        {
            if(*ptr == '+' || *ptr == '-')
            {
                searchPtr = ptr-1;
                while(searchPtr >= eq && *searchPtr != '+' && *searchPtr != '-' && *searchPtr != '*' && *searchPtr != '/')
                    searchPtr--;
                strncpy(x, searchPtr+1, ptr-searchPtr-1);
                x[ptr-searchPtr-1] = '\0';
                strncpy(newEq, eq, searchPtr-eq+1);
                newEq[searchPtr-eq+1] = '\0';
                
                searchPtr = ptr+1;
                while(*searchPtr != '+' && *searchPtr != '-' && *searchPtr != '*' && *searchPtr != '/' && *searchPtr != '\0')
                    searchPtr++;
                strncpy(y, ptr+1, searchPtr-ptr-1);
                y[searchPtr-ptr-1] = '\0';
                
                if (*ptr == '+')
                    sprintf(strFromInt, "%d", atoi(x)+atoi(y));
                else
                    sprintf(strFromInt, "%d", atoi(x)-atoi(y));
                strcat(newEq, strFromInt);
                sprintf(newEq, "%s%s", newEq, searchPtr);
//                    strncat(newEq, searchPtr, strlen(eq)-(searchPtr-eq));
                
                DEBUG_PRINT("NextEq: %s\n", newEq);
                return calculate(newEq);
                
            }
            ptr++;
        }
    }
    return 0;
}


int colCharToInt(char* col)
{
    int colNum;
    
    if (validateIntBetween(*col, 64, 90) >= 0)
        colNum = *col-64;
    else
        return -1;
    
    col++;
    while(*col)
    {
        if (validateIntBetween(*col, 64, 90) >= 0)
            colNum = colNum*26 + (*col-64);
        else
            return -1;
        col++;
//        index--;
    }
    // Indexing starts at 0
    colNum = colNum - 1;
    return colNum;
}