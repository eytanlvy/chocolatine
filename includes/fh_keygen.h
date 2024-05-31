#ifndef _FH_KEYGEN_H_
#define _FH_KEYGEN_H_

#include "keygen.h"

extern const int S;
extern const int s;
extern int q;
extern fmpz_t R;

typedef struct FhSecretKey {
	fmpz_t **eta;
} FhSecretKey;

typedef struct FhPublicKey {
	fmpz_t d;
	fmpz_t r;
	fmpz_t p; //message space
	fmpz_t *x;
	fmpz_t **eta_hint;
	int n;
} FhPublicKey;

typedef struct FhKeyPair {
	FhSecretKey sk;
	FhPublicKey pk;
} FhKeyPair;

void init_constants();
void init_fh_key_pair(FhKeyPair *key_pair);
void clear_fh_key_pair(FhKeyPair *key_pair);

void generate_indices_and_values(const fmpz_t w, int s, int S, const fmpz_t d, fmpz_t *x, fmpz_t *i_k);
void generate_data(KeyPair *key_pair, int s, int S, fmpz_t *x, fmpz_t *i_k, int **sigma_k);

void encrypt_sk(FhKeyPair *fh_key_pair);
FhKeyPair *gen_fh_key_pair(int n, int t, int p);


#endif