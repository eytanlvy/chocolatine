#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include "../includes/utils.h"

void key_pair_to_file(const KeyPair *key_pair, const char *filename) {
	FILE *fout = fopen(filename, "w");
	if (fout == NULL) {
		perror("Erreur lors de l'ouverture du fichier");
		exit(1);
	}
	char *hex_str;

    fprintf(fout, "sk->w_");
    hex_str = fmpz_get_str(NULL, 16, key_pair->sk.w);
    fprintf(fout, "%s\n", hex_str);
    free(hex_str);

    fprintf(fout, "pk->d_");
    hex_str = fmpz_get_str(NULL, 16, key_pair->pk.d);
    fprintf(fout, "%s\n", hex_str);
    free(hex_str);

    fprintf(fout, "pk->r_");
    hex_str = fmpz_get_str(NULL, 16, key_pair->pk.r);
    fprintf(fout, "%s\n", hex_str);
    free(hex_str);

    fclose(fout);
}

int main(int argc, char **argv) {
    int n = (argc > 1) ? atoi(argv[1]) : 128;
    int t = (argc > 2) ? atoi(argv[2]) : 128;

    clock_t start_time = clock();

    KeyPair *key_pair = gen_key_pair(n, t);

    clock_t end_time = clock();
    double elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;

    printf("Key generation achieved in %f seconds\n", elapsed_time);

    key_pair_to_file(key_pair, "key_pair.txt");

    int non_zero = 15;
    fmpz_t c;
	fmpz_init(c);

    int success_count = 0;
    int num_tests = 10;
	int *bit = malloc(sizeof(int) * num_tests);
	for (int i = 0; i < num_tests; i++) {
		bit[i] = i & 1;
	}

    for (int i = 0; i < num_tests; i++) {
        sw_encrypt_bit(c, bit[i], non_zero, &key_pair->pk);

        int res = sw_decrypt_bit(c, key_pair);

        if (res == bit[i]) {
            success_count++;
		}
    }

    printf("Number of successful decryptions: %d out of %d\n", success_count, num_tests);

    clear_key_pair(key_pair);
	fmpz_clear(c);
	free(bit);
    return 0;
}