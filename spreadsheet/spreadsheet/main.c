//
//  main.c
//  spreadsheet
//
//  Created by pawissakan-throughwave on 6/8/2559 BE.
//  Copyright © 2559 Throughwave. All rights reserved.
//

#include <stdio.h>
#include <ctype.h>
#include "spreadsheet.h"
#include "debug.h"
#include <signal.h>
#include <unistd.h>

void sig_handler(int signo);
void print_help();
void sample_data(struct Spreadsheet* sheet);
void sum_cell_list(int list_len, char* list[list_len]);

struct Spreadsheet* sheet;

int main(int argc, const char * argv[]) {
      if (signal(SIGINT, sig_handler) == SIG_ERR)
          printf("\ncan't catch SIGINT\n");
      if (signal(SIGHUP, sig_handler) == SIG_ERR)
          printf("\ncan't catch SIGHUP\n");

    sheet = spreadsheet_init("Test", 1, 1);
    sample_data(sheet);

    print_help();

    while(1){
        printf("$sheet-cmd: ");
        char* cmd = (char*) malloc(sizeof(char)*50);
        fgets(cmd,50,stdin);
        trimwhitespace(cmd);
        char* token = strtok(cmd, " ");

        char* attr[3];
        attr[2] = calloc(50, 50);
        int i = 0 ;
        while(token){
            attr[i] = token;
            i++;
            token = strtok(NULL, " ");
        }
        DEBUG_PRINT("%s %s %s\n",attr[0],attr[1],attr[2]);
        int c = -1;
        if(strcmp(attr[0],"set") == 0) c = 0;
        else if (strcmp(attr[0],"get") == 0) c = 1;
        else if (strcmp(attr[0],"recalc") == 0) c = 2;
        else if (strcmp(attr[0],"sum") == 0) c = 3;
        else if (strcmp(attr[0],"sum-list") == 0) c = 4;
        else if (attr[0][0]==24) c = 5;
        else if (strcmp(attr[0],"list") == 0) c = 6;

        char* pos[2] ;
        char* pos2[2] ;
        struct Node* node;
        int n_cell ;
        switch (c) {
            case 0:
                trimwhitespace(attr[2]);
                if (strlen(attr[2])==0) {
                    printf("Attribute is invalid.\n");
                    continue;
                }

                split_pos(pos,attr[1]);

                node = get_node(sheet->tree,pos[0],pos[1]);
                if(node == NULL){
                    // New Node
                    node = add_node(sheet->tree, "", "", pos[0],pos[1] );
                }

                char f = attr[2][0];
                if(f == '='){
                    // Add formula cell
                    char* formula = (char*) malloc(sizeof(char)*(strlen(attr[2])-1));
                    for(c = 1 ; c < strlen(attr[2]) ; c++){
                        formula[c-1] = attr[2][c] ;
                    }
                    DEBUG_PRINT("New Formula\n");
                    node->formula = formula;
                }else{
                    // Add value cell
                    DEBUG_PRINT("New Val\n");
                    node->val = attr[2];
                }
                update_formula_node(sheet->tree,sheet->tree->root);
                printf("CELL %s%s Val=%s Formula=%s\n", pos[0], pos[1], node->val, node->formula);
                break;
            case 1:
                //GET A2
                split_pos(pos,attr[1]);
                node = get_node(sheet->tree,pos[0],pos[1]);
                if (node == NULL){
                    printf("No cell at X=%s Y=%s\n",pos[0],pos[1]);
                    continue;
                }
                printf("CELL %s%s Val=%s Formula=%s\n", pos[0], pos[1], node->val, node->formula);
                break;
            case 2:
                //RECALCULATE
                printf("Update spreadsheet\n");
                update_formula_node(sheet->tree,sheet->tree->root);
                break;
            case 3:
                //SUM
                split_pos(pos,attr[1]);
                split_pos(pos2,attr[2]);
                char* sum_res = sum(sheet, pos[0], pos[1], pos2[0], pos2[1]);
                printf("sum(%s%s:%s%s)=%s\n",pos[0], pos[1], pos2[0], pos2[1],sum_res);
                break;
            case 4:
                //SUM LIST
                // ...BUG WARNING...
                // SEGMENTATION FAUlT :(
                // Trim last item
                trimwhitespace(attr[i-1]);
                sum_cell_list(i,attr);
                break;

            case 5:
                //CTRL+X = CANCEL
                printf("Exit Program...");
                exit(1);
                break;
            case 6:
                //LIST
                print_sheet(sheet);
                break;
            default:
                printf("Wrong command!\n");
                print_help();
                break;
        }

        // Clean variable
        pos[2] = NULL ;
        pos2[2] = NULL ;
        node = NULL;
        n_cell = 0 ;
    }
}



void sig_handler(int signo)
{
    //printf("%d\n",signo);
    if (signo == SIGINT){
        printf("Do you want to exit? (yes/no):");
        char s[256];
        scanf("%s",s);
        int cmp = strcmp(s, "yes");
        int cmp2 = strcmp(s, "no");
        if( cmp ==0){
            printf("type yes\n");
            exit(1);
        }else if(cmp2 == 0){
            printf("type no\n");
        }
    }
    else if (signo == SIGHUP)
        printf("received HUP\n");
}
void print_help(){
    printf("**********C Spreadsheet***********\n");
    printf("CLI -> cell format XY\n");
    printf("1 : set <cell> <value>, set <cell> =<formula> ; ex. set B1 =A2+A3\n");
    printf("2 : get <cell> ; ex. get A1\n");
    printf("3 : recalc\n");
    printf("4 : sum <cell1> <cell2> ; ex. sum A1 C3\n");
    printf("!BUG! 5 : sum-list <cell1> <cell2> ... <cell-i> ; ex. sum-list A2 A3 B1 C1\n");
    printf("6 : list\n");
    printf("**********************************\n");
}
void sample_data(struct Spreadsheet *sheet){
    add_node(sheet->tree, "20", NULL, "A", "1");
    add_node(sheet->tree, "30", NULL, "A", "2");
    add_node(sheet->tree, "50", NULL, "A", "3");
    add_node(sheet->tree, "2", NULL, "B", "2");
    DEBUG_PRINT("Add formula\n");
    add_node(sheet->tree, "", "A1+A2", "C", "1");
    DEBUG_PRINT("Add formula\n");
    add_node(sheet->tree, "", "A2/B2", "C", "2");
    DEBUG_PRINT("Add formula\n");
    add_node(sheet->tree, "", "A1+A2", "B", "1");
    DEBUG_PRINT("Add formula\n");
    add_node(sheet->tree, "", "B2*A3", "C", "3");
    DEBUG_PRINT("Edit cell\n");
    add_node(sheet->tree, "25", NULL, "A", "3");
    DEBUG_PRINT("FINISH INIT\n");
    // print_sheet(sheet);
}
void sum_cell_list(int list_len, char* list[list_len]){

    //Ignore first item (command)
    int n_cell = list_len -1 ;
    char* cell_pos[n_cell];
    int i ;
    for(i = 1 ; i < n_cell + 1 ; i++){
        cell_pos[i-1] = list[i] ;
    }

    char* res = sum_list(sheet, n_cell, cell_pos);
    printf("sum of %d cells =%s\n", n_cell, res);
}
