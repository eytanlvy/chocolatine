#include "../includes/operation.h"

void xor(fmpz_t res, fmpz_t a, fmpz_t b, PublicKey *pk){
	fmpz_add(res, a, b);
	normalize_mod(res, res, pk->d);
}

void and(fmpz_t res, fmpz_t a, fmpz_t b, PublicKey *pk){
	fmpz_mul(res, a, b);
	normalize_mod(res, res, pk->d);
}