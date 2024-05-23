#ifndef __SOMEWHAT_H__
#define __SOMEWHAT_H__

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "polynomial.h"
#include "keygen.h"

int *gen_noise_vector(int n, double q);
int normalize_mod(int value, int modulus);
int encrypt_bit(int n,int q, int bit, PublicKey* public_key);
int decrypt_bit(int cypher, KeyPair* keypair);

#endif