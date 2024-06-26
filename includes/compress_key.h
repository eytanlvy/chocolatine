#ifndef _COMPRESS_KEY_H_
#define _COMPRESS_KEY_H_

#include "utils.h"
#include <math.h>


void calculate_q(int S);
int ind(int a, int b, int q);
void find_a_b(int index, int q, int *a, int *b);
void set_eta_values(fmpz_t **eta, int s, int q, int **sigma_k);

#endif
