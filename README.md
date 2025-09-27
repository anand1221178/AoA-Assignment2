# Binary Search Trees and Order-Statistic Trees Assignment

## Project Structure

```
.
├── README.md           # This file
├── Makefile           # Cross-platform build system
├── include/           # Header files
│   ├── bst.h         # Binary Search Tree declarations
│   ├── os_tree.h     # Order-Statistic Tree declarations
│   └── utils.h       # Utility functions (timing, random, etc.)
├── src/              # Implementation files
│   ├── bst.c         # BST implementation
│   ├── os_tree.c     # OS-Tree implementation
│   ├── utils.c       # Utility implementations
│   └── main.c        # Main driver program
├── experiments/      # Experiment files
│   ├── bst_experiments.c    # Part A experiments
│   └── os_experiments.c     # Part B experiments
├── data/             # Output CSV files (generated)
│   ├── bst_heights.csv
│   ├── bst_build_times.csv
│   ├── bst_delete_times.csv
│   ├── inorder_walk_times.csv
│   └── os_comparison.csv
├── scripts/          # Python scripts
│   └── plot_graphs.py    # Graph generation script
├── obj/              # Object files (generated)
├── bin/              # Executables (generated)
└── report/           # Final report and graphs

```

## Requirements

### For C code:
- GCC or compatible C compiler
- Standard C library (C99)
- POSIX-compliant system (Linux/macOS) or Windows with MinGW

### For graphing:
- Python 3.x
- matplotlib
- pandas
- numpy

## Building the Project

### On Linux/macOS:
```bash
make all          # Build all executables
make run-all      # Run all experiments
make graphs       # Generate graphs from data
```

### On Windows (with MinGW):
```bash
mingw32-make all
mingw32-make run-all
python scripts/plot_graphs.py
```

## Running Experiments

### Part A - Binary Search Trees
```bash
make run-bst      # Run BST experiments only
```
This will:
- Build BSTs with random keys
- Measure tree heights
- Measure build times
- Measure deletion times
- Test Inorder-Tree-Walk performance

### Part B - Order-Statistic Trees
```bash
make run-os       # Run OS-Tree experiments only
```
This will:
- Test OS-Select and OS-Rank operations
- Compare performance with regular BST

### Generate All Graphs
```bash
make graphs       # Create all graphs from CSV data
```

## Implementation Notes

### Part A: Binary Search Trees
- Implements Tree-Insert, Tree-Delete, and Inorder-Tree-Walk
- Follows textbook algorithms exactly (CLRS 3rd edition, Chapter 12)
- No external libraries or packages used

### Part B: Order-Statistic Trees
- Augments BST nodes with size attribute
- Implements OS-Select and OS-Rank
- Modified Tree-Insert and Tree-Delete maintain size invariant
- Based on BST, not Red-Black Trees

## Experiments Configuration

Default test sizes: 100, 500, 1000, 5000, 10000, 50000, 100000 nodes
Repetitions vary by size (more for smaller trees)

## Output

All experiment results are saved as CSV files in the `data/` directory.
Graphs are generated in the `report/` directory.

## Cleaning Up

```bash
make clean        # Remove object files and executables
make clean-all    # Remove everything including data files
```

## Submission Requirements

1. Source code (all .c and .h files)
2. Document containing:
   - All required graphs
   - Description of experiment methodology
   - Explanation of size attribute maintenance in OS-Trees