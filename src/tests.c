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

void test_somewhat_int_encrypt(int n, int t, int p) {
    clock_t start_time = clock();

    KeyPair *key_pair = gen_key_pair(n, t, p);

    clock_t end_time = clock();
    double elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;

    printf("Key generation achieved in %f seconds\n", elapsed_time);

	key_pair_to_file(key_pair, "key_pair.txt");

    int non_zero = 15;

    fmpz_t c, res;
    fmpz_init(c);
    fmpz_init(res);

    int success_count = 0;
    int num_tests = 10;
    fmpz_t *bit = malloc(sizeof(fmpz_t) * num_tests);
    for (int i = 0; i < num_tests; i++) {
        fmpz_init(bit[i]);
    }

    // Generate random values for the test within the range [0, p-1]
    flint_rand_t state;
    flint_randinit(state);
    for (int i = 0; i < num_tests; i++) {
        fmpz_randm(bit[i], state, key_pair->pk.p);
    }
    flint_randclear(state);

    for (int i = 0; i < num_tests; i++) {
        sw_encrypt(c, bit[i], non_zero, &key_pair->pk);

        sw_decrypt(res, c, key_pair);

        if (fmpz_equal(res, bit[i])) {
            success_count++;
        }
        printf("Before encrypt: %ld, After: %ld\n", fmpz_get_ui(bit[i]), fmpz_get_ui(res));
    }

    printf("Number of successful decryptions: %d out of %d\n", success_count, num_tests);

    clear_key_pair(key_pair);
    fmpz_clear(c);
    fmpz_clear(res);
    for (int i = 0; i < num_tests; i++) {
        fmpz_clear(bit[i]);
    }
    free(bit);
}

void test_somewhat_int_op(int n, int t, int p) {
    clock_t start_time = clock();

    KeyPair *key_pair = gen_key_pair(n, t, p);

    clock_t end_time = clock();
    double elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;

    printf("Key generation achieved in %f seconds\n", elapsed_time);
    
    key_pair_to_file(key_pair, "key_pair.txt");

    int non_zero = 15;

    fmpz_t c_xor, c_and, res, p_fmpz;
    fmpz_init(c_xor);
    fmpz_init(c_and);
    fmpz_init(res);
    fmpz_init_set_ui(p_fmpz, p);

    int success_count_xor = 0;
    int success_count_and = 0;
    int total_tests = 0;
    int num_tests = 10;
    fmpz_t *bit = malloc(sizeof(fmpz_t) * num_tests);
    for (int i = 0; i < num_tests; i++) {
        fmpz_init(bit[i]);
    }
    fmpz_t *cypher_bit = malloc(sizeof(fmpz_t) * num_tests);
    for (int i = 0; i < num_tests; i++) {
        fmpz_init(cypher_bit[i]);
    }
    // Generate random values for the test within the range [0, p-1]
    flint_rand_t state;
    flint_randinit(state);
    for (int i = 0; i < num_tests; i++) {
        fmpz_randm(bit[i], state, p_fmpz);
        sw_encrypt(cypher_bit[i], bit[i], non_zero, &key_pair->pk);
    }
    flint_randclear(state);

    for (int i = 0; i < num_tests; i++) {
        for (int j = i + 1; j < num_tests; j++) {

            // Effectuer les opérations XOR et AND
            xor(c_xor, cypher_bit[i], cypher_bit[j], &key_pair->pk);
            and(c_and, cypher_bit[i], cypher_bit[j], &key_pair->pk);

            // Déchiffrer les résultats
            sw_decrypt(res, c_xor, key_pair);
            int decrypted_xor = fmpz_get_ui(res);
            int expected_xor = (fmpz_get_ui(bit[i]) + fmpz_get_ui(bit[j])) % p;

            sw_decrypt(res, c_and, key_pair);
            int decrypted_and = fmpz_get_ui(res);
            int expected_and = (fmpz_get_ui(bit[i]) * fmpz_get_ui(bit[j])) % p;

            // Vérifier les résultats
            if (decrypted_xor == expected_xor) {
                success_count_xor++;
            }
            if (decrypted_and == expected_and) {
                success_count_and++;
            }
            total_tests++;

            printf("XOR %d: Expected = %d, got= %d\n", total_tests, expected_xor, decrypted_xor);
            printf("AND %d: Expected = %d, got = %d\n", total_tests, expected_and, decrypted_and);
        }
    }

    printf("Total XOR tests succeeded: %d/%d\n", success_count_xor, total_tests);
    printf("Total AND tests succeeded: %d/%d\n", success_count_and, total_tests);

    clear_key_pair(key_pair);
    fmpz_clear(c_xor);
    fmpz_clear(c_and);
    fmpz_clear(res);
    fmpz_clear(p_fmpz);
    for (int i = 0; i < num_tests; i++) {
        fmpz_clear(bit[i]);
    }
    free(bit);
    free(cypher_bit);
}