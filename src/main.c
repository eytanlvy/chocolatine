#include "../includes/includes.h"

void printVector(int* v, int n){
	printf("[%i",v[0]);
	for(int i = 1; i < n; i ++){
		printf(", %i",v[i]);
	}
	printf("]");
}
void printMatrix(int **m, int n){
	for(int i = 0; i < n; i ++){
		printVector(m[i],n);
		printf("\n");
	}
}

void printPolynomial(Polynomial* p){
	size_t deg = polynomial_get_degree(p);
	int coefi = floor(polynomial_get_coefficient(p,deg));
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
		coefi = floor(polynomial_get_coefficient(p,i));
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
	Polynomial* a = polynomial_new (1);
	polynomial_set_coefficient(a, 1, 1);
	
		printPolynomial(a);
	printf("\n");
	polynomial_mod(a,2);

	printPolynomial(a);
	printf("\n");

	polynomial_destroy (&a);
	return 0;
}