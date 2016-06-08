//
//  matrix.c
//  Matrix
//
//  Created by pawissakan-throughwave on 6/7/2559 BE.
//  Copyright © 2559 Throughwave. All rights reserved.
//

#include "matrix.h"
#include "debug.h"
#include <stdlib.h>
#include <stdio.h>

struct Matrix matrix(int* row, int* col){
    struct Matrix mat;
    mat.row = *row;;
    mat.col = *col;
    mat.ptr = malloc(sizeof(int)*(*row)*(*col));
    return mat;
}
int* get_val(struct Matrix* mat, int i, int j){
    return &mat->ptr[(i*mat->row)+j];
}
void set_val(struct Matrix* mat, int* val, int i, int j){
    int* val_ptr = &mat->ptr[(i*mat->row)+j];
    *val_ptr = *val ;
    DEBUG_PRINT("i=%d j=%d ptr=%p val=%d\n", i,j, val_ptr,*val_ptr);
    
}
void print_matrix(struct Matrix* mat){
    int i,j;
    DEBUG_PRINT("****MAT PRINT****\n");
    DEBUG_PRINT("mat.ptr=%p\n", mat->ptr);
    printf("****Matrix %dx%d****\n",mat->row,mat->col);
    for (i=0 ; i < mat->row ; i++){
        for (j = 0; j < mat->col; j++){
            int* x = get_val(mat, i, j);
            printf("%d\t",*x);
        }
        printf("\n");
    }
    
}
void transpose(struct Matrix* mat){
    struct Matrix trans = matrix( &mat->col,&mat->row);

    int i,j;
    for (i=0 ; i < mat->col ; i++){
        for (j = 0; j < mat->row; j++){
            int tmp = *get_val(mat, j, i);
            set_val(&trans, &tmp, i, j);
        }
    }
    free(mat->ptr);
    mat->ptr = trans.ptr;
    mat->col = trans.col;
    mat->row = trans.row;
    
}
void swap(struct Matrix* mat, int i, int j, int i2, int j2){
    int tmp = *get_val(mat, i, j);
    int tmp2 = *get_val(mat, i2, j2);
    set_val(mat, &tmp2, i, j);
    set_val(mat, &tmp, i2, j2);
}
void reverse_row(struct Matrix* mat){
    int i,j;
    for (i=0 ; i < mat->row ; i++){
        for (j = 0; j < mat->col/2; j++){
            swap(mat, i, j, i, mat->col-j-1);
        }
    }
}
void reverse_col(struct Matrix* mat){
    int i,j;
    for (j=0 ; j < mat->col ; j++){
        for (i = 0; i < mat->row/2; i++){
            swap(mat, i, j, mat->row-j-1,j );
        }
    }
}
void rotate_c(struct Matrix* mat){
    transpose(mat);
    reverse_row(mat);
}
void rotate_cw(struct Matrix* mat){
    transpose(mat);
    reverse_col(mat);
}