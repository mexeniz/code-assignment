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

    printf("Start Interface -> cell format X:Y\n");
    printf("set <cell> <value>\n");
    printf("get <cell>\n");
    printf("recalc\n");
    printf("sum <cell1> <cell2>\n");
    while(1){
        char cmd[50];
        fgets(cmd,50,stdin);
        char* token = strtok(cmd, " ");

        char* attr[3];
        int i = 0 ;
        while(token){
            attr[i] = token;
            i++;
            token = strtok(NULL, " ");
        }
        printf("%s %s %s",attr[0],attr[1],attr[2]);
        int c = -1;
        if(strcmp(attr[0],"set") == 0) c = 0;
        else if (strcmp(attr[0],"get") == 0) c = 1;
        else if (strcmp(attr[0],"recalc") == 0) c = 2;
        else if (strcmp(attr[0],"sum") == 0) c = 3;
        char* pos[2] ;
        char* pos2[2] ;
        struct Node* node;
        switch (c) {
            case 0:
                split_pos(pos,attr[1]);
                node = get_node(sheet->tree,pos[0],pos[1]);
                node->val = attr[2];
                printf("X=%s Y=%s Val=%s Formula=%s\n", pos[0], pos[1], node->val, node->formula);
                update_formula_node(sheet->tree,sheet->tree->root);
                break;
            case 1:
                //GET A2
                split_pos(pos,attr[1]);
                node = get_node(sheet->tree,pos[0],pos[1]);
                printf("X=%s Y=%s Val=%s Formula=%s\n", pos[0], pos[1], node->val, node->formula);
                break;
            case 2:
                //RECALCULATE
                printf("Update spreadsheet");
                update_formula_node(sheet->tree,sheet->tree->root);
                break;
            case 3:
                //SUM
                split_pos(pos,attr[1]);
                split_pos(pos2,attr[2]);
                char* sum_res = sum(sheet, pos[0], pos[1], pos2[0], pos2[1]);
                printf("sum(%s%s:%s%s)=%s\n",pos[0], pos[1], pos2[0], pos2[1],sum_res);
                break;

            default:
                printf("Wrong command!");
                break;
        }
    }
}
