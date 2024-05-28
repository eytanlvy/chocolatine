#include "../includes/fh_keygen.h"

const int S = 1024;
const int s = 15;
fmpz_t R;

void init_constants(){
	fmpz_init(R);
    fmpz_set_ui(R, 1);
	fmpz_mul_2exp(R, R, 51);
}

void generate_indices_and_values(const fmpz_t w, int s, int S, const fmpz_t d, fmpz_t *x, fmpz_t *i_k) {
    flint_rand_t state;
    flint_randinit(state);
    fmpz_t sum, temp, temp2, inv_temp2;
    fmpz_init(sum);
    fmpz_init(temp);
    fmpz_init(temp2);
    fmpz_init(inv_temp2);

    for (int k = 0; k < s - 1; k++) {
        int i = rand() % S;
        fmpz_set_ui(i_k[k], i);
        fmpz_randm(x[k], state, d);
        fmpz_pow_ui(temp, R, i);
        fmpz_mul(temp, temp, x[k]);
        fmpz_mod(temp, temp, d);
        fmpz_add(sum, sum, temp);
        fmpz_mod(sum, sum, d);
    }

    int found_inverse = 0;
    while (!found_inverse) {
        int last_index = rand() % S;
        fmpz_set_ui(i_k[s - 1], last_index);
        fmpz_sub(temp, w, sum);  // temp = w - sum
        fmpz_mod(temp, temp, d);
        fmpz_pow_ui(temp2, R, last_index);

        if (fmpz_invmod(inv_temp2, temp2, d) != 0) {
            found_inverse = 1;
            fmpz_mul(x[s - 1], temp, inv_temp2);
            fmpz_mod(x[s-1], x[s-1], d);
        }
    }

    flint_randclear(state);
    fmpz_clear(sum);
    fmpz_clear(temp);
    fmpz_clear(temp2);
    fmpz_clear(inv_temp2);
}

void generate_data(KeyPair *key_pair, int s, int S, fmpz_t *x, fmpz_t *i_k, int **sigma_k) {
    const fmpz_t *w = &(key_pair->sk.w);
    const fmpz_t *d = &(key_pair->pk.d);

    generate_indices_and_values(*w, s, S, *d, x, i_k);

    for (int k = 0; k < s; k++) {
        for (int i = 0; i < S; i++) {
            if (i == fmpz_get_ui(i_k[k])) {
                sigma_k[k][i] = 1;
            } else {
                sigma_k[k][i] = 0;
            }
        }
    }
}