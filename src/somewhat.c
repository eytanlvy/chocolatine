#include "../includes/somewhat.h"

void gen_noise_vector(fmpz *noise, int n, double q) {
    for (int i = 0; i < n; i++) {
        fmpz_init(noise + i);
    }

    srand(time(NULL));

    for (int i = 0; i < n; i++) {
        double rand_val = (double)rand() / RAND_MAX;
        if (rand_val < q) {
            fmpz_set_si(noise + i, 0);
        } else if (rand_val < q + (1 - q) / 2) {
            fmpz_set_si(noise + i, -1);
        } else {
            fmpz_set_si(noise + i, 1);
        }
    }
}

void sw_encrypt_bit(int bit, int q, PublicKey *pk){
    
}

int sw_decrypt_bit(int n, SecretKey *sk){ return 0;}