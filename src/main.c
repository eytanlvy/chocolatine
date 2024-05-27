#include "../includes/tests.h"


int main(int argc, char **argv) {
    int n = (argc > 1) ? atoi(argv[1]) : 128;
    int t = (argc > 2) ? atoi(argv[2]) : 128;
	int p = (argc > 3) ? atoi(argv[3]) : 2;
	
	test_somewhat_int_mul(n,t,p);
    return 0;
}