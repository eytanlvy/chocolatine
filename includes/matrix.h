#ifndef MATRIX_H
#define MATRIX_H

#include <stdio.h>
#include <stdlib.h>

#define SQR(x) ((x)*(x))


void getMinor(int **matrix, int **minor, int n, int row, int col);
int determinant(int **matrix, int n);
void print_vector(int* v, int n);
void print_matrix(int **m, int n);


#endif