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

void sw_encrypt_bit(fmpz_t c, int bit, int non_zero, PublicKey *pk){
    double q;
    fmpz_poly_t u;

    fmpz_poly_init(u);
    q = (pk->n - (double)non_zero) / pk->n;

    gen_noise_vector(u, pk->n, q);

    fmpz_poly_evaluate_horner_fmpz(c, u, pk->r);
    fmpz_mul_ui(c, c, 2);
    fmpz_add_ui(c, c, bit);

    normalize_mod(c, c, pk->d);

    fmpz_poly_clear(u);
}

int sw_decrypt_bit(fmpz_t c, KeyPair *key){
    fmpz_t temp, mod_result;
    fmpz_init(temp);
    fmpz_init(mod_result);

    fmpz_mul(temp, c, key->sk.w);

    normalize_mod(temp, temp, key->pk.d);

    fmpz_mod_ui(mod_result, temp, 2);
    int result = fmpz_get_ui(mod_result);

    fmpz_clear(temp);
    fmpz_clear(mod_result);

    return result;
}

void sw_encrypt_int(fmpz_t c, const fmpz_t m, const fmpz_t p, int non_zero, PublicKey *pk) {
    double q;
    fmpz_poly_t u;

    fmpz_poly_init(u);
    q = (pk->n - (double)non_zero) / pk->n;

    gen_noise_vector(u, pk->n, q);

    fmpz_poly_evaluate_horner_fmpz(c, u, pk->r);
    fmpz_mul(c, c, p);
    fmpz_add(c, c, m);
    normalize_mod(c, c, pk->d);

    fmpz_poly_clear(u);
}

void sw_decrypt_int(fmpz_t res, const fmpz_t c, const fmpz_t p, KeyPair *key) {
    fmpz_t temp, mod_result;
    fmpz_init(temp);
    fmpz_init(mod_result);

    fmpz_mul(temp, c, key->sk.w);

    normalize_mod(temp, temp, key->pk.d);

    fmpz_mod(mod_result, temp, p);
    
    fmpz_set(res, mod_result);

    fmpz_clear(temp);
    fmpz_clear(mod_result);
}