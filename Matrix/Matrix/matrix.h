//
//  matrix.h
//  Matrix
//
//  Created by pawissakan-throughwave on 6/7/2559 BE.
//  Copyright © 2559 Throughwave. All rights reserved.
//

#ifndef matrix_h
#define matrix_h

#include <stdio.h>
struct Matrix{
    int *ptr;
    int row ;
    int col ;
};

struct Matrix matrix(int* row, int* col );
int* get_val(struct Matrix* mat, int i, int j);
void set_val(struct Matrix* mat, int* val, int i, int j);
void print_matrix(struct Matrix* mat);
void transpose(struct Matrix* mat);
void swap(struct Matrix* mat, int i, int j, int i2, int j2);
void reverse_row(struct Matrix* mat);
void reverse_col(struct Matrix* mat);
void rotate_c(struct Matrix* mat);
void rotate_cw(struct Matrix* mat);

#endif /* matrix_h */
