#include "../includes/keygen.h"
#include <stdio.h>

int* randomVector(int n){
    srand(time(NULL));
    int* randVector = malloc(n*sizeof(int));
    for(int i = 0; i < n; i++){
        randVector[i] = rand();
    }
    return randVector;
}

int** generateMatrixFromVector(int* vector, int n){
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