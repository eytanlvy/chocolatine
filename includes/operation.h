#ifndef _OPERATION_H_
#define _OPERATION_H_

#include "somewhat.h"
#include "modulus.h"

void xor(fmpz_t res, fmpz_t a, fmpz_t b, PublicKey *pk);
void and(fmpz_t res, fmpz_t a, fmpz_t b, PublicKey *pk);

#endif