#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <mpi.h>
#include <sys/time.h>
#include <limits.h>

#define NTHREADS 8
#define STD_TAG 0
#define MASTER 0

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
    unsigned int aux = elem[k];
    elem[k] = elem[i];
    elem[i] = aux;
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
    //verify_bitonic_sequence(elem, n);
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
    unsigned int i;

    #pragma omp parallel for num_threads(NTHREADS)
    for (i = 0; i < n; i++) {
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
    int n_elems, rank, n_procs, start;
    unsigned int i, j, k, n;
    unsigned short int *elem;
    float elapsed;
    struct timeval t0, t1;

    MPI_Status status;
    MPI_Comm workers;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &n_procs);
    MPI_Comm_split(MPI_COMM_WORLD, (rank == 0), rank, &workers);

    if (argc != 2) {
        printf("Usage: %s <N>\n", argv[0]);
        exit(1);
    }
    else {
        n = atoll(argv[1]);
    }

    n_elems = (n/2)/(n_procs-1);

    elem = malloc(n * sizeof(unsigned short int));

    if (rank == MASTER) {
        header(n);

        // how many times a number should be repeated while filling the array
        unsigned short int repeat = n / (USHRT_MAX+1);
        generate_bitonic_sequence(elem, n, repeat);
    }

    printf("Worker %d ready!\n", rank);
    // Wait all processes to begin
    MPI_Barrier(MPI_COMM_WORLD);

    MPI_Bcast(elem, n, MPI_UNSIGNED_SHORT, 0, MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD);

    if (rank == MASTER) {
        for (i = 0; i < n; i++) {
            printf("%d ", elem[i]);
        }
        printf("\n");
    }

    MPI_Barrier(MPI_COMM_WORLD);

    int c, c2, done = -1;
    unsigned int to_compare[n/2];
    if (rank != MASTER) {
        for (k = n/2; k >= 1; k /= 2) {

            c = 0;
            for (i = 0; i < n; i += 2*k) {
                for (j = 0; j < k; j++) {
                    to_compare[c++] = i+j;
                }
            }

            MPI_Barrier(workers);

            start = (rank-1) * n_elems;
            for (j = start; j < start + n_elems; j++) {
                if (elem[to_compare[j]] > elem[to_compare[j] + k]) {
                    printf("%d: troque %d\n", rank, to_compare[j]);
                    MPI_Send(&to_compare[j], 1, MPI_INT, MASTER, STD_TAG, MPI_COMM_WORLD);
                }
            }
            printf("%d: nada mais a trocar\n", rank);
            MPI_Send(&done, 1, MPI_INT, MASTER, STD_TAG, MPI_COMM_WORLD);
            MPI_Barrier(workers);

            MPI_Bcast(elem, n, MPI_UNSIGNED_SHORT, 0, MPI_COMM_WORLD);
        }
    }
    else {
        int worker, pos;
        for (k = n/2; k >= 1; k /= 2) {

            for (worker = 1; worker < n_procs; worker++) {
                MPI_Recv(&pos, 1, MPI_INT, worker, STD_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                while (pos != -1) {
                    printf("[MASTER] vou trocar %d\n", pos);

                    /*for (i = 0; i < n; i++) {
                        printf("%d ", elem[i]);
                    }*/
                    printf("\n");
                    swap(elem, pos, pos+k);
                    /*for (i = 0; i < n; i++) {
                        printf("%d ", elem[i]);
                    }
                    printf("\n");*/
                    MPI_Recv(&pos, 1, MPI_INT, worker, STD_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                }
            }
            for (i = 0; i < n; i++) {
                printf("%d ", elem[i]);
            }
            printf("\n");

            MPI_Bcast(elem, n, MPI_UNSIGNED_SHORT, 0, MPI_COMM_WORLD);
        }
    }

    if (rank == MASTER) {
        // verify if array is sorted
        verify_sorted(elem, n);

        free(elem);
    }

    MPI_Finalize();
    exit(0);
}
