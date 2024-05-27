#include "../includes/somewhat.h"
#include "../includes/modulus.h"
#include "../includes/utils.h"

void gen_noise_vector(fmpz_poly_t noise, int n, double q) {
    srand(time(NULL));

    for (int i = 0; i < n; i++) {
        double rand_val = (double)rand() / RAND_MAX;
        if (rand_val < q) {
            fmpz_poly_set_coeff_si(noise, i, 0);
        } else if (rand_val < q + (1 - q) / 2) {
            fmpz_poly_set_coeff_si(noise, i, -1);
        } else {
            fmpz_poly_set_coeff_si(noise, i, 1);
        }
    }
}

void sw_encrypt(fmpz_t c, fmpz_t bit, int non_zero, PublicKey *pk){
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

void sw_decrypt(fmpz_t res, fmpz_t c, KeyPair *key){
    fmpz_t temp, mod_result;
    fmpz_init(temp);
    fmpz_init(mod_result);

    fmpz_mul(temp, c, key->sk.w);

    normalize_mod(temp, temp, key->pk.d);

    fmpz_mod(mod_result, temp, key->pk.p);
    fmpz_set(res, mod_result);

    fmpz_clear(temp);
    fmpz_clear(mod_result);
}