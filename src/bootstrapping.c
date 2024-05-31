#include "../includes/bootstrapping.h"
#include "../includes/somewhat.h"
#include "../includes/modulus.h"

void fh_encrypt(fmpz_t c, fmpz_t bit, int non_zero, FhPublicKey *pk){
	double q;
    fmpz_poly_t u;

    fmpz_poly_init(u);
    q = (pk->n - (double)non_zero) / pk->n;

    gen_noise_vector(u, pk->n, q);

    fmpz_poly_evaluate_horner_fmpz(c, u, pk->r);
    fmpz_mul(c, c, pk->p);
    fmpz_add(c, c, bit);

    normalize_mod(c, c, pk->d);

    fmpz_poly_clear(u);
}

void fh_decrypt(fmpz_t res, fmpz_t c, FhKeyPair *key){} //Bootstrapping