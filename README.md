# Binary Search Trees and Order-Statistic Trees Assignment ✅ COMPLETED

**Course:** Applications of Algorithms
**Due:** Monday 13th October at 23h00
**Implementation:** C with Python plotting
**Status:** ✅ All experiments completed, report finalized

## Project Overview

This assignment implements and analyzes:
- **Part A:** Binary Search Trees (BST) - Performance analysis with 4 shuffle methods
- **Part B:** Order-Statistic Trees - Augmented BST with rank operations

All algorithms coded from scratch following CLRS 3rd Edition (Chapters 12 & 14).

### Key Features
- ✅ **Triple averaging** for maximum accuracy (each size run 3 times)
- ✅ **80 data points** with exponential spacing (1.15× growth factor)
- ✅ **8 comprehensive graphs** with proper analyses
- ✅ **Uniform random permutation validation** across 3 CLRS shuffle algorithms
- ✅ **Complete 15-page report** with corrected counterintuitive results

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
- **C Compiler:** GCC 15.1.0+ (macOS/Linux) or MinGW (Windows)
- **Python 3:** With matplotlib, numpy, pandas
- **Platform:** Tested on Apple M4 Pro, macOS, 24GB RAM

```bash
# Install Python dependencies
pip install matplotlib numpy pandas
```

### Run Everything (One Command!)
```bash
# Make script executable (first time only)
chmod +x run.sh

# Run all experiments and generate graphs (2-3 hours with triple averaging)
./run.sh
```

**That's it!** The `run.sh` script handles everything:
1. ✅ Cleans previous builds and data
2. ✅ Compiles all code with optimizations (-O2)
3. ✅ Runs BST tests with correctness validation
4. ✅ Runs Part A experiments (~1-1.5 hours)
5. ✅ Runs OS-Tree tests with correctness validation
6. ✅ Runs Part B experiments (~1-1.5 hours)
7. ✅ Generates all 8 graphs automatically
8. ✅ Displays completion summary

**No Makefile needed** - `run.sh` compiles and runs everything directly!

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
- **Tree sizes:** 10 to 100,000 nodes
- **Data points:** 80 samples (exponential spacing, 1.15× growth)
- **Triple averaging:** Each size run 3 times for maximum accuracy
- **Trees per run:** 100-300 for small sizes (n ≤ 100), 15-30 for large sizes (n > 10,000)
- **Shuffle methods:**
  - Fisher-Yates (Modern optimal shuffle)
  - RANDOMIZE-IN-PLACE (CLRS Algorithm 5.3)
  - PERMUTE-BY-SORTING (CLRS Algorithm 5.2)
  - No-Shuffle (Sorted baseline for worst case)

**Why 3 shuffle methods?** Validates that **uniform random permutations** (not the specific algorithm) determine BST performance, removing shuffle choice as an experimental variable.

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

### Part A Graphs Generated (4 individual comparisons)
1. ✅ **height_comparison.png** - Height vs n (confirms O(log n) ≈ 2.99 ln n)
2. ✅ **build_time_comparison.png** - Build time vs n (confirms O(n log n))
3. ✅ **destroy_time_comparison.png** - Delete time vs n (O(n) for chain, O(n log n) for random)
4. ✅ **inorder_walk_comparison.png** - Walk time vs n (confirms Θ(n), chain faster due to cache locality)

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
1. ✅ **insert_comparison.png** - OS-Tree vs BST insert (negligible overhead, ±8%)
2. ✅ **delete_comparison.png** - OS-Tree vs BST delete (8-10% overhead)
3. ✅ **os_select_runtime.png** - OS-SELECT runtime (confirms O(log n))
4. ✅ **os_rank_runtime.png** - OS-RANK runtime (confirms O(log n))



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