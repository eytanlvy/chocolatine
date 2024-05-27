CC = gcc -Wall -g -O2
OBJ_DIR = obj
SRC_DIR = src
INCLUDE_DIR = includes

ifdef MAC
	FLAGS = -I/opt/homebrew/include -L/opt/homebrew/lib -lflint -lgmp
else
	FLAGS = -I/usr/local/include -L/usr/local/lib -lflint -lgmp
endif

SRCS = $(wildcard $(SRC_DIR)/*.c)

OBJS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

all: $(OBJ_DIR) main

main: $(OBJS)
	$(CC) -o main $(OBJS) $(FLAGS)

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) -I$(INCLUDE_DIR) -c $< -o $@ $(FLAGS)

clean:
	rm -rf $(OBJ_DIR) main
.PHONY: clean
