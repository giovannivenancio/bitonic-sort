#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <sys/time.h>
#include <limits.h>

#define NTHREADS 8
#define SIZE 64
#define STD_TAG 0

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
    if (i > k) {
        unsigned int aux = elem[3];
        elem[3] = elem[2];
        elem[2] = aux;
        
        aux = elem[1];
        elem[1] = elem[0];
        elem[0] = aux;
    }
}

void
updateArray(unsigned short int *elem, unsigned short int *recv) {
    
    elem[recv[0]] = recv[2];
    elem[recv[1]] = recv[3];
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
    unsigned int k;
    for (k = 0; k < n; k++) {
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

    //#pragma omp parallel for num_threads(NTHREADS)
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
waitProcess(MPI_Status status, unsigned short int *elem){
    unsigned short int recvElem[4];
    MPI_Recv(recvElem, 4, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
    updateArray(elem, recvElem);
    return status.MPI_SOURCE;
}

int
main(int argc, char *argv[]) {
    int finished, my_rank, n_procs, position;
    unsigned int i, j, k, n, proc_count;
    unsigned short int *elem;
    unsigned short int sendElem[4], recvElem[4];
    MPI_Status status;

    float elapsed;
    struct timeval t0, t1;
    
    n = SIZE;
    proc_count = 1;

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
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &n_procs);
    if(my_rank == 0){
        for (k = n/2; k >= 1; k /= 2) {
            gettimeofday(&t0, 0);

            // loop through halves
            for (i = 0; i < n; i += 2*k) {
                // loop through elements of a half
                for (j = 0; j < k; j++) {
                    int ijPos = i + j;
                    int kiPos = k + i;
                    MPI_Pack(&(ijPos), 1, MPI_UNSIGNED, sendElem, 32, &position, MPI_COMM_WORLD); 
                    MPI_Pack(&(kiPos), 1, MPI_UNSIGNED, sendElem, 32, &position, MPI_COMM_WORLD); 
                    MPI_Pack(&elem[i+j], 1, MPI_UNSIGNED_SHORT, sendElem, 32, &position, MPI_COMM_WORLD); 
                    MPI_Pack(&elem[k+i], 1, MPI_UNSIGNED_SHORT, sendElem, 32, &position, MPI_COMM_WORLD); 

                    if(proc_count <= 0){
                        printf("Entrei no proc_count 0\n");
                        int freeProcess = waitProcess(status, elem);                        
                        proc_count++;
                        printf("Enviando para o processo %d\n", freeProcess);
                        MPI_Send(sendElem, position, MPI_PACKED, freeProcess, 0, MPI_COMM_WORLD); 
                    }
                    else{
                        printf("Enviando normal para o proc_count %d\n", proc_count);
                        MPI_Send(sendElem, position, MPI_PACKED, proc_count, 0, MPI_COMM_WORLD);
                    }

                    proc_count--;
                    //swap(elem, i+j, k);
                }
                while(proc_count <= n_procs){
                    MPI_Recv(recvElem, 4, MPI_INT, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
                    updateArray(elem, recvElem);
                    proc_count++;
                }
                
            }

            gettimeofday(&t1, 0);
            elapsed = delta(t0, t1);

            printf("k = %u\nTime (ms): %.2f\n\n", k, elapsed);
        }
    }
    else{
        unsigned short int unpck[4];
        MPI_Recv(recvElem, 32, MPI_PACKED, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
        position = 0;
        MPI_Unpack(&recvElem, 32, &position, &unpck[0], 1, MPI_UNSIGNED_SHORT, MPI_COMM_WORLD);
        MPI_Unpack(&recvElem, 32, &position, &unpck[1], 1, MPI_UNSIGNED_SHORT, MPI_COMM_WORLD);
        MPI_Unpack(&recvElem, 32, &position, &unpck[2], 1, MPI_UNSIGNED_SHORT, MPI_COMM_WORLD);
        MPI_Unpack(&recvElem, 32, &position, &unpck[3], 1, MPI_UNSIGNED_SHORT, MPI_COMM_WORLD);
        printf("Recebi %d | %d | %d | %d\n",unpck[0], unpck[1], unpck[2], unpck[3]);
        swap(unpck, unpck[2], unpck[3]);
        MPI_Send(unpck, 4, MPI_INT,  0, 0, MPI_COMM_WORLD); 

    }

    // verify if array is sorted
    verify_sorted(elem, n);

    free(elem);
    exit(0);
}
