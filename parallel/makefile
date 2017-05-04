CC = gcc
PARAMS = -g -fopenmp

EXEC = uniq

all:
	$(CC) $(PARAMS) -c uniq.c -o uniq.o
	$(CC) $(PARAMS) -o $(EXEC) uniq.o

clean:
	rm -rf *.o $(EXEC)
