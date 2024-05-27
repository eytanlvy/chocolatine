#include "../includes/tests.h"

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

void test_somewhat_bit_encrypt(int n, int t) {
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
}

void test_somewhat_bit_op(int n, int t) {
    clock_t start_time = clock();
    KeyPair *key_pair = gen_key_pair(n, t);
    clock_t end_time = clock();
    double elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    printf("Key generation achieved in %f seconds\n", elapsed_time);
    key_pair_to_file(key_pair, "key_pair.txt");

    srand(time(NULL));
    int tableau[10];
    for (int i = 0; i < 10; i++) {
        tableau[i] = rand() % 2;
    }

    int non_zero = 15;
    fmpz_t c[10], c_xor, c_and;
    for (int i = 0; i < 10; i++) {
        fmpz_init(c[i]);
    }
    fmpz_init(c_xor);
    fmpz_init(c_and);

    int success_count_xor = 0;
    int success_count_and = 0;
    int total_tests = 0;

    for (int j = 0; j < 10; j++) {
        // Chiffrer les bits
        sw_encrypt_bit(c[j], tableau[j], non_zero, &(key_pair->pk));
    }

    for (int i = 0; i < 10; i++) {
        for (int j = i + 1; j < 10; j++) {
            // Appliquer XOR et AND
            xor(c_xor, c[i], c[j], &(key_pair->pk));
            and(c_and, c[i], c[j], &(key_pair->pk));

            // Déchiffrer les résultats
            int decrypted_xor = sw_decrypt_bit(c_xor, key_pair);
            int decrypted_and = sw_decrypt_bit(c_and, key_pair);

            if (decrypted_xor == (tableau[i] ^ tableau[j])) {
                success_count_xor++;
            }
            if (decrypted_and == (tableau[i] & tableau[j])) {
                success_count_and++;
            }
            total_tests++;
        }
    }

    printf("Total XOR tests succeeded: %d/%d\n", success_count_xor, total_tests);
    printf("Total AND tests succeeded: %d/%d\n", success_count_and, total_tests);

    for (int i = 0; i < 10; i++) {
        fmpz_clear(c[i]);
    }
    fmpz_clear(c_xor);
    fmpz_clear(c_and);

    clear_key_pair(key_pair);
}

void test_somewhat_int_encrypt(int n, int t) {
    clock_t start_time = clock();

    KeyPair *key_pair = gen_key_pair(n, t);

    clock_t end_time = clock();
    double elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;

    printf("Key generation achieved in %f seconds\n", elapsed_time);


    int non_zero = 15;
    fmpz_t p;
    fmpz_init_set_ui(p, 100);

    fmpz_t c, res;
    fmpz_init(c);
    fmpz_init(res);

    int success_count = 0;
    int num_tests = 10;
    fmpz_t *bit = malloc(sizeof(fmpz_t) * num_tests);
    for (int i = 0; i < num_tests; i++) {
        fmpz_init_set_ui(bit[i], i * 10);
    }

    for (int i = 0; i < num_tests; i++) {
        sw_encrypt_int(c, bit[i], p, non_zero, &key_pair->pk);

        sw_decrypt_int(res, c, p, key_pair);

        if (fmpz_equal(res, bit[i])) {
            success_count++;
        }
        printf("Before encrypt: %ld, After: %ld\n", fmpz_get_ui(bit[i]), fmpz_get_ui(res));
    }

    printf("Number of successful decryptions: %d out of %d\n", success_count, num_tests);

    clear_key_pair(key_pair);
    fmpz_clear(c);
    fmpz_clear(res);
    fmpz_clear(p);
    for (int i = 0; i < num_tests; i++) {
        fmpz_clear(bit[i]);
    }
    free(bit);
}