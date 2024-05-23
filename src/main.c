#include "../includes/polynomial.h"
#include "../includes/keygen.h"
#include "../includes/somewhat.h"
#include "../includes/matrix.h"
#include <stdio.h>

void print_polynomial(Polynomial* p){
	size_t deg = polynomial_get_degree(p);
	int coefi = polynomial_get_coefficient(p,deg);
	if(!deg){
		printf("%d",coefi);
		return;
	}
	if(coefi == 1){
		if(deg == 1){
			printf("x");
		}
		else{
			printf("x^%ld",deg);
		}
	}
	else if(coefi == -1){
		if(deg == 1){
			printf("-x");
		}
		else{
			printf("x^%ld",deg);
		}
	}
	else{
		if(deg == 1){
			printf("%dx",coefi);
		}
		else{
			printf("%dx^%ld",coefi,deg);
		}

	}
	for(int i = deg - 1; i >= 0; i--){
		coefi = polynomial_get_coefficient(p,i);
		if(coefi == 0){
			continue;
		}
		if(coefi > 0){
			if(coefi == 1 && i){
				printf(" + ");
			}
			else{
				printf(" + %i",coefi);
			}
		}
		else{
			if(coefi == 1 && i){
				printf(" - ");
			}
			else{
				printf(" - %i",-(coefi));
			}
		}
		if(i > 1){
			printf("x^%i",i);
		}
		else if (i == 1){
			printf("x");
		}
	}
}

int main() {
	// Polynomial* a = polynomial_new (1);
	// polynomial_set_coefficient(a, 1, 1);
	
	// 	print_polynomial(a);
	// printf("\n");
	// polynomial_mod(a,2);

	// print_polynomial(a);
	// printf("\n");

	// polynomial_destroy (&a);

	//Test determinant()
	int n = 3;
	int **matrix = malloc(n*sizeof(int*));
	for(int i = 0; i < n; i++){
		matrix[i] = malloc(n*sizeof(int));
		for(int j = 0; j < n; j++){
			if (i == j){
				matrix[i][j] = 2;
			}
			else{
				matrix[i][j] = 0;
			}
		}
	}
	print_matrix(matrix,n);
	printf("Determinant: %i\n",determinant(matrix,n));
	for(int i = 0; i < n; i++){
		free(matrix[i]);
	}
	free(matrix);
	return 0;
}