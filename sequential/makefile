CC = gcc
PARAMS = -g

EXEC = harmonic

all:
	$(CC) $(PARAMS) -c harmonic.c -o harmonic.o
	$(CC) $(PARAMS) -o $(EXEC) harmonic.o

clean:
	rm -rf *.o $(EXEC)
