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
void swap(unsigned short int *elem, unsigned int i, unsigned int k);
void verify_sorted(unsigned short int *elem, unsigned int n);
void verify_bitonic_sequence(unsigned short int *elem, unsigned int n);

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

    int n_elems,    // number of elements per process
        rank,       // process rank
        n_procs,    // total number of processes
        start,
        done = -1,  // used to inform master that some worker will not send more data
        worker,
        index;

    unsigned int i, j, k, l, n;
    unsigned short int *elem;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &n_procs);

    // communicator of workers (i.e., all processes except master)
    MPI_Comm workers;
    MPI_Comm_split(MPI_COMM_WORLD, (rank == 0), rank, &workers);

    if (argc != 2) {
        printf("Usage: %s <N>\n", argv[0]);
        exit(1);
    }
    else {
        n = atoll(argv[1]);
    }

    // hold the indexes of elements that should be compared
    unsigned int *to_compare;
    to_compare = malloc((n/2) * sizeof(unsigned int));

    // n/2 elements are compared per step.
    // n_procs - 1 because we have a master process
    n_elems = (n/2)/(n_procs-1);

    elem = malloc(n * sizeof(unsigned short int));

    if (!elem) {
        printf("Process %d has failed to allocate memory\n", rank);
        exit(1);
    }

    if (rank == MASTER) {
        // how many times a number should be repeated while filling the array
        unsigned short int repeat = n / (USHRT_MAX+1);
        generate_bitonic_sequence(elem, n, repeat);
    }

    // master broadcast the array to all workers.
    MPI_Bcast(elem, n, MPI_UNSIGNED_SHORT, 0, MPI_COMM_WORLD);

    if (rank != MASTER) {
        for (k = n/2; k >= 1; k /= 2) {
            for (i = 0, l = 0; i < n; i += 2*k) {
                for (j = 0; j < k; j++) {
                    // store index of element that should be compared
                    to_compare[l++] = i + j;
                }
            }

            // wait until all workers compute these indexes
            // indexes are equal to all workers
            // NOTE: maybe one worker compute and broadcast indexes through workers comm?
            MPI_Barrier(workers);

            // given an array of indexes, compute the start index for each process
            // based on the number of elements that each process should compare.
            // ex.: first n/2 comparisons (with n = 16 and p = 4)
            //      worker 1: process elements 0, 1
            //      worker 2: process elements 2, 3
            //      worker 3: process elements 4, 5
            //      worker 4: process elements 6, 7
            start = (rank-1) * n_elems;
            for (j = start; j < start + n_elems; j++) {
                if (elem[to_compare[j]] > elem[to_compare[j] + k]) {
                    // if element should be swapped, send to master the index
                    MPI_Send(&to_compare[j], 1, MPI_INT, MASTER, STD_TAG, MPI_COMM_WORLD);
                }
            }

            // notify master that worker has finished this step
            MPI_Send(&done, 1, MPI_INT, MASTER, STD_TAG, MPI_COMM_WORLD);

            // after master finished swapping, receive the new array
            MPI_Bcast(elem, n, MPI_UNSIGNED_SHORT, 0, MPI_COMM_WORLD);
        }
    }
    else {
        for (k = n/2; k >= 1; k /= 2) {
            printf("k = %u\n", k);

            for (worker = 1; worker < n_procs; worker++) {
                // receive index from worker
                MPI_Recv(&index, 1, MPI_INT, worker, STD_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

                while (index != -1) {
                    swap(elem, index, index + k);
                    MPI_Recv(&index, 1, MPI_INT, worker, STD_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                }
            }

            // after all the swaps, send new array to all workers
            MPI_Bcast(elem, n, MPI_UNSIGNED_SHORT, 0, MPI_COMM_WORLD);
        }
    }

    if (rank == MASTER) {
        // verify if array is sorted
        verify_sorted(elem, n);
    }

    free(elem);
    free(to_compare);

    MPI_Finalize();
    exit(0);
}
