#include "../includes/keygen.h"
#include <stdio.h>

int* gen_random_vector(int n){
    srand(time(NULL));
    int* randVector = malloc(n*sizeof(int));
    for(int i = 0; i < n; i++){
        randVector[i] = rand();
    }
    return randVector;
}

int** gen_matrix_from_vector(int* vector, int n){
    int** matrix = malloc(n*sizeof(int*));
    for(int i = 0; i < n; i++){
        matrix[i] = malloc(n*sizeof(int));
        for(int j = 0; j < n; j++){
            if(j - i < 0){
                matrix[i][j] = -vector[j-i+n];
            }
            else{
                matrix[i][j] = vector[j-i];
            }
        }
    }
    return matrix;
}

bool is_odd(int num) {
    return (num & 1);
}

int key_is_valid(int *v, int n){
    int **m = gen_matrix_from_vector(v, n);
    int det = determinant(m, n);

    for (int i = 0; i < n; i++) {
        free(m[i]);
    }
    free(m);

    return is_odd(det);
}

int gen_keys(int n){
    int *vec = gen_random_vector(n);
    Polynomial *v = polynomial_from_vector(vec, n);
    int **v_matrix = gen_matrix_from_vector(vec, n);

    return 0;
}