#!/bin/bash

echo "BST and Order-Statistic Tree Assignment Runner"
echo "=============================================="

# Create necessary directories
mkdir -p data obj bin

echo "Building project..."
make clean
make all

if [ $? -ne 0 ]; then
    echo "Build failed! Please check for compilation errors."
    exit 1
fi

echo "Build successful!"
echo ""

echo "Running Part A: BST Experiments..."
echo "This will test:"
echo "  - Tree heights for randomly built BSTs"
echo "  - Time to build BSTs (cumulative insertion time)"
echo "  - Time to destroy BSTs (by deleting root repeatedly)"
echo "  - Inorder-Tree-Walk runtime (confirming Θ(n))"
echo ""

./bin/bst_experiments

if [ $? -ne 0 ]; then
    echo "BST experiments failed!"
    exit 1
fi

echo ""
echo "Running Part B: Order-Statistic Tree Experiments..."
echo "This will test:"
echo "  - OS-Select and OS-Rank operations"
echo "  - Compare Tree-Insert/Delete times: OS-Tree vs BST"
echo ""

./bin/os_experiments

if [ $? -ne 0 ]; then
    echo "OS-Tree experiments failed!"
    exit 1
fi

echo ""
echo "All experiments completed successfully!"
echo ""

echo "Data files generated:"
ls -la data/*.csv

echo ""
echo "Generating graphs..."

if command -v python3 &> /dev/null; then
    python3 scripts/plot_graphs.py
    echo "Graphs generated in the 'graphs' directory!"
else
    echo "Python3 not found. Please install Python3 and matplotlib to generate graphs."
    echo "Or run: python3 scripts/plot_graphs.py"
fi

echo ""
echo "Assignment completed!"
echo "Submit:"
echo "  1. All source code (.c and .h files)"
echo "  2. Generated graphs and experiment description"
echo "  3. Explanation of size attribute maintenance in OS-Trees"