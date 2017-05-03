#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define SIZE 500000

//http://stackoverflow.com/a/822361/2382100
/* Returns an integer in the range [0, n).
 *
 * Uses rand(), and so is affected-by/affects the same seed.
 */
int randint(int n) {
  if ((n - 1) == RAND_MAX) {
    return rand();
  } else {
    // Chop off all of the values that would cause skew...
    long end = RAND_MAX / n; // truncate skew
    assert (end > 0L);
    end *= n;

    // ... and ignore results from rand() that fall above that limit.
    // (Worst case the loop condition should succeed 50% of the time,
    // so we can expect to bail out of this loop pretty quickly.)
    int r;
    while ((r = rand()) >= end);

    return r % n;
  }
}

int main(int argc, char *argv[]) {
    int repeatedArray[2][SIZE];
    int i, j;
    
    for(i = 0; i < SIZE; i++){
        int value = randint(SIZE/10);
        repeatedArray[0][i] = randint(SIZE/10);
        repeatedArray[1][i] = 0;
    }
    
    for(i = 0; i < SIZE; i++){
        if(repeatedArray[1][i] == 0){
            for(j = i+1; j < SIZE; j++){
                if(repeatedArray[0][i] == repeatedArray[0][j]){
                    repeatedArray[1][i]++;
                    repeatedArray[1][j] = 1;
                }
            }
            //printf("O valor %d aparece %d vez(es)\n", repeatedArray[0][i], repeatedArray[1][i]);
        }
    }
}
