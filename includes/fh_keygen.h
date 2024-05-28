#ifndef _FH_KEYGEN_H_
#define _FH_KEYGEN_H_

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
#include "keygen.h"

extern const int S;
extern const int s;
extern fmpz_t R;

void init_constants();
void generate_indices_and_values(const fmpz_t w, int s, int S, const fmpz_t d, fmpz_t *x, fmpz_t *i_k);
void generate_data(KeyPair *key_pair, int s, int S, fmpz_t *x, fmpz_t *i_k, fmpz_t **sigma_k);
#endif