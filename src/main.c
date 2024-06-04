#include "../includes/tests.h"
#include "../includes/benchmark.h"


int main(int argc, char **argv) {
    // int n = (argc > 1) ? atoi(argv[1]) : 128;
    // int t = (argc > 2) ? atoi(argv[2]) : 128;
	// int p = (argc > 3) ? atoi(argv[3]) : 2;
	

	benchmark_keygen("result.txt");
    return 0;
}