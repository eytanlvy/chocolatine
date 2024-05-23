CC = gcc -Wall -g -O2
OBJ_DIR = obj
SRC_DIR = src
INCLUDE_DIR = includes

.PHONY: clean

# Trouver tous les fichiers sources dans le répertoire src
SRCS = $(wildcard $(SRC_DIR)/*.c)

# Générer les noms de fichiers objets correspondants
OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

all: $(OBJ_DIR) main

main: $(OBJS)
	$(CC) -o main $(OBJS)

# Créer le répertoire obj s'il n'existe pas
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

# Règle générique pour compiler les fichiers .c en .o
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) -I$(INCLUDE_DIR) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR) main

