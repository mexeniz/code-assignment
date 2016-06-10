//
//  spreadsheet.c
//  spreadsheet
//
//  Created by pawissakan-throughwave on 6/8/2559 BE.
//  Copyright © 2559 Throughwave. All rights reserved.
//

#include "spreadsheet.h"


struct Spreadsheet* spreadsheet_init(char* name,int row,int col){
    struct Spreadsheet* spreadsheet = (struct Spreadsheet* ) malloc(sizeof(struct Spreadsheet));
    spreadsheet->tree = avltree_init();
    spreadsheet->name = name;
    spreadsheet->row = 0;
    spreadsheet->col = 0;
    spreadsheet->n_cell = 0;

    return spreadsheet;
}
char* sum_list(struct Spreadsheet* sheet, int list_len, char* pos_list[list_len]){
    int result = 0;
    int i;
    for(i=0; i < list_len; i++){
        // interpret_pos2(pos_int,pos_list[i]);

        // printf("REAL x=%d y=%d",pos_int[0],pos_int[1]);
        //printf("SPLIT %s len=%d\n",pos_list[i],strlen(pos_list[i]) );
        // START SPLIT
        char* x = (char*)malloc(sizeof(int)*20);
        char* y = (char*)malloc(sizeof(int)*20);
        int sin = 0;
        for (sin = 0 ; sin < strlen(pos_list[i]) ; sin++){
            if (pos_list[i][sin] <= 90 && pos_list[i][sin] >= 64){
                // X Position
                int len = (int) strlen(x) ;
                if(len==0){
                    x[len] = pos_list[i][sin];
                    x[len+1] = '\0' ;
                }else if(len==1){
                    x[len] = pos_list[i][sin];
                    x[len+1] = '\0' ;
                }
            }else if (pos_list[i][sin] <= 57 && pos_list[i][sin] >= 48){
                // Y Position
                int len = (int) strlen(y) ;
                y[len] = pos_list[i][sin];
                y[len+1] = '\0';
            }
        }
        // printf("Interpret x=%s y=%s\n");
        // START INTERPRET
        trimwhitespace(x);
        trimwhitespace(y);
        i = -1;
        if(strlen(x) > 2 || strlen(x) < 1){
            printf("Wrong first position!\n");
            return ;
        }
        else if(strlen(x)== 1){
            i = x[0]-65;
        }
        else if(strlen(x)== 2){
            i = x[0]*26+x[1]-65;
        }
        int j = atoi(y)-1;

        free(x);
        free(y);

        printf("i=%d j=%d",i,j);
        struct Node *node = get_node_i(sheet->tree, i , j);

        if(node != NULL){
            printf("Cell %s Val=%s\n",pos_list[i],node->val);
            result += atoi(node->val);
            }else{
            printf("Cell %s\n invalid\n",pos_list[i]);
            }

        //free(pos_int);
    }

    char* buffer = malloc(sizeof(char)*20);
    sprintf(buffer, "%d", result);

    return buffer;
}
char* sum(struct Spreadsheet* sheet,char* x1, char* y1, char* x2, char* y2){
    int result = 0 ;

    // Interpret position fron char* to integer coordinate
    int* pos_int1 = (int *)malloc(sizeof(int)*2);
    int* pos_int2 = (int *)malloc(sizeof(int)*2);

    interpret_pos(pos_int1,x1,y1);
    interpret_pos(pos_int2,x2,y2);

    // i1&j1 must be min
    // i2&j2 must be max
    int i1 = pos_int1[0];
    int j1 = pos_int1[1];

    int i2 = pos_int2[0];
    int j2 = pos_int2[1];

    if (i1 > i2){
        int tmp = i1;
        i1 = i2;
        i2 = tmp;
    }
    if (j1 > j2){
        int tmp = j1;
        j1 = j2;
        j2 = tmp;
    }

    int a ,b ;
    for (a = i1; a <= i2 ;a++){
        for(b = j1; b <= j2 ;b++ ){
            struct Node *node = get_node_i(sheet->tree, a , b);
            if(node != NULL) result += atoi(node->val);
        }

    }
    char* buffer = malloc(sizeof(char)*20);
    sprintf(buffer, "%d", result);

    return buffer;
}
void print_sheet(struct Spreadsheet *sheet){
    printf("*****List all cell******\n");
    print_tree(sheet->tree);
    printf("*****End of sheet*******\n");
    }
void interpret_pos2(int** pos_int, char* cord){
    char** split = (char *) malloc(sizeof(char)*24);
    split_pos(split,cord);
    interpret_pos(pos_int,split[0],split[1]);
    printf("IP2 x=%d y=%d\n",pos_int[0],pos_int[1]);
    free(split);
}
void interpret_pos(int** pos_int, char* x, char* y){
    trimwhitespace(x);
    trimwhitespace(y);

    int* i = (int* ) malloc(sizeof(int));
    int* j = (int* ) malloc(sizeof(int));

    i = -1;
    if(strlen(x) > 2 || strlen(x) < 1){
        printf("Wrong first position!\n");
        return ;
    }
    else if(strlen(x)== 1){
        i = x[0]-65;
    }
    else if(strlen(x)== 2){
        i = x[0]*26+x[1]-65;
    }
    j = atoi(y)-1;

    pos_int[0] = i;
    pos_int[1] = j;
}
// This trim function is from Stackoverflow
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
