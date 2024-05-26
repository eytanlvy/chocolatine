#include "../includes/keygen.h"
#include "../includes/modulus.h"

void init_key_pair(KeyPair *key_pair) {
    fmpz_init(key_pair->sk.w);
    fmpz_init(key_pair->pk.d);
    fmpz_init(key_pair->pk.r);
}

void clear_key_pair(KeyPair *key_pair) {
    fmpz_clear(key_pair->sk.w);
    fmpz_clear(key_pair->pk.d);
    fmpz_clear(key_pair->pk.r);
    free(key_pair);
}

void gen_random_polynomial(fmpz_poly_t v, int n, int t) {
    flint_rand_t state;
    flint_randinit(state);

    ulong seed1 = time(NULL);
    ulong seed2 = time(NULL) + 1;
    flint_randseed(state, seed1, seed2);
    fmpz_poly_randtest(v, state, n, t);
    flint_randclear(state);
}

void build_rotation_matrix(fmpz_mat_t mat, const fmpz_poly_t v, slong n) {
    fmpz_t coeff, neg_coeff;
    fmpz_init(coeff);
    fmpz_init(neg_coeff);

    for (slong i = 0; i < n; i++) {
        for (slong j = 0; j < n; j++) {
            slong index = (j - i);
            if (index < 0) {
                index += n;
                fmpz_poly_get_coeff_fmpz(coeff, v, index);
                fmpz_neg(neg_coeff, coeff);
                fmpz_set(fmpz_mat_entry(mat, i, j), neg_coeff);
            } else {
                fmpz_poly_get_coeff_fmpz(coeff, v, index);
                fmpz_set(fmpz_mat_entry(mat, i, j), coeff);
            }
        }
    }

    fmpz_clear(coeff);
    fmpz_clear(neg_coeff);
}

void compute_rotation_determinant(fmpz_t det, const fmpz_poly_t v, slong n) {
    fmpz_mat_t mat;
    fmpz_mat_init(mat, n, n);

    build_rotation_matrix(mat, v, n);

    fmpz_mat_det(det, mat);

    fmpz_mat_clear(mat);
}
void compute_scaled_inverse(fmpz_poly_t w, fmpz_t d, const fmpz_poly_t v, slong n) {
    fmpz_poly_t fn, s, t;
    fmpz_poly_init(fn);
    fmpz_poly_init(s);
    fmpz_poly_init(t);

    fmpz_poly_set_coeff_ui(fn, n, 1);
    fmpz_poly_set_coeff_ui(fn, 0, 1);

    fmpz_poly_xgcd(d, s, t, fn, v);

    fmpz_poly_set(w, s);

    fmpz_poly_clear(fn);
    fmpz_poly_clear(s);
    fmpz_poly_clear(t);
}

slong find_odd_coefficient_index(const fmpz_poly_t w) {
    slong len = fmpz_poly_length(w);
    for (slong i = 0; i < len; i++) {
        if (fmpz_is_odd(fmpz_poly_get_coeff_ptr(w, i))) {
            return i;
        }
    }
    return -1;
}


void calculate_r_and_verify(fmpz_t r, const fmpz_poly_t w, fmpz_t d, slong n) {
    fmpz_t w0, w1, w1_inv, r_pow_n, neg_one;
    fmpz_init(w0);
    fmpz_init(w1);
    fmpz_init(w1_inv);
    fmpz_init(r_pow_n);
    fmpz_init(neg_one);
    fmpz_set_si(neg_one, -1);

    fmpz_poly_get_coeff_fmpz(w0, w, 0);
    fmpz_poly_get_coeff_fmpz(w1, w, 1);

    // Compute r = w0 * inv(w1) mod d
    if (fmpz_invmod(w1_inv, w1, d)) {
        fmpz_mul(r, w0, w1_inv);
        fmpz_mod(r, r, d);

        // Verify that r^n ≡ -1 (mod d)
        fmpz_powm_ui(r_pow_n, r, n, d);
        normalize_mod(r_pow_n, r_pow_n, d);
        if (fmpz_equal(r_pow_n, neg_one)) {
            fmpz_clear(w0);
            fmpz_clear(w1);
            fmpz_clear(w1_inv);
            fmpz_clear(r_pow_n);
            fmpz_clear(neg_one);
            return;
        }
    }

    fmpz_set_ui(r, 0); // Set r to 0 if any condition fails

    fmpz_clear(w0);
    fmpz_clear(w1);
    fmpz_clear(w1_inv);
    fmpz_clear(r_pow_n);
    fmpz_clear(neg_one);
}

KeyPair *gen_key_pair(int n, int t) {
    KeyPair *key_pair;
    fmpz_poly_t v, w;
    fmpz_t d, r, w_odd;
    slong w_odd_index;

    key_pair = (KeyPair *)malloc(sizeof(KeyPair));
    if (key_pair == NULL) {
        fprintf(stderr, "Error: Unable to allocate memory for key pair\n");
        exit(1);
    }
    init_key_pair(key_pair);

    fmpz_init(d);
    fmpz_init(r);
    fmpz_init(w_odd);
    fmpz_poly_init(v);
    fmpz_poly_init(w);

    do {
        fmpz_set_ui(r, 0);
        gen_random_polynomial(v, n, t);
        compute_scaled_inverse(w, d, v, n);
        w_odd_index = find_odd_coefficient_index(w);
        if (w_odd_index != -1 && fmpz_is_odd(d)) {
            calculate_r_and_verify(r, w, d, n);
        }
    } while (fmpz_is_zero(r));

    fmpz_set(w_odd, fmpz_poly_get_coeff_ptr(w, w_odd_index));

    fmpz_set(key_pair->sk.w, w_odd);
    fmpz_set(key_pair->pk.d, d);
    fmpz_set(key_pair->pk.r, r);
    key_pair->pk.n = n;

    fmpz_poly_clear(v);
    fmpz_poly_clear(w);
    fmpz_clear(d);
    fmpz_clear(r);
    fmpz_clear(w_odd);

    return key_pair;
}
