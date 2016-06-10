//
//  main.c
//  Matrix
//
//  Created by pawissakan-throughwave on 6/7/2559 BE.
//  Copyright © 2559 Throughwave. All rights reserved.
//

#include <stdio.h>
#include "matrix.h"

//Test Case
//3 5
//1 2 3 4 5
//6 7 8 9 10
//11 12 13 14 15

int main(int argc, const char * argv[]) {
    printf("Matrix RxC\n");
    int row,col ;
    scanf("%d",&row);
    scanf("%d",&col);
    
    struct Matrix mat = matrix(&row,&col);
    int i,j;
    for (i=0 ; i < row ; i++){
        for (j = 0; j < col; j++){
            int x;
            scanf("%d",&x);
            set_val(&mat, &x, i, j);
        }
    }
    print_matrix(&mat);
    printf("Command\n0:print\n1:transpose\n2:rotate c\n3:rotate cw\n");
    while(1){
        int x;
        scanf("%d",&x);
        switch (x) {
            case 0:
                print_matrix(&mat);
                break;
            case 1:
                transpose(&mat);
                break;
            case 2:
                rotate_c(&mat);
                break;
            case 3:
                rotate_cw(&mat);
                break;
                
            default:
                printf("Wrong command!");
                break;
        }
    }
    return 0;
}
