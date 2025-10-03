# Binary Search Trees and Order-Statistic Trees Assignment

**Course:** Applications of Algorithms
**Due:** Monday 13th October at 23h00
**Implementation:** C with Python plotting

## Project Overview

This assignment implements and analyzes:
- **Part A:** Binary Search Trees (BST) - Performance analysis
- **Part B:** Order-Statistic Trees - Augmented BST with rank operations

All algorithms coded from scratch following CLRS 3rd Edition textbook.

## Project Structure

```
.
├── include/
│   ├── bst.h          # BST declarations (Part A)
│   ├── os_tree.h      # Order-Statistic Tree declarations (Part B)
│   └── utils.h        # Utility functions
├── src/
│   ├── bst.c          # BST implementation (Chapter 12)
│   ├── os_tree.c      # OS-Tree implementation (Chapter 14)
│   ├── utils.c        # Timing, shuffling utilities
│   ├── main.c         # BST test program
│   └── os_main.c      # OS-Tree test program
├── experiments/
│   ├── bst_experiments.c   # Part A experiments
│   └── os_experiments.c    # Part B experiments
├── scripts/
│   ├── plot_bst.py    # Part A graph generation
│   └── plot_os.py     # Part B graph generation
├── data/              # CSV experimental results
├── graphs/            # Generated plots
├── bin/               # Compiled executables
├── obj/               # Object files
├── Makefile           # Build system
└── run.sh             # Automated experiment runner
```

## 🚀 Quick Start

### Prerequisites
- **C Compiler:** GCC (macOS/Linux) or MinGW (Windows)
- **Python 3:** With matplotlib, numpy, pandas
- **Make:** Build automation

```bash
# Install Python dependencies
pip install matplotlib numpy pandas
```

### Run Everything (Recommended)
```bash
# Make script executable (first time only)
chmod +x run.sh

# Run all experiments and generate graphs (45-60 minutes)
./run.sh
```

This will:
1. ✅ Clean previous builds
2. ✅ Compile all code
3. ✅ Run BST tests
4. ✅ Run Part A experiments
5. ✅ Run OS-Tree tests
6. ✅ Run Part B experiments
7. ✅ Generate all graphs

### Manual Steps
```bash
# Compile
make all

# Test implementations
./bin/bst_test
./bin/os_test

# Run experiments
./bin/bst_experiments > data/bst_results.csv
./bin/os_experiments > data/os_results.csv

# Generate graphs
python scripts/plot_bst.py
python scripts/plot_os.py
```

## Part A: Binary Search Trees ✅

### Algorithms Implemented (Chapter 12)
1. ✅ **TREE-INSERT** - Insert node into BST
2. ✅ **TREE-DELETE** - Delete node from BST (with TRANSPLANT)
3. ✅ **INORDER-TREE-WALK** - In-order traversal
4. ✅ **TREE-SEARCH** - Search for key
5. ✅ **TREE-MINIMUM/MAXIMUM** - Find min/max
6. ✅ **TREE-SUCCESSOR** - Find successor

### Experiments (4 Required)

**Configuration:**
- Tree sizes: 10 to 100,000 nodes
- Data points: ~80 samples
- Repetitions: 100-300 for small, 15-30 for large trees
- Shuffle methods: Fisher-Yates, RANDOMIZE-IN-PLACE, PERMUTE-BY-SORTING, No-Shuffle

#### (ii) Tree Height Analysis ✅
- Builds random BSTs with shuffled keys
- Records height for each tree
- **Verifies:** O(log n) expected height (Theorem 12.4)

#### (iii) Build Time Analysis ✅
- Times tree construction via repeated TREE-INSERT
- **Verifies:** O(n log n) build time

#### (iv) Deletion Time Analysis ✅
- Destroys tree by repeatedly deleting root
- **Verifies:** O(n log n) deletion time

#### (v) Inorder Walk Analysis ✅
- Confirms Θ(n) runtime
- Uses silent traversal for accurate timing

### Part A Graphs Generated
- Height vs n (with O(log n) theoretical)
- Build time vs n
- Delete time vs n
- Inorder walk time vs n

## Part B: Order-Statistic Trees ✅

### Algorithms Implemented (Chapter 14)

**Augmented BST with size attribute:**
1. ✅ **OS-TREE-INSERT** - Insert maintaining size
2. ✅ **OS-TREE-DELETE** - Delete maintaining size
3. ✅ **OS-SELECT** - Find i-th smallest element (O(log n))
4. ✅ **OS-RANK** - Find rank of element (O(log n))

**Key Implementation:**
- Each node stores `size` = subtree size
- Insert: Increment size along insertion path
- Delete: Decrement size along deletion path
- **Adapted from RB-Tree to plain BST** (no rotations needed)

### Experiments (4 Required)

#### (i) Insert Comparison ✅
- Compares OS-Tree vs BST insert times
- **Shows:** Overhead of maintaining size attribute

#### (ii) Delete Comparison ✅
- Compares OS-Tree vs BST delete times
- **Shows:** Overhead of maintaining size attribute

#### (iii) OS-SELECT Runtime ✅
- Measures time to find i-th smallest
- **Verifies:** O(log n) complexity

#### (iv) OS-RANK Runtime ✅
- Measures time to find rank
- **Verifies:** O(log n) complexity

### Part B Graphs Generated
- Insert comparison (OS-Tree vs BST)
- Delete comparison (OS-Tree vs BST)
- OS-SELECT runtime analysis
- OS-RANK runtime analysis
- Combined OS operations

## 📊 Results Summary

**Part A:**
- ✅ Height ≈ 2 log₂(n) for random BSTs
- ✅ Build time O(n log n)
- ✅ Delete time O(n log n)
- ✅ Inorder walk Θ(n)

**Part B:**
- ✅ OS-Tree insert ~1.0-1.2x slower than BST
- ✅ OS-Tree delete ~1.0-1.3x slower than BST
- ✅ OS-SELECT achieves O(log n)
- ✅ OS-RANK achieves O(log n)
- ✅ Trade-off: Slight overhead for powerful rank queries

## 📝 Submission Requirements

**✅ Completed:**
1. ✅ Source code (all .c and .h files)
2. ✅ All required graphs with methodology
3. ⏳ Documentation explaining size maintenance (see DOCUMENTATION.md)

**Files to Submit:**
- All source code (src/, include/, experiments/)
- Generated graphs (graphs/)
- This README
- Documentation (DOCUMENTATION.md)

## 🔧 Build System

```bash
make all           # Build everything
make clean         # Remove binaries, data, graphs
make os_test       # Build OS-Tree tests only
make bst_experiments  # Build Part A only
make os_experiments   # Build Part B only
```

## 📚 References
- CLRS 3rd Edition - Chapter 12 (BST)
- CLRS 3rd Edition - Chapter 14 (Augmenting Data Structures)