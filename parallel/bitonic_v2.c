#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <sys/time.h>

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
    printf("- Threads: MUDAR\n");
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

float timedifference_msec(struct timeval t0, struct timeval t1) {
    return (t1.tv_sec - t0.tv_sec) * 1000.0f + (t1.tv_usec - t0.tv_usec) / 1000.0f;
}

/*---------------------------------------------------------------------------*/

int
main(int argc, char *argv[]) {
    unsigned int i, j, k, term;
    long long int n;
    unsigned int *elem;

    struct timeval t0;
    struct timeval t1;
    float elapsed;


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
        term = n/(2*k);
        // loop through halves
        gettimeofday(&t0, 0);
        #pragma omp parallel for collapse(2) schedule(guided) num_threads(8)
        for (i = 0; i < n; i += 2*k) {
            // loop through elements of a half
            for (j = 0; j < k; j++) {
                swap(elem, i+j, k);
            }
        }
        gettimeofday(&t1, 0);
        elapsed = timedifference_msec(t0, t1);
        printf("Loop executed in %f milliseconds.\n", elapsed);
        printf("k = %u\nt = %u\n\n", k, term);
    }

    /*for (i = 0; i < n; i++) {
        printf("%u ", elem[i]);
    }
    printf("\n");*/

    verify(elem, n);
}
