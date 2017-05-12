#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <sys/time.h>
#include <limits.h>

#define NTHREADS 8

/*---------------------------------------------------------------------------*/

void generate_bitonic_sequence(unsigned short int *elem, unsigned int n, unsigned short int repeat);
void header(unsigned int n);
void swap(unsigned short int *elem, unsigned int i, unsigned int k);
void verify_sorted(unsigned short int *elem, unsigned int n);
void verify_bitonic_sequence(unsigned short int *elem, unsigned int n);

/*---------------------------------------------------------------------------*/

void
header(unsigned int n) {
    printf("*********************************\n");
    printf("- Problem: Bitonic Sort\n");
    printf("- N: %u\n", n);
    printf("- Threads: %d\n", NTHREADS);
    printf("*********************************\n\n");
}

/*---------------------------------------------------------------------------*/

void
swap(unsigned short int *elem, unsigned int i, unsigned int k) {
    if (elem[i] > elem[i+k]) {
        unsigned int aux = elem[i+k];
        elem[i+k] = elem[i];
        elem[i] = aux;
    }
}

/*---------------------------------------------------------------------------*/

// http://stackoverflow.com/questions/10192903/time-in-milliseconds
float delta(struct timeval t0, struct timeval t1) {
    return (t1.tv_sec - t0.tv_sec) * 1000.0f + (t1.tv_usec - t0.tv_usec) / 1000.0f;
}

/*---------------------------------------------------------------------------*/

void
generate_bitonic_sequence(unsigned short int *elem, unsigned int n, unsigned short int repeat) {
    unsigned int i = 0, k;
    unsigned short int repeat_counter = 0;

    for (k = 0; k < n; k++) {
        elem[k] = i;
        repeat_counter++;

        if (repeat_counter > repeat) {
            if (k < n/2) i++;
            else i--;
            repeat_counter = 0;
        }
    }

    // verify if array is a bitonic sequence
    verify_bitonic_sequence(elem, n);
}

/*---------------------------------------------------------------------------*/

void
verify_bitonic_sequence(unsigned short int *elem, unsigned int n) {
    for (unsigned int k = 0; k < n; k++) {
        if (k < n/2) {
            if (elem[k] > elem[k+1] && k+1 < n/2) {
                printf("Array isn't a bitonic sequence!\n");
                exit(1);
            }
        }
        else {
            if (elem[k] < elem[k+1] && k+1 < n) {
                printf("Array isn't a bitonic sequence!\n");
                exit(1);
            }
        }
    }

    printf("Array is a bitonic sequence!\n");
}

/*---------------------------------------------------------------------------*/

void
verify_sorted(unsigned short int *elem, unsigned int n) {
    for (unsigned int i = 0; i < n; i++) {
        if (elem[i] > elem[i+1] && i+1 < n) {
            printf("Array isn't sorted!\n");
            exit(1);
        }
    }

    printf("Array is sorted!\n");
}

/*---------------------------------------------------------------------------*/

int
main(int argc, char *argv[]) {
    unsigned int i, j, k, n;
    unsigned short int *elem;

    float elapsed;
    struct timeval t0, t1;

    if (argc != 2) {
        printf("Usage: %s <N>\n", argv[0]);
        exit(1);
    }
    else {
        n = atoll(argv[1]);
    }

    // how many times a number should be repeated while filling the array
    unsigned short int repeat = n / (USHRT_MAX+1);

    elem = malloc(n * sizeof(unsigned short int));

    if (!elem) {
        printf("Failed to allocate memory!\n");
        exit(1);
    }

    header(n);

    gettimeofday(&t0, 0);
    generate_bitonic_sequence(elem, n, repeat);
    gettimeofday(&t1, 0);

    elapsed = delta(t0, t1);
    printf("Array initialized in %.2f ms\n\n", elapsed);

    // log n steps
    for (k = n/2; k >= 1; k /= 2) {
        gettimeofday(&t0, 0);

        // loop through halves
        #pragma omp parallel for collapse(2) schedule(guided) num_threads(NTHREADS)
        for (i = 0; i < n; i += 2*k) {
            // loop through elements of a half
            for (j = 0; j < k; j++) {
                swap(elem, i+j, k);
            }
        }

        gettimeofday(&t1, 0);
        elapsed = delta(t0, t1);

        printf("k = %u\nTime (ms): %.2f\n\n", k, elapsed);
    }

    // verify if array is sorted
    verify_sorted(elem, n);
}
