#ifndef _BOOTSTRAPPING_H_
#define _BOOTSTRAPPING_H_

#include "fh_keygen.h"

void fh_encrypt(fmpz_t c, fmpz_t bit, int non_zero, FhPublicKey *pk);
void fh_decrypt(fmpz_t res, fmpz_t c, FhKeyPair *key);

#endif