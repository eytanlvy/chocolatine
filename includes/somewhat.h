#include "keygen.h"

void gen_noise_vector(fmpz *noise, int n, double q);
void sw_encrypt_bit(int bit, int q, PublicKey *pk);
int sw_decrypt_bit(int n, SecretKey *sk);