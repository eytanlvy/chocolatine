#include "../includes/compress_key.h"

int calculate_q(int S) {
    return (int)(ceil(sqrt(2 * S)) + 1);
}

void initialize_eta_vectors(fmpz_t **eta, int s, int q) {
    for (int k = 0; k < s; k++) {
        eta[k] = malloc(q * sizeof(fmpz_t));
        for (int i = 0; i < q; i++) {
            fmpz_init(eta[k][i]);
            fmpz_set_ui(eta[k][i], 0);
        }
    }
}

int ind(int a, int b, int q) {
    return (a - 1) * q - ((a * (a - 1)) / 2) + (b - a);
}

void find_a_b(int index, int q, int *a, int *b) {
    for (int i = 0; i < q; i++) {
		for(int j = i; j < q; j++) {
			if (ind(i,j,q) == index) {
				*a = i;
				*b = j;
				return;
			}
		}
	}
}

void set_eta_values(fmpz_t **eta, int s, int q, int **sigma_k){
	int a, b;
	for (int k = 0; k < s; k++) {
		for (int i = 0; i < S; i++) {
			if (sigma_k[k][i] == 1) {
				find_a_b(i, q, &a, &b);
				fmpz_set_ui(eta[k][a], 1);
				fmpz_set_ui(eta[k][b], 1);
				break;
			}
		}
	}
}
