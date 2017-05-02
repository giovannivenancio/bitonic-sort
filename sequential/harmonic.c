#include <stdio.h>
#include <stdlib.h>

/*

limite: 1000000000000000000
        1.000.000.000.000.000.000

*/

int main(int argc, char *argv[]) {
    int k;
    double h_number = 0.0;
    long long int n;

    if (argc != 2) {
        printf("Usage: %s <N>\n", argv[0]);
        exit(1);
    }
    else {
        n = atoll(argv[1]);
    }

    printf("*********************************\n");
    printf("* Problem: Harmonic Series\n");
    printf("* N: %lld\n", n);
    printf("* Threads: Sequential\n");
    printf("*********************************\n\n");

    for (k = 1; k <= n; k++) {
        h_number += (double) 1/k;
    }

    printf("%f\n", h_number);
}
