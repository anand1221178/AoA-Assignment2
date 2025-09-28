# Binary Search Trees and Order-statistic Trees Assignment

## Part A: Binary Search Trees

### Implementation
This project implements Binary Search Trees (BST) following the algorithms from CLRS Chapter 12.

### Files Structure
```
├── include/
│   ├── bst.h         # BST data structures and function declarations
│   └── utils.h       # Utility functions for experiments
├── src/
│   ├── bst.c         # BST implementation (from CLRS)
│   ├── utils.c       # Utility functions implementation
│   └── main.c        # Test program
├── experiments/
│   └── bst_experiments.c  # Part A experiments
├── scripts/
│   └── plot_bst.py   # Python script for generating graphs
├── data/             # Experimental results (CSV files)
├── graphs/           # Generated plots
└── bin/              # Compiled executables

```

### Algorithms Implemented (Part A)
1. **TREE-INSERT** - Insert a node into BST
2. **TREE-DELETE** - Delete a node from BST  
3. **INORDER-TREE-WALK** - Traverse tree in sorted order
4. **TREE-SEARCH** - Search for a key
5. **TREE-MINIMUM/MAXIMUM** - Find min/max in tree
6. **TREE-SUCCESSOR** - Find successor of a node
7. **TRANSPLANT** - Helper for deletion

### How to Build and Run

#### Prerequisites
- GCC compiler
- Python 3 with matplotlib (for plotting)
- Make

#### Quick Start
```bash
# Make the run script executable
chmod +x run.sh

# Run everything (compile, test, experiments, plots)
./run.sh
```

#### Manual Steps
```bash
# 1. Compile the code
make all

# 2. Run basic tests
./bin/bst_test

# 3. Run experiments
make run_experiments

# 4. Generate plots
make plot
```

### Experiments

#### Experiment 1: Tree Height
- Builds random BSTs of varying sizes (10 to 10,000 nodes)
- Measures average height over 100 trees per size
- Confirms O(log n) expected height for randomly built BSTs

#### Experiment 2: Build Time
- Measures time to build BSTs using repeated TREE-INSERT
- Tests various tree sizes with random key insertion order
- Confirms expected O(n log n) build time

#### Experiment 3: Destroy Time  
- Measures time to destroy BSTs using repeated TREE-DELETE on root
- Tests deletion performance on various tree sizes
- Analyzes the complexity of destroying entire trees

#### Experiment 4: Inorder Walk Time
- Measures runtime of INORDER-TREE-WALK
- Confirms Θ(n) runtime complexity
- Uses silent traversal (no printing) for accurate timing

### Results

The experiments generate:
- **data/bst_results.csv** - Raw experimental data
- **graphs/bst_height.png** - Height vs n plot
- **graphs/bst_build_time.png** - Build time vs n plot  
- **graphs/bst_destroy_time.png** - Destroy time vs n plot
- **graphs/bst_inorder_time.png** - Inorder walk time vs n plot

### Key Findings

1. **Height**: Randomly built BSTs have average height ≈ 2 log₂(n), confirming Theorem 12.4
2. **Build Time**: O(n log n) average case for random insertion order
3. **Destroy Time**: O(n log n) for destroying via root deletion
4. **Inorder Walk**: Θ(n) as expected for visiting all nodes

## Part B: Order-statistic Trees

(To be implemented in the next phase)

### Planned Implementation
- OS-SELECT - Select i-th smallest element
- OS-RANK - Find rank of element
- Modified TREE-INSERT/DELETE to maintain size attribute

---

## Author
Anand Patel

## References
- Introduction to Algorithms (3rd Edition) - Cormen, Leiserson, Rivest, Stein
- Chapter 12: Binary Search Trees
- Chapter 14: Augmenting Data Structures