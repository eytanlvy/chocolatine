#include"../includes/benchmark.h"
#include<unistd.h>
#include<sys/types.h>
#include<sys/mman.h>
#include<sys/wait.h>
#include<fcntl.h>
#include<sys/stat.h>


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
    for(int i = n_start; i <= n_end; i ++){
        values[i-n_start] = mmap(NULL, (t_end-t_start+1)*sizeof(double),PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS,-1, 0);
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
                values[i-n_start][j-t_start] = average_time_n_keygen(nb_iterations,1<<i,j*100,p);
            }
            exit(0);
        }
    }

    for(int i = n_start; i <= n_end; i++){
        waitpid(pid[i-n_start],NULL,0);
        for(int j = t_start; j <= t_end; j++){
            printf("average time for n = %i, t = %i : %f\n",1<<i,j*100,values[i-n_start][j-t_start]);
        }
        munmap(values[i-n_start],t_end-t_start);
    }
    munmap(values,n_end-n_start);
    free(pid);
}