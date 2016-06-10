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

char *trimwhitespace(char *str);
void sig_handler(int signo);

int main(int argc, const char * argv[]) {
      if (signal(SIGINT, sig_handler) == SIG_ERR)
          printf("\ncan't catch SIGINT\n");
      if (signal(SIGHUP, sig_handler) == SIG_ERR)
          printf("\ncan't catch SIGHUP\n");
//    if (signal(SIGSTOP, sig_handler) == SIG_ERR)
//        printf("\ncan't catch SIGSTOP\n");
    // insert code here...
    
    struct Spreadsheet* sheet = spreadsheet_init("Test", 1, 1);
    add_node(sheet->tree, "20", NULL, "A", "1");
    add_node(sheet->tree, "30", NULL, "A", "2");
    add_node(sheet->tree, "50", NULL, "A", "3");
    add_node(sheet->tree, "2", NULL, "B", "2");
    DEBUG_PRINT("Add formula\n");
    add_node(sheet->tree, "", "A1+A2", "C", "1");
    DEBUG_PRINT("Add formula\n");
    add_node(sheet->tree, "", "A2/B2", "C", "2");
    DEBUG_PRINT("Add formula\n");
    add_node(sheet->tree, "", "B2*A3", "C", "3");
    DEBUG_PRINT("Edit cell\n");
    add_node(sheet->tree, "25", NULL, "A", "3");
    char* res = sum(sheet, "A", "1", "A", "3");
    DEBUG_PRINT("%s",res);
    DEBUG_PRINT("FINISH INIT\n");
    print_tree(sheet->tree);
    
    printf("**********C Spreadsheet***********\n");
    printf("Start Interface -> cell format XY\n");
    printf("1 : set <cell> <value>, set <cell> =<formula>\n");
    printf("2 : get <cell>\n");
    printf("3 : recalc\n");
    printf("4 : sum <cell1> <cell2>\n");
    printf("**********************************\n");
    
    while(1){
        printf("$sheet-cmd: ");
        char cmd[50];
        fgets(cmd,50,stdin);
        char* token = strtok(cmd, " ");

        char* attr[3];
        attr[2] = calloc(50, 50);
        int i = 0 ;
        while(token){
            attr[i] = token;
            i++;
            token = strtok(NULL, " ");
        }
        DEBUG_PRINT("%s %s %s",attr[0],attr[1],attr[2]);
        int c = -1;
        if(strcmp(attr[0],"set") == 0) c = 0;
        else if (strcmp(attr[0],"get") == 0) c = 1;
        else if (strcmp(attr[0],"recalc") == 0) c = 2;
        else if (strcmp(attr[0],"sum") == 0) c = 3;
        else if (attr[0][0]==24) c = 4;
        char* pos[2] ;
        char* pos2[2] ;
        struct Node* node;
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
                    DEBUG_PRINT("New Formula");
                    node->formula = formula;
                }else{
                    // Add value cell
                    DEBUG_PRINT("New Val");
                    node->val = attr[2];
                }
                printf("CELL %s%s Val=%s Formula=%s\n", pos[0], pos[1], node->val, node->formula);
                update_formula_node(sheet->tree,sheet->tree->root);
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
            case 4:
                //CTRL+X = CANCEL
                printf("Exit Program...");
                exit(1);
                break;
            default:
                printf("Wrong command!\n");
                break;
        }
        
        // Clean variable
        pos[2] = NULL ;
        pos2[2] = NULL ;
        node = NULL;
    }
}
char *trimwhitespace(char *str)
{
    char *end;
    
    // Trim leading space
    while(isspace(*str)) str++;
    
    if(*str == 0)  // All spaces?
        return str;
    
    // Trim trailing space
    end = str + strlen(str) - 1;
    while(end > str && isspace(*end)) end--;
    
    // Write new null terminator
    *(end+1) = 0;
    
    return str;
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

