CC = gcc
PARAMS_PAR = -g -fopenmp 
PARAMS_SEQ = -g

EXEC_PAR = bitonic_par
EXEC_SEQ = bitonic_seq

all:
	$(CC) $(PARAMS_PAR) -c bitonic.c -o bitonic_par.o -lm
	$(CC) $(PARAMS_SEQ) -c bitonic.c -o bitonic_seq.o -lm
	$(CC) $(PARAMS_PAR) -o $(EXEC_PAR) bitonic_par.o -lm
	$(CC) $(PARAMS_SEQ) -o $(EXEC_SEQ) bitonic_seq.o -lm

clean:
	rm -rf *.o $(EXEC_PAR) $(EXEC_SEQ) 
