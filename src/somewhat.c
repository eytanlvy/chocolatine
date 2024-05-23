#include "../includes/somewhat.h"

int *gen_noise_vector(int n, double q) {
    int *u = (int *)malloc(n * sizeof(int));
    if (u == NULL) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    srand(time(NULL));

    for (int i = 0; i < n; i++) {
        double rand_val = (double)rand() / RAND_MAX;
        if (rand_val < q) {
            u[i] = 0;
        } else if (rand_val < q + (1 - q) / 2) {
            u[i] = -1;
        } else {
            u[i] = 1;
        }
    }

    return u;
}

int normalize_mod(int value, int modulus) {
    int half_modulus = modulus >> 1;
    int result = value % modulus;

    if (result < -half_modulus) {
        result += modulus;
    } else if (result >= half_modulus) {
        result -= modulus;
    }

    return result;
}

int encrypt_bit(int n, int q, int bit, PublicKey* public_key){
	int *u = gen_noise_vector(n, q);
	int *a = malloc(n*sizeof(int));
	a[0] = 2*u[0] + bit;
	for (int i = 1; i < n; i++){
		a[i] = 2*u[i];
	}
	Polynomial *a_poly = polynomial_from_vector(a, n);
	int cypher = polynomial_evaluate(a_poly, public_key->r);
    cypher = normalize_mod(cypher, public_key->d);
    polynomial_destroy(&a_poly);
    free(u);
    free(a);
	return cypher;
}

int decrypt_bit(int cypher, KeyPair* keypair) {
    int cypher_mod = normalize_mod(cypher * keypair->private_key, keypair->public_key->d);
    return (cypher_mod & 1);
}

