#ifndef __KEYGEN_H__
#define __KEYGEN_H__

#include <stdlib.h>
#include <time.h>
#include "polynomial.h"
#include "matrix.h"


typedef struct public_key_s{
	int r;
	int d;
} PublicKey;

typedef struct keypair_s{
	PublicKey *public_key;
	int private_key;
} KeyPair;

int* gen_random_vector(int size);
int** gen_matrix_from_vector(int* vector, int size);
bool is_odd(int num);
int key_is_valid(int *v, int n);
int gen_keys(int n);

#endif