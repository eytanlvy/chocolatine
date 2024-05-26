#ifndef _UTILS_H_
#define _UTILS_H_

#include "somewhat.h"

void print_poly(const fmpz_poly_t poly);
void print_matrix(const fmpz_mat_t mat);
void print_key_pair(const KeyPair *key_pair, int hexa);

#endif