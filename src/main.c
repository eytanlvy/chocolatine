#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../includes/utils.h"

int main() {
    int n = 2048;
    int t = 200;

    clock_t start_time = clock();

    KeyPair *key_pair = gen_key_pair(n, t);

    clock_t end_time = clock();
    double elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;

    printf("Temps de génération des clés: %f secondes\n", elapsed_time);

    // Écrire les valeurs de la clé privée et de la clé publique dans un fichier
    FILE *fout = fopen("key_pair.txt", "w");
    if (fout == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        return 1;
    }

    char *hex_str;

    fprintf(fout, "Secret key w: ");
    hex_str = fmpz_get_str(NULL, 16, key_pair->sk.w);
    fprintf(fout, "%s\n", hex_str);
    free(hex_str);

    fprintf(fout, "Public key d: ");
    hex_str = fmpz_get_str(NULL, 16, key_pair->pk.d);
    fprintf(fout, "%s\n", hex_str);
    free(hex_str);

    fprintf(fout, "Public key r: ");
    hex_str = fmpz_get_str(NULL, 16, key_pair->pk.r);
    fprintf(fout, "%s\n", hex_str);
    free(hex_str);

    fclose(fout);

    // Nettoyer la mémoire
    fmpz_clear(key_pair->sk.w);
    fmpz_clear(key_pair->pk.d);
    fmpz_clear(key_pair->pk.r);
    free(key_pair);

    return 0;
}
