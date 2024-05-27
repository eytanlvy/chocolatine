#ifndef _SOMEWHAT_H_
#define _SOMEWHAT_H_

#include "keygen.h"

void gen_noise_vector(fmpz_poly_t noise, int n, double q);
void sw_encrypt_bit(fmpz_t c, int bit, int non_zero, PublicKey *pk);
int sw_decrypt_bit(fmpz_t c, KeyPair *key);

void sw_encrypt_int(fmpz_t c, int m, int p, int non_zero, PublicKey *pk);
int sw_decrypt_int(fmpz_t c, int p, KeyPair *key);

#endif