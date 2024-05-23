#include "../includes/matrix.h"

void getMinor(int **matrix, int **minor, int n, int row, int col) {
    int minorRow = 0, minorCol = 0;
    for (int i = 0; i < n; i++) {
        if (i == row) continue;
        minorCol = 0;
        for (int j = 0; j < n; j++) {
            if (j == col) continue;
            minor[minorRow][minorCol] = matrix[i][j];
            minorCol++;
        }
        minorRow++;
    }
}

int determinant(int **matrix, int n) {
    if (n == 1) {
        return matrix[0][0];
    }
    
    if (n == 2) {
        return matrix[0][0] * matrix[1][1] - matrix[0][1] * matrix[1][0];
    }
    
    int det = 0;
    int **minor = (int **)malloc((n - 1) * sizeof(int *));
    for (int i = 0; i < n - 1; i++) {
        minor[i] = (int *)malloc((n - 1) * sizeof(int));
    }
    
    for (int i = 0; i < n; i++) {
        getMinor(matrix, minor, n, 0, i);
        int minorDet = determinant(minor, n - 1);
        det += (i % 2 == 0 ? 1 : -1) * matrix[0][i] * minorDet;
    }
    
    for (int i = 0; i < n - 1; i++) {
        free(minor[i]);
    }
    free(minor);
    
    return det;
}

void print_vector(int* v, int n){
	printf("[%i",v[0]);
	for(int i = 1; i < n; i ++){
		printf(", %i",v[i]);
	}
	printf("]");
}

void print_matrix(int **m, int n){
	for(int i = 0; i < n; i ++){
		print_vector(m[i],n);
		printf("\n");
	}
}