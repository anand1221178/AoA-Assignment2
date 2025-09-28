CC = gcc
CFLAGS = -Wall -Wextra -O2 -g
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin
DATA_DIR = data
INCLUDE_DIR = include

# Detect OS for proper linking
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
    LDFLAGS = -lm
endif
ifeq ($(UNAME_S),Darwin)
    LDFLAGS = -lm
endif

# Create directories if they don't exist
$(shell mkdir -p $(OBJ_DIR) $(BIN_DIR) $(DATA_DIR))

# Source files
SRCS = $(SRC_DIR)/bst.c $(SRC_DIR)/utils.c
OBJS = $(OBJ_DIR)/bst.o $(OBJ_DIR)/utils.o

# Targets
all: bst_test bst_experiments

# Build BST test program
bst_test: $(OBJS) $(OBJ_DIR)/main.o
	$(CC) $(CFLAGS) -o $(BIN_DIR)/bst_test $^ $(LDFLAGS)

# Build BST experiments program
bst_experiments: $(OBJS) $(OBJ_DIR)/bst_experiments.o
	$(CC) $(CFLAGS) -o $(BIN_DIR)/bst_experiments $^ $(LDFLAGS)

# Object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

$(OBJ_DIR)/bst_experiments.o: experiments/bst_experiments.c
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

# Run experiments and save data
run_experiments:
	./$(BIN_DIR)/bst_experiments > $(DATA_DIR)/bst_results.csv

# Generate plots (requires Python with matplotlib)
plot:
	python3 scripts/plot_bst.py

# Run everything
experiments: bst_experiments run_experiments plot

# Clean up
clean:
	rm -f $(OBJ_DIR)/*.o
	rm -f $(BIN_DIR)/*
	rm -f $(DATA_DIR)/*.csv

.PHONY: all clean run_experiments plot experiments