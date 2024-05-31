#include "../includes/fh_keygen.h"
#include "../includes/compress_key.h"
#include "../includes/bootstrapping.h"

const int S = 1024;
const int s = 15;
int q;
fmpz_t R;

void init_constants(){
	fmpz_init(R);
    fmpz_set_ui(R, 1);
	fmpz_mul_2exp(R, R, 51);

    calculate_q(S);
}

void init_fh_key_pair(FhKeyPair *key_pair){
    fmpz_init(key_pair->pk.d);
    fmpz_init(key_pair->pk.r);
    fmpz_init(key_pair->pk.p);
    
    key_pair->pk.x = malloc(s * sizeof(fmpz_t));
    key_pair->pk.eta_hint = malloc(s * sizeof(fmpz_t*));
    key_pair->sk.eta = malloc(s * sizeof(fmpz_t*));

    for (int i = 0; i < s; i++){
        fmpz_init(key_pair->pk.x[i]);
        key_pair->pk.eta_hint[i] = malloc(q * sizeof(fmpz_t));
        key_pair->sk.eta[i] = malloc(q * sizeof(fmpz_t));
        for (int j = 0; j < q; j++){
            fmpz_init(key_pair->pk.eta_hint[i][j]);
            fmpz_init(key_pair->sk.eta[i][j]);
            fmpz_set_ui(key_pair->sk.eta[i][j], 0);
        }
    }
}

void clear_fh_key_pair(FhKeyPair *key_pair){
    fmpz_clear(key_pair->pk.d);
    fmpz_clear(key_pair->pk.r);
    fmpz_clear(key_pair->pk.p);
    for (int i = 0; i < s; i++){
        fmpz_clear(key_pair->pk.x[i]);
        for (int j = 0; j < q; j++){
            fmpz_clear(key_pair->sk.eta[i][j]);
            fmpz_clear(key_pair->pk.eta_hint[i][j]);
        }
        free(key_pair->pk.eta_hint[i]);
        free(key_pair->sk.eta[i]);
    }
    free(key_pair->sk.eta);
    free(key_pair->pk.eta_hint);
    free(key_pair->pk.x);

    free(key_pair);
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

void encrypt_sk(FhKeyPair *fh_key_pair) {
    int non_zero = 15;
    fh_key_pair->pk.eta_hint = malloc(s*sizeof(fmpz_t*));

    for (int k = 0; k < s; k++){
        (fh_key_pair->pk.eta_hint)[k] = malloc(q * sizeof(fmpz_t));
        for (int i = 0; i < q; i++){
            fmpz_init((fh_key_pair->pk.eta_hint)[k][i]);
            fh_encrypt((fh_key_pair->pk.eta_hint)[k][i], (fh_key_pair->sk.eta)[k][i], non_zero, &fh_key_pair->pk);
        }
    }
}

FhKeyPair *gen_fh_key_pair(int n, int t, int p) {
    init_constants();
    FhKeyPair *fh_key_pair;
    KeyPair *key_pair;
    
    key_pair = gen_key_pair(n, t, p);
    printf("Key pair generated\n");

    fh_key_pair = (FhKeyPair *)malloc(sizeof(FhKeyPair));
    init_fh_key_pair(fh_key_pair);

    fmpz_t *i_k = malloc(s * sizeof(fmpz_t));
    int **sigma_k = malloc(s * sizeof(int *));
    
    for (int k = 0; k < s; k++) {
        sigma_k[k] = malloc(S * sizeof(int));
    }

    generate_data(key_pair, s, S, fh_key_pair->pk.x, i_k, sigma_k);

    set_eta_values(fh_key_pair->sk.eta, s, q, sigma_k);

    fh_key_pair->pk.n = key_pair->pk.n;
    fmpz_set(fh_key_pair->pk.d, key_pair->pk.d);
    fmpz_set(fh_key_pair->pk.r, key_pair->pk.r);
    fmpz_set(fh_key_pair->pk.p, key_pair->pk.p);

    clear_key_pair(key_pair);
    free(i_k);
    for (int k = 0; k < s; k++) {
        free(sigma_k[k]);
    }
    free(sigma_k);
    encrypt_sk(fh_key_pair);

    return fh_key_pair;
}
