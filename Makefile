CC = gcc -Wall -g -O2

.PHONY: clean

all: main

main: main.o keygen.o polynomial.o
	$(CC) -o main main.o keygen.o polynomial.o

main.o: src/main.c includes/includes.h
	$(CC) -c src/main.c

keygen.o: src/keygen.c includes/keygen.h
	$(CC) -c src/keygen.c

polynomial.o: src/polynomial.c includes/polynomial.h
	$(CC) -c src/polynomial.c

clean:
	rm -f main *.o