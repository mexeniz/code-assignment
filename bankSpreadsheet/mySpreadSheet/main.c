//
//  main.c
//  mySpreadSheet
//
//  Created by Bankde on 7/6/16.
//  Copyright © 2016 Bankde. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <unistd.h>
#include <pthread.h>
#include "debug.h"
#include "cell.h"
#include "spreadsheet.h"

#include <termios.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>

//#define MY_TIME_CONSTANT 1000

void help();
void mySigHandler(int s);
int *keyDetection();

void changemode(int);
int  kbhit(void);

char* keyboardInputs;
struct spreadsheet *mySpreadsheet;
void processInput();

// Thank you so much to Kanwa Saad in stackoverflow for his interesting code, however, his code is not perfect and need lot of fix (his code makes ctrl+x works but breaks everything else). I have solve everything by hardcoding, if it somehow bugs please forgive me.
int *keyDetection()
{
    int ch;
    int len;
    printf("Enter input: ");
    fflush(stdout);
    while(1)
    {
        ch = 0;
        changemode(1);
        
        fflush(stdout);
        fflush(stderr);
        while ( !kbhit() );      // Waiting for some keyboard input.
        
        // something has been detected. now get that.
        ch = getchar();
        len = strlen(keyboardInputs);
        DEBUG_PRINT("Key detected: %d\n", ch);
        
        if (ch == 24) // ctrl+x
        {
            printf("\nCtrl+X detected. Raise quit signal\n");
            raise(SIGQUIT);
            break;
        }
        else if (ch == 127) // backspace
        {
            if(len > 0)
            {
                keyboardInputs[len-1] = '\0';
            }
            printf("\33[2K\r");
            printf("Enter input: %s", keyboardInputs);
            fflush(stdout);
            
        }
        else if (ch == '\n') // enter/return/newline
        {
            processInput();
            keyboardInputs[0] = '\0';
            printf("Enter input: ");
            fflush(stdout);
        }
        else
        {
            keyboardInputs[len] = ch;
            keyboardInputs[len+1] = '\0';
        }
        changemode(0);
    }
    return 0;
}

// man7.org/linux/man-pages/man3/termios.3.html is bit useful here
void changemode(int dir)
{
    static struct termios oldt, newt;
    
    if ( dir == 1 )
    {
        tcgetattr( STDIN_FILENO, &oldt);
        newt = oldt;
//        newt.c_lflag &= ~( ICANON | ECHO );
        // ICANON means non-blocking, ECHO is printing out
        newt.c_lflag &= ~(ICANON) | ECHO;
        tcsetattr( STDIN_FILENO, TCSANOW, &newt);
    }
    else
        tcsetattr( STDIN_FILENO, TCSANOW, &oldt);
}

// seriously I have no idea how this works. I know timeval but fuck to anything else in this func.
int kbhit (void)
{
    struct timeval tv;
    fd_set rdfs;
    
    tv.tv_sec = 0;
    tv.tv_usec = 0;
    
    FD_ZERO(&rdfs);
    FD_SET (STDIN_FILENO, &rdfs);
    
    select(STDIN_FILENO+1, &rdfs, NULL, NULL, &tv);
    return FD_ISSET(STDIN_FILENO, &rdfs);
    
}

// Hello magic
void mySigHandler(int s)
{
    switch(s)
    {
        case SIGHUP:
            printf("SigHub being caught: %d\n", s);
            break;
        case SIGINT:
            printf("SigInt being caught: %d\n", s);
            printf("Program is exited\n");
            exit(0);
            break;
        case SIGQUIT:
            // The program is automatically quit by this signal. In case sth went wrong...
            exit(0);
            break;
            
    }
}

int main(int argc, const char * argv[]) {
//    char* input = (char*)malloc(sizeof(char)*125);
//    char* row;
//    char* col;
//    char* inVal;
//    char* token;
//    char* inputTok;
//    pthread_t pth;
    
    struct sigaction sigHandler;
    
    keyboardInputs = (char*)malloc(sizeof(char)*1000);
    keyboardInputs[0] = '\0';
    mySpreadsheet = spreadsheetInit();
    
    sigHandler.sa_handler = mySigHandler;
    sigemptyset(&sigHandler.sa_mask);
    sigHandler.sa_flags = 0;
    
    
    sigaction(SIGINT, &sigHandler, NULL);
    sigaction(SIGHUP, &sigHandler, NULL);
    
    keyDetection();
    // Apparently we don't need multithread for now...
//    pthread_create(&pth, NULL, keyDetection, NULL);
    
    // move all this code to new function
//    while(0)
//    {
//        printf("Enter input: ");
//        scanf("%100s", input);
//        token = strtok_r(input, ",", &inputTok);
//        DEBUG_PRINT("User cmd: %s\n", input);
//        if (strnlen(token,2) > 1)
//        {
//            printf("Command not found\n");
//            help();
//        }
//        else if (strncmp(token, "w", 1) == 0)
//        {
//            col = strtok_r(NULL, ",", &inputTok);
//            row = strtok_r(NULL, ",", &inputTok);
//            inVal = strtok_r(NULL, ",", &inputTok);
//            if (!col || !row || !inVal)
//            {
//                fprintf(stderr, "You need 3 arguments here: col,row,val");
//                continue;
//            }
//            spreadSheetInsertCell(mySpreadsheet, row, col, inVal);
//        }
//        else if (strncmp(token, "q", 1) == 0)
//            break;
//        else if (strncmp(token, "r", 1) == 0)
//        {
//            col = strtok_r(NULL, ",", &inputTok);
//            row = strtok_r(NULL, ",", &inputTok);
//            if (!col || !row)
//            {
//                fprintf(stderr, "You need 2 arguments here: col,row");
//                continue;
//            }
//            printf("Cell %s%s: %s\n", col, row, spreadSheetGetCellValue(mySpreadsheet, row, col) );
//        }
//        else if (strncmp(token, "f", 1) == 0)
//        {
//            col = strtok_r(NULL, ",", &inputTok);
//            row = strtok_r(NULL, ",", &inputTok);
//            if (!col || !row)
//            {
//                fprintf(stderr, "You need 2 arguments here: col,row");
//                continue;
//            }
//            printf("Cell %s%s: %s\n", col, row, spreadSheetGetCellFormula(mySpreadsheet, row, col) );
//        }
//        else if (strncmp(token, "h", 1) == 0)
//            help();
//        else
//        {
//            printf("Command not found\n");
//            help();
//        }
//    }
    
//    printf("%d\n", calculate("5+6*7/2*0+4*20-15")); //Answer should be 70
    
//    int colNum = colCharToInt("AAA");
//    printf("%d\n", colNum);
    
    pause(); // We don't really need pause because we already have infinite loop but hey I love pause()
    return 0;
}

void processInput()
{
    char* row;
    char* col;
    char* inVal;
    char* token;
    char* inputTok;
    
    if (strnlen(keyboardInputs, 2) == 0)
        return;
    
    token = strtok_r(keyboardInputs, ",", &inputTok);
    DEBUG_PRINT("User cmd: %s\n", keyboardInputs);
    if (strnlen(token,2) > 1)
    {
        printf("Command not found\n");
        help();
    }
    else if (strncmp(token, "w", 1) == 0)
    {
        col = strtok_r(NULL, ",", &inputTok);
        row = strtok_r(NULL, ",", &inputTok);
        inVal = strtok_r(NULL, ",", &inputTok);
        if (!col || !row || !inVal)
        {
            fprintf(stderr, "You need 3 arguments here: col,row,val");
            return;
        }
        spreadSheetInsertCell(mySpreadsheet, row, col, inVal);
    }
    else if (strncmp(token, "q", 1) == 0)
        exit(0);
    else if (strncmp(token, "r", 1) == 0)
    {
        col = strtok_r(NULL, ",", &inputTok);
        row = strtok_r(NULL, ",", &inputTok);
        if (!col || !row)
        {
            fprintf(stderr, "You need 2 arguments here: col,row");
            return;
        }
        printf("Cell %s%s: %s\n", col, row, spreadSheetGetCellValue(mySpreadsheet, row, col) );
    }
    else if (strncmp(token, "f", 1) == 0)
    {
        col = strtok_r(NULL, ",", &inputTok);
        row = strtok_r(NULL, ",", &inputTok);
        if (!col || !row)
        {
            fprintf(stderr, "You need 2 arguments here: col,row");
            return;;
        }
        printf("Cell %s%s: %s\n", col, row, spreadSheetGetCellFormula(mySpreadsheet, row, col) );
    }
    else if (strncmp(token, "h", 1) == 0)
        help();
    else
    {
        printf("Command not found\n");
        help();
    }
    
    // if there is any stdout or stderr stuck
    fflush(stdout);
    fflush(stderr);

}

void help()
{
    printf("=================\n");
    printf("This is help mode\n");
    printf("To write: w,{col},{row},{value or formula}\n");
    printf("\t\tw,A,1,7\n");
    printf("\t\tw,B,1,=$A1*5\n");
    printf("To read: r,{col},{row}\n");
    printf("\t\tr,B,1\n");
    printf("To read formula: f,{col},{row}\n");
    printf("\t\tf,B,1\n");
    printf("To quit: q\n");
    printf("PS: column must be in UPPERCASE\n");
    printf("=================\n");
}
