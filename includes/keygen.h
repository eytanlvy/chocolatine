#ifndef _KEYGEN_H_
#define _KEYGEN_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <flint/flint.h>
#include <flint/fmpz.h>
#include <flint/fmpz_poly.h>
#include <flint/fmpz_vec.h>
#include <flint/fmpz_mat.h>


typedef struct SecretKey {
	fmpz_t w;
} SecretKey;

typedef struct PublicKey {
	fmpz_t d;
	fmpz_t r;
	fmpz_t p; //message space
	int n;
} PublicKey;

typedef struct KeyPair {
	SecretKey sk;
	PublicKey pk;
} KeyPair;


void init_key_pair(KeyPair *key_pair);
void clear_key_pair(KeyPair *key_pair);

void gen_random_polynomial(fmpz_poly_t v, int n, int t);
void compute_scaled_inverse(fmpz_poly_t w, fmpz_t d, const fmpz_poly_t v, slong n);

void build_rotation_matrix(fmpz_mat_t mat, const fmpz_poly_t v, slong n);
void compute_rotation_determinant(fmpz_t det, const fmpz_poly_t v, slong n);

slong find_odd_coefficient_index(const fmpz_poly_t w);
void calculate_r_and_verify(fmpz_t r, const fmpz_poly_t w, fmpz_t d, slong n);
int check_modulo_conditions(const fmpz_t d, const fmpz_t w, const fmpz_t p);
KeyPair *gen_key_pair(int n, int t, int p);

#endif