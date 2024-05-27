#ifndef _SOMEWHAT_H_
#define _SOMEWHAT_H_

#include "keygen.h"

void gen_noise_vector(fmpz_poly_t noise, int n, double q);
void sw_encrypt(fmpz_t c, fmpz_t bit, int non_zero, PublicKey *pk);
void sw_decrypt(fmpz_t res, fmpz_t c, KeyPair *key);

#endif