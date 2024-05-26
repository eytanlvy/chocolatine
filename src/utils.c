#include "utils.h"

void print_poly(const fmpz_poly_t poly) {
    slong len = fmpz_poly_length(poly);

    printf("[");
    for (slong i = 0; i < len; i++) {
        if (i > 0) {
            printf(", ");
        }
        fmpz_t coeff;
        fmpz_init(coeff);
        fmpz_poly_get_coeff_fmpz(coeff, poly, i);
        fmpz_print(coeff);
        fmpz_clear(coeff);
    }
    printf("]\n");
}

void print_matrix(const fmpz_mat_t mat) {
    slong rows = fmpz_mat_nrows(mat);
    slong cols = fmpz_mat_ncols(mat);

    for (slong i = 0; i < rows; i++) {
        for (slong j = 0; j < cols; j++) {
            fmpz_print(fmpz_mat_entry(mat, i, j));
            if (j < cols - 1) {
                printf(", ");
            }
        }
        printf("\n");
    }
}

void print_key_pair(const KeyPair *key_pair, int hexa) {
    if (hexa) {
        char *hex_str;

        printf("Secret key w: ");
        hex_str = fmpz_get_str(NULL, 16, key_pair->sk.w);
        printf("%s\n", hex_str);
        free(hex_str);

        printf("Public key d: ");
        hex_str = fmpz_get_str(NULL, 16, key_pair->pk.d);
        printf("%s\n", hex_str);
        free(hex_str);

        printf("Public key r: ");
        hex_str = fmpz_get_str(NULL, 16, key_pair->pk.r);
        printf("%s\n", hex_str);
        free(hex_str);
    } else {
        printf("Secret key w: ");
        fmpz_print(key_pair->sk.w);
        printf("\n");
        printf("Public key d: ");
        fmpz_print(key_pair->pk.d);
        printf("\n");
        printf("Public key r: ");
        fmpz_print(key_pair->pk.r);
        printf("\n");
    }
}
