#include <stdio.h>
#include <stdlib.h>

/*---------------------------------------------------------------------------*/

void generate_bitonic_sequence(unsigned int *elem, long long int n);
void header(long long int n);
void swap(unsigned int *elem, unsigned int i, unsigned int k);
void verify(unsigned int *elem, long long int n);

/*---------------------------------------------------------------------------*/

void
generate_bitonic_sequence(unsigned int *elem, long long int n) {
    unsigned int i = 0;
    unsigned int j = n/2;
    unsigned int k = 0;

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
swap(unsigned int *elem, unsigned int i, unsigned int k) {
    unsigned int aux;
    if (elem[i] > elem[i+k]) {
        aux = elem[i+k];
        elem[i+k] = elem[i];
        elem[i] = aux;
    }
}

/*---------------------------------------------------------------------------*/

void
verify(unsigned int *elem, long long int n) {
    unsigned int i;

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
    unsigned int i, j, k;
    long long int n;
    unsigned int *elem;

    if (argc != 2) {
        printf("Usage: %s <N>\n", argv[0]);
        exit(1);
    }
    else {
        n = atoll(argv[1]);
    }

    elem = malloc(n * sizeof(unsigned int));

    if( elem == NULL ) {
        puts("malloc falhou!!!");
        exit(1);
    }

    header(n);
    generate_bitonic_sequence(elem, n);

    // log n steps
    for (k = n/2; k >= 1; k /= 2) {
        printf("k = %u\n", k);
        // loop through halves
        for (i = 0; i < n; i += 2*k) {
            // loop through elements of a half
            for (j = 0; j < k; j++) {
                swap(elem, i+j, k);
            }
        }
    }

    /*for (i = 0; i < n; i++) {
        printf("%lld ", elem[i]);
    }
    printf("\n");*/

    verify(elem, n);
}
