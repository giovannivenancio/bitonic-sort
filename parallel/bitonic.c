#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

/*---------------------------------------------------------------------------*/

void generate_bitonic_sequence(int *elem, long long int n);
void header(long long int n);
void swap(int *elem, int i, int k);
void verify(int *elem, long long int n);

/*---------------------------------------------------------------------------*/

void
generate_bitonic_sequence(int *elem, long long int n) {
    int i = 0;
    int first = 1;
    int j = 0;
    int k = 0;

    //while (k < n) {
    #pragma omp parallel for private(i, j) firstprivate(first)
    for(k = 0; k<n ; k++){
        if(first == 1){
            first = 0;
            i = k;
            
            if(i >= (n/2))
                j = k-((k-(n/2))*2);
            else
                j = 0;
        }
        if (i < n/2) {
            elem[k] = i;
            i++;
        }
        else {
            elem[k] = j;
            j--;
        }

    }
    /*for (i = 0; i < n; i++) {
        printf("|%d", elem[i]);
    }*/
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
swap(int *elem, int i, int k) {
    int aux = elem[i+k];
    elem[i+k] = elem[i];
    elem[i] = aux;
}

/*---------------------------------------------------------------------------*/

void
verify(int *elem, long long int n) {
    int i;

    #pragma omp parallel for
    for (i = 0; i < n; i++) {
        if (elem[i] > elem[i+1] && i+1 < n) {
            printf("Error!\n");
            exit(1);
        }
        //printf("|%d", elem[i]);
    }
    
    for (i = 0; i < n; i++) {
        printf("|%d", elem[i]);
    }
    printf("\nSuccess!\n");
}

/*---------------------------------------------------------------------------*/

int
main(int argc, char *argv[]) {
    int i, j, k, oldK;
    long long int n;
    int *elem;

    if (argc != 2) {
        printf("Usage: %s <N>\n", argv[0]);
        exit(1);
    }
    else {
        n = atoll(argv[1]);
    }

    elem = malloc(n * sizeof(int));

    if( elem == NULL ) {
        puts("malloc falhou!!!");
        exit(1);
    }

    header(n);
    generate_bitonic_sequence(elem, n);
    oldK = n/2;
    // log n steps
    #pragma omp parallel for collapse(3) firstprivate(oldK)
    for (k = n/2; k >= 1; k -= oldK/2) {
        //printf("\nk = %u\n", k);
        // loop through halves
        for (i = 0; i < n; i += k) {
            // loop through elements of a half
            for (j = 0; j < k; j++) {
                printf("I:%d\nJ:%d\n", i, j);
                if (elem[i] > elem[i+k]) {
                    swap(elem, i, k);
                }
                i++;
                if(k == 1 && i >= (n-1))
                    oldK = 2;
                else
                    oldK = k;
                printf("%d\n", k);
            }
        }
        
        
    }

    /*for (i = 0; i < n; i++) {
        printf("%lld ", elem[i]);
    }
    printf("\n");*/

    verify(elem, n);
}
