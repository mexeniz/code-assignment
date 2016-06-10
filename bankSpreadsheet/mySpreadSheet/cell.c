//
//  cell.c
//  mySpreadSheet
//
//  Created by Bankde on 9/6/16.
//  Copyright © 2016 Bankde. All rights reserved.
//

#include "cell.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "spreadsheet.h"
#include "debug.h"
#include "myUtils.h"

struct cell* cellInit(int pointX, int pointY)
{
    struct cell* new = (struct cell*)malloc(sizeof(struct cell));
    new->nextCell = 0;
    new->prevCell = 0;
    new->pointX = pointX;
    new->pointY = pointY;
    new->value = NULL;
    new->formula = NULL;
    
    // These two will be used for dynamically updated value (like Excel) in future, again I don't have time T T
    new->formUsed = NULL;
    new->formUsing = NULL;
    return new;
}

struct cell* cellInitWithVal(struct spreadsheet* inSheet, int pointX, int pointY, char* inVal)
{
    struct cell* new = cellInit(pointX, pointY);
    if (cellUpdateVal(inSheet, new, inVal) >= 0)
        return new;
    return NULL;
}

// In order to create dynamically update, also need parent Cell as argument to add formUsed and formUsing
// To expandFormula correctly, we need brackets but unfortunately we don't support this feature yet.
// This function now has to expand and calculate at the same time (because of no brackets)
int expandFormula(struct spreadsheet* inSheet, char* formula, char** result)
{
    char* ptr = formula;
    char* ptrCell;
    char row[10];
    char column[10];
    int index = 0;
    int rowNum = 0;
    int colNum = 0;
    int tempCal = 0;
    // This is shit (but I have limit length of cellInput from spreadsheet.c)
    char tempCharCal[250];
    struct cell* nextCell = NULL;
    char* nextResult = NULL;
    
    char* eq = NULL;
    char* prevFormula = NULL;
    
    // Again this is shit...
    eq = (char*)malloc(sizeof(char)*250);
    prevFormula = (char*)malloc(sizeof(char)*250);
    
    strcpy(prevFormula,formula);
    
    while(*ptr)
    {
        if (*ptr == '$')
        {
            ptrCell = ptr+1;
            index = 0;
            while (validateIntBetween(*ptrCell, 65, 90) >= 0) // if char between A-Z
            {
                column[index] = *ptrCell;
                ptrCell++;
                index++;
            }
            column[index] = '\0';
            
            index = 0;
            while (validateIntBetween(*ptrCell, 48, 57) >= 0) // if char between 0-9
            {
                row[index] = *ptrCell;
                ptrCell++;
                index++;
            }
            row[index] = '\0';
            
            strncpy(eq, prevFormula, ptr-formula);
            eq[ptr-formula] = '\0';
            
            rowNum = atoi(row)-1;
            colNum = colCharToInt(column);
            
            nextCell = spreadSheetGetCell(inSheet, rowNum, colNum);
            if (!nextCell)
                printf("Cell %s%s has no value or wrong input\n", column, row);
            if (!nextCell->formula)
                strcat(eq, nextCell->value);
            else
            {
                expandFormula(inSheet, nextCell->formula, &nextResult);
                tempCal = calculate(nextResult);
                sprintf(tempCharCal, "%d", tempCal);
                strcat(eq, tempCharCal);
                free(nextResult);
            }
            
            strcat(eq, ptrCell);
            
            strcpy(prevFormula,eq);
            eq[0] = '\0';
        }
        ptr++;
    }
    *result = prevFormula;
    free(eq);
    return 0;
}

int cellUpdateVal(struct spreadsheet* inSheet, struct cell* c, char* inVal)
{
    // This is also shit
    char* sVal = (char*)malloc(sizeof(char)*125);
    char* sFor = (char*)malloc(sizeof(char)*125);
    char* equation = NULL;
    
    if(c->value)
        free(c->value);
    if(c->formula)
        free(c->formula);
    
    if (*inVal == '=')
    {
        inVal++;
        DEBUG_PRINT("Recieve formula: %s\n",inVal);
        strcpy(sFor, inVal);
        c->formula = sFor;
        expandFormula(inSheet, c->formula, &equation);
        DEBUG_PRINT("Full formula: %s\n", equation);
        sprintf(sVal, "%d", calculate(equation));
        c->value = sVal;
    }
    else
    {
        strcpy(sVal, inVal);
        c->value = sVal;
    }
    
    return 0;
}