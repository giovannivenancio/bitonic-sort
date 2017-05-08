CC = gcc
PARAMS = -g -fopenmp

EXEC = bitonic

all:
	$(CC) $(PARAMS) -c bitonic.c -o bitonic.o
	$(CC) $(PARAMS) -o $(EXEC) bitonic.o

clean:
	rm -rf *.o $(EXEC)
