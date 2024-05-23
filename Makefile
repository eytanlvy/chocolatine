CC = gcc -Wall -g -O2
OBJ_DIR = obj

.PHONY: clean

all: $(OBJ_DIR) main

main: $(OBJ_DIR)/main.o $(OBJ_DIR)/keygen.o $(OBJ_DIR)/polynomial.o
	$(CC) -o main $(OBJ_DIR)/main.o $(OBJ_DIR)/keygen.o $(OBJ_DIR)/polynomial.o

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/main.o: src/main.c includes/includes.h
	$(CC) -c src/main.c -o $(OBJ_DIR)/main.o

$(OBJ_DIR)/keygen.o: src/keygen.c includes/keygen.h
	$(CC) -c src/keygen.c -o $(OBJ_DIR)/keygen.o

$(OBJ_DIR)/polynomial.o: src/polynomial.c includes/polynomial.h
	$(CC) -c src/polynomial.c -o $(OBJ_DIR)/polynomial.o

clean:
	rm -rf $(OBJ_DIR) main
