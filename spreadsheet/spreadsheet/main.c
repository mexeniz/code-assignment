//
//  main.c
//  spreadsheet
//
//  Created by pawissakan-throughwave on 6/8/2559 BE.
//  Copyright © 2559 Throughwave. All rights reserved.
//

#include <stdio.h>
#include "spreadsheet.h"
#include "debug.h"
#include<signal.h>
#include<unistd.h>

void sig_handler(int signo)
{
    if (signo == SIGINT)
        printf("received SIGINT\n");
}

int main(int argc, const char * argv[]) {
    if (signal(SIGINT, sig_handler) == SIG_ERR)
        printf("\ncan't catch SIGINT\n");
    
    // insert code here...
    printf("Hello, World!\n");
    struct Spreadsheet* sheet = spreadsheet_init("Test", 1, 1);
    add_node(sheet->tree, "20", NULL, "A", "1");
    add_node(sheet->tree, "30", NULL, "A", "2");
    add_node(sheet->tree, "50", NULL, "A", "3");
    add_node(sheet->tree, "2", NULL, "B", "2");
    printf("Add formula\n");
    add_node(sheet->tree, "", "A1+A2", "C", "1");
    printf("Add formula\n");
    add_node(sheet->tree, "", "A2/B2", "C", "2");
    printf("Add formula\n");
    add_node(sheet->tree, "", "B2*A3", "C", "3");
    printf("Edit cell\n");
    add_node(sheet->tree, "25", NULL, "A", "3");
    char* res = sum(sheet, "A", "1", "A", "3");
    printf("%s",res);
    DEBUG_PRINT("FINISH INIT\n");
    print_tree(sheet->tree);
    
    printf("Start Interface\n");
    while(1){
        char cmd[256];
        scanf("%s", &cmd);
        char* token = strtok(cmd, " ");
        printf("token: %s\n", token);
        token = strtok(NULL, " ");
        
        switch (0) {
            case 0:
                break;
                
            default:
                printf("Wrong command!");
                break;
        }
    }
}
