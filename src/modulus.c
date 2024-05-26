#include "../includes/modulus.h"

void normalize_mod(fmpz_t result, const fmpz_t value, const fmpz_t modulus) {
    fmpz_t half_modulus, negative_half_modulus;
    fmpz_init(half_modulus);
    fmpz_init(negative_half_modulus);

    fmpz_fdiv_q_2exp(half_modulus, modulus, 1);
    fmpz_neg(negative_half_modulus, half_modulus);

    // result = value % modulus
    fmpz_mod(result, value, modulus);

    if (fmpz_cmp(result, half_modulus) >= 0) {
        fmpz_sub(result, result, modulus);
    } else if (fmpz_cmp(result, negative_half_modulus) < 0) {
        fmpz_add(result, result, modulus);
    }

    fmpz_clear(half_modulus);
    fmpz_clear(negative_half_modulus);
}