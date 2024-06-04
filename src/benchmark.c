#include"../includes/benchmark.h"
#include<unistd.h>
#include<sys/types.h>
#include<sys/mman.h>
#include<sys/wait.h>
#include<fcntl.h>
#include<sys/stat.h>


int nb_multiplication(int n, int t, int p, KeyPair *key_pair){
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

    clear_key_pair(key_pair);
    fmpz_clear(cypher_factor);
    fmpz_clear(product);
    fmpz_clear(cypher_product);
    fmpz_clear(p_fmpz);
    return nb_mult;
}

static double average_time_n_keygen(int nb_keys, int n, int t, int p, int* nb_mult){
    double total_time = 0;
    int total_mult = 0;
    clock_t start_time, end_time;
    for(int i = 0; i < nb_keys; i++){
        start_time = clock();
        KeyPair* key_pair = gen_key_pair(n, t, p);
        end_time = clock();
        total_time += (double)(end_time - start_time) / CLOCKS_PER_SEC;
        total_mult += nb_multiplication(n, t, p, key_pair);
    }
    *nb_mult = total_mult/nb_keys;
    return total_time/nb_keys;
}

int find_pid(pid_t* pid_array, pid_t pid, int nb_process){
    for(int i = 0; i < nb_process; i++){
        if(pid == pid_array[i]){
            return i;
        }
    }
    return -1;   
}

void benchmark_keygen(char* filename){
    int n_start = 6;
    int n_end = 10;
    int t_start = 2;
    int t_end = 6;
    int p = 10;
    int nb_iterations = 1;
    if(filename != NULL){
        int output_file = open(filename,O_WRONLY | O_APPEND | O_CREAT,0666);
        if(output_file < 0){
            perror("[!] Error : open failed on file given in argument");
            exit(1);                                                                                                            
        }
        dup2(output_file, STDOUT_FILENO);
    }


    int max_process = sysconf(_SC_NPROCESSORS_ONLN);
    pid_t* pid = malloc(max_process*sizeof(pid_t));
    int nb_process = 0;
    int index;
    double** values = mmap(NULL, (n_end-n_start+1)*sizeof(double*),PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS,-1, 0);
    if(values == MAP_FAILED){
        perror("mmap failed");
        exit(1);
    }
    int** nb_mult = mmap(NULL, (n_end-n_start+1)*sizeof(int*),PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS,-1, 0);
    if(nb_mult == MAP_FAILED){
        perror("mmap failed");
        exit(1);
    }
    for(int i = n_start; i <= n_end; i ++){
        values[i-n_start] = mmap(NULL, (t_end-t_start+1)*sizeof(double),PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS,-1, 0);
        values[i-n_start] = mmap(NULL, (t_end-t_start+1)*sizeof(int),PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS,-1, 0);
        if(values[i-n_start] == MAP_FAILED){
            perror("mmap failed");
            exit(1);
        }
        if(nb_process >= max_process){
            index = find_pid(pid,wait(NULL),max_process);
            if(index < 0){
                perror("find_pid failed");
                exit(1);
            }
            pid[index] = fork();
        }
        else{
            index = nb_process;
            pid[nb_process++] = fork();
        }
        if(pid[index] < 0){
            perror("fork failed");
            exit(1);
        }
        if(pid[index] == 0){//child
            for(int j = t_start; j <= t_end; j++){
                values[i-n_start][j-t_start] = average_time_n_keygen(nb_iterations,1<<i,j*100,p, nb_mult[i-n_start]+j-t_start);
            }
            exit(0);
        }
    }

    for(int i = n_start; i <= n_end; i++){
        waitpid(pid[i-n_start],NULL,0);
        for(int j = t_start; j <= t_end; j++){
            printf("average time for n = %i, t = %i : %f, nb_mult = %i\n",1<<i,j*100,values[i-n_start][j-t_start],nb_mult[i-n_start][j-t_start]);
        }
        munmap(values[i-n_start],(t_end-t_start)*sizeof(double));
        munmap(nb_mult[i-n_start],(t_end-t_start)*sizeof(int));
    }
    munmap(values,(n_end-n_start)*sizeof(double*));
    munmap(nb_mult,(n_end-n_start)*sizeof(int*));
    free(pid);
}