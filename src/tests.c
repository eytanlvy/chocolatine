#include "../includes/tests.h"
#include "../includes/fh_keygen.h"

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

void test_somewhat_int_mul(int n, int t, int p) {
    clock_t start_time = clock();

    KeyPair *key_pair = gen_key_pair(n, t, p);

    clock_t end_time = clock();
    double elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;

    printf("Key generation achieved in %f seconds\n", elapsed_time);
    
    key_pair_to_file(key_pair, "key_pair.txt");

    int non_zero = 15;

    fmpz_t product, cypher_product, factor, cypher_factor, p_fmpz;
    fmpz_init(product);
    fmpz_init(cypher_product);
    fmpz_init(factor);
    fmpz_init_set_ui(p_fmpz, p);
    // Generate random value for res
    flint_rand_t state;
    flint_randinit(state);
    fmpz_randm(product, state, p_fmpz);
    fmpz_randm(factor, state, p_fmpz);
    flint_randclear(state);
    sw_encrypt(cypher_product, product, non_zero, &key_pair->pk);
    sw_encrypt(cypher_factor, factor, non_zero, &key_pair->pk);
    fmpz_clear(factor);
    int int_factor = fmpz_get_ui(factor);
    int nb_mult = -1;
    int expected_and, decrypted_and, temp;
    do{
        temp = fmpz_get_ui(product);
        and(cypher_product, cypher_product, cypher_factor, &key_pair->pk);

        sw_decrypt(product, cypher_product, key_pair);
        decrypted_and = fmpz_get_ui(product);
        expected_and = (temp * int_factor) % p;

        nb_mult++;
    }
    while(expected_and == decrypted_and);

    printf("nb_mult = %d\n",nb_mult);
    clear_key_pair(key_pair);
    fmpz_clear(cypher_factor);
    fmpz_clear(product);
    fmpz_clear(cypher_product);
    fmpz_clear(p_fmpz);
}

void test_fh_keygen(int n, int t, int p) {
    clock_t start_time = clock();

    KeyPair *key_pair = gen_key_pair(n, t, p);

    clock_t end_time = clock();
    double elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;

    printf("Key generation achieved in %f seconds\n", elapsed_time);
    
    key_pair_to_file(key_pair, "key_pair.txt");

    fmpz_t *x = malloc(s * sizeof(fmpz_t));
    fmpz_t *i_k = malloc(s * sizeof(fmpz_t));
    fmpz_t **sigma_k = malloc(s * sizeof(fmpz_t *));

    for (int k = 0; k < s; k++) {
        sigma_k[k] = malloc(S * sizeof(fmpz_t));
    }

    generate_data(key_pair, s, S, x, i_k, sigma_k);

    // Vérification de la somme
    fmpz_t sum, temp, R_cpy;
    fmpz_init(R_cpy);
    fmpz_init(sum);
    fmpz_init(temp);
    fmpz_zero(sum);
    fmpz_set(R_cpy, R);

    for (int k = 0; k < s; k++) {
        for (int i = 0; i < S; i++) {
            fmpz_mul(temp, sigma_k[k][i], x[k]);
		    fmpz_powm(R_cpy, R, i_k[k], key_pair->pk.d);
            fmpz_mul(temp, temp, R_cpy);
            fmpz_add(sum, sum, temp);
        }
    }
    fmpz_mod(sum, sum, key_pair->pk.d);

    fmpz_t w_mod_d;
    fmpz_init(w_mod_d);
    fmpz_mod(w_mod_d, key_pair->sk.w, key_pair->pk.d);

    if (fmpz_equal(sum, w_mod_d)) {
        printf("Verification successful: sum equals w mod d.\n");
    } else {
        printf("Verification failed: sum does not equal w mod d.\n");
    }

    fmpz_clear(sum);
    fmpz_clear(temp);
    fmpz_clear(w_mod_d);

    for (int k = 0; k < s; k++) {
        fmpz_clear(x[k]);
        fmpz_clear(i_k[k]);
        for (int i = 0; i < S; i++) {
            fmpz_clear(sigma_k[k][i]);
        }
        free(sigma_k[k]);
    }
    free(x);
    free(i_k);
    free(sigma_k);
    fmpz_clear(R);

    clear_key_pair(key_pair);
}