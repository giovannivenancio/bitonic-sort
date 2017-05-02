#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int
main(int argc, char *argv[]) {
    int nthreads;
    double h_number = 0.0;
    long long int n;

    if (argc != 3) {
        printf("Usage: %s <N> <NTHREADS>\n", argv[0]);
        exit(1);
    }
    else {
        n = atoll(argv[1]);
        nthreads = atoi(argv[2]);
    }

    printf("************************************\n");
    printf("* Problem: Parallel Harmonic Series\n");
    printf("* N: %lld\n", n);
    printf("* Threads: %d\n", nthreads);
    printf("************************************\n\n");

    #pragma omp parallel num_threads(nthreads)
    {
        double partial_sum = 0.0;

        for (int k = omp_get_thread_num() + 1; k <= n; k += nthreads) {
            partial_sum += (double) 1/k;
        }

        #pragma omp critical
        h_number += partial_sum;
    }

    printf("%f\n", h_number);
}
