#include "../includes/tests.h"


int main(int argc, char **argv) {
    int n = (argc > 1) ? atoi(argv[1]) : 128;
    int t = (argc > 2) ? atoi(argv[2]) : 128;
	int p = (argc > 3) ? atoi(argv[3]) : 10;

    clock_t start_time, end_time;
    double elapsed_time;
    int non_zero = 15;
    KeyPair *key_pair = gen_key_pair(n, t, p);
    fmpz_t bit3, bit1;
    fmpz_init_set_ui(bit3,3);
    fmpz_init_set_ui(bit1,1);
    fmpz_t c3,c1;
    fmpz_init(c3);
    fmpz_init(c1);
    fmpz_t res;
    fmpz_init(res);

    start_time = clock();
        sw_encrypt(c3, bit3, non_zero, &key_pair->pk);
        sw_encrypt(c1, bit1, non_zero, &key_pair->pk);
    end_time = clock();
    elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    printf("n = %i, t = %i, encrypt : %f seconds\n",n,t, elapsed_time/2);


    start_time = clock();
        xor(res, bit3, bit1, &key_pair->pk);
    end_time = clock();
    elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    printf("n = %i, t = %i, xor : %f seconds\n",n,t, elapsed_time);

    start_time = clock();
        and(res, bit3, bit1, &key_pair->pk);
    end_time = clock();
    elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    printf("n = %i, t = %i, and : %f seconds\n",n,t, elapsed_time);

    int nb_mult = 0;
    int temp, decrypted_and, expected_and;
    fmpz_t decyphered;
    fmpz_init_set_ui(decyphered,3);
    do{
        temp = fmpz_get_ui(decyphered);
        and(res, res, c3, &key_pair->pk);

        sw_decrypt(decyphered, res, key_pair);
        decrypted_and = fmpz_get_ui(decyphered);
        expected_and = (temp * 3) % p;

        nb_mult++;
    }
    while(expected_and == decrypted_and);

    printf("n = %i, t = %i, nb_mult = %i\n",n,t, nb_mult);

    start_time = clock();
        sw_decrypt(res, c3, key_pair);
        sw_decrypt(res, c1, key_pair);
    end_time = clock();
    elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    printf("n = %i, t = %i, decrypt : %f seconds\n",n,t, elapsed_time/2);

    fmpz_clear(bit3);
    fmpz_clear(bit1);
    fmpz_clear(c3);
    fmpz_clear(c1);
    fmpz_clear(decyphered);
    return 0;
}