CC = gcc
CFLAGS = -Wall -Wextra -O2 -std=c99 -I./include
LDFLAGS = -lm

# Detect OS
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)
    CFLAGS += -D_GNU_SOURCE
endif
ifeq ($(UNAME_S),Darwin)
    # macOS specific flags if needed
endif
ifdef OS
    # Windows
    EXE_EXT = .exe
else
    EXE_EXT =
endif

# Directories
SRC_DIR = src
INC_DIR = include
OBJ_DIR = obj
BIN_DIR = bin
EXP_DIR = experiments
DATA_DIR = data

# Create directories if they don't exist
$(shell mkdir -p $(OBJ_DIR) $(BIN_DIR) $(DATA_DIR))

# Source files
BST_SRCS = $(SRC_DIR)/bst.c $(SRC_DIR)/utils.c
OS_TREE_SRCS = $(SRC_DIR)/os_tree.c $(SRC_DIR)/utils.c
EXP_SRCS = $(EXP_DIR)/bst_experiments.c $(EXP_DIR)/os_experiments.c

# Object files
BST_OBJS = $(OBJ_DIR)/bst.o $(OBJ_DIR)/utils.o
OS_TREE_OBJS = $(OBJ_DIR)/os_tree.o $(OBJ_DIR)/utils.o
EXP_OBJS = $(OBJ_DIR)/bst_experiments.o $(OBJ_DIR)/os_experiments.o

# Executables
BST_EXP = $(BIN_DIR)/bst_experiments$(EXE_EXT)
OS_EXP = $(BIN_DIR)/os_experiments$(EXE_EXT)
MAIN = $(BIN_DIR)/main$(EXE_EXT)

# Default target
all: $(BST_EXP) $(OS_EXP) $(MAIN)

# Main executable
$(MAIN): $(OBJ_DIR)/main.o $(BST_OBJS) $(OS_TREE_OBJS)
	$(CC) $^ -o $@ $(LDFLAGS)

# BST experiments executable
$(BST_EXP): $(OBJ_DIR)/bst_experiments.o $(BST_OBJS)
	$(CC) $^ -o $@ $(LDFLAGS)

# OS-tree experiments executable
$(OS_EXP): $(OBJ_DIR)/os_experiments.o $(OS_TREE_OBJS) $(OBJ_DIR)/bst.o
	$(CC) $^ -o $@ $(LDFLAGS)

# Object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/%.o: $(EXP_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Run experiments
run-bst: $(BST_EXP)
	./$(BST_EXP)

run-os: $(OS_EXP)
	./$(OS_EXP)

run-all: run-bst run-os
	@echo "All experiments completed. Check data/ directory for results."

# Generate graphs
graphs:
	python3 scripts/plot_graphs.py

# Clean
clean:
	rm -rf $(OBJ_DIR)/*.o $(BIN_DIR)/* $(DATA_DIR)/*.csv

# Clean everything including data
clean-all: clean
	rm -rf $(DATA_DIR)/*

# Help
help:
	@echo "Available targets:"
	@echo "  all       - Build all executables"
	@echo "  run-bst   - Run BST experiments"
	@echo "  run-os    - Run Order-Statistic tree experiments"
	@echo "  run-all   - Run all experiments"
	@echo "  graphs    - Generate graphs from CSV data"
	@echo "  clean     - Remove object files and executables"
	@echo "  clean-all - Remove everything including data"
	@echo "  help      - Show this help message"

.PHONY: all run-bst run-os run-all graphs clean clean-all help