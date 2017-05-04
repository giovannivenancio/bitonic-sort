#include <stdio.h>
#include <stdlib.h>

/*
2^27 = 134217728 => 17s

*/

/*---------------------------------------------------------------------------*/

void generate_bitonic_sequence(long long int *elem, long long int n);
void header(long long int n);
void swap(long long int *elem, int i, int k);
void verify(long long int *elem, int n);

/*---------------------------------------------------------------------------*/

void
generate_bitonic_sequence(long long int *elem, long long int n) {
    int i = 0, j = n/2, k = 0;

    while (k < n) {
        if (i < n/2) {
            elem[k] = i;
            i++;
        }
        else {
            elem[k] = j;
            j--;
        }

        k++;
    }
}

/*---------------------------------------------------------------------------*/

void
header(long long int n) {
    printf("*********************************\n");
    printf("- Problem: Bitonic Sort\n");
    printf("- N: %lld\n", n);
    printf("- Threads: Sequential\n");
    printf("*********************************\n\n");
}

/*---------------------------------------------------------------------------*/

void
swap(long long int *elem, int i, int k) {
    int aux = elem[i+k];
    elem[i+k] = elem[i];
    elem[i] = aux;
}

/*---------------------------------------------------------------------------*/

void
verify(long long int *elem, int n) {
    int i;

    for (i = 0; i < n; i++) {
        if (elem[i] > elem[i+1] && i+1 < n) {
            printf("Error!\n");
            exit(1);
        }
    }
    printf("Success!\n");
}

/*---------------------------------------------------------------------------*/

int
main(int argc, char *argv[]) {
    int i, j, k;
    long long int n;
    long long int *elem;

    if (argc != 2) {
        printf("Usage: %s <N>\n", argv[0]);
        exit(1);
    }
    else {
        n = atoll(argv[1]);
    }

    elem = malloc(n * sizeof(long long int*));

    header(n);
    generate_bitonic_sequence(elem, n);

    // log n steps
    for (k = n/2; k >= 1; k /= 2) {
        // loop through halves
        for (i = 0; i < n; i += k) {
            // loop through elements of a half
            for (j = 0; j < k; i++, j++) {
                if (elem[i] > elem[i+k]) {
                    swap(elem, i, k);
                }
            }
        }
    }

    /*for (i = 0; i < n; i++) {
        printf("%lld ", elem[i]);
    }
    printf("\n");*/

    verify(elem, n);
}
