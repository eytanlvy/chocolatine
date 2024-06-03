#include"../includes/benchmark.h"

static double average_time_n_keygen(int nb_keys, int n, int t, int p){
    double total_time = 0;
    clock_t start_time, end_time;
    for(int i = 0; i < nb_keys; i++){
        start_time = clock();
        gen_key_pair(n, t, p);
        end_time = clock();
        total_time += (double)(end_time - start_time) / CLOCKS_PER_SEC;
    }
    return total_time/nb_keys;
}

void benchmark_keygen(){
    int n_start = 1<<8;
    int n_end = 1<<12;
    int t_start = 200;
    int t_end = 1000;
    int p = 10;

    for(int i = n_start; i <= n_end; i <<= 1){
        for(int j = t_start; j < t_end; j += 100){
            printf("average time for n = %i, t = %i : %f\n",i,j,average_time_n_keygen(10,i,j,p));
        }

    }
}