#!/bin/bash

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

echo -e "${BLUE}========================================${NC}"
echo -e "${BLUE}  BST & Order-Statistic Tree Assignment${NC}"
echo -e "${BLUE}========================================${NC}"
echo -e "${YELLOW}Configuration:${NC}"
echo -e "  • Tree sizes: 10 to 100,000 nodes"
echo -e "  • Data points: ~80 samples per experiment"
echo -e "  • Each size run: 3 times "
echo -e "  • Trees per run: 100-300 for small sizes, 15-30 for large sizes"
echo -e "${BLUE}========================================${NC}"

# Clean and compile
echo -e "\n${YELLOW}[1/8] Cleaning previous builds...${NC}"
make clean

echo -e "\n${YELLOW}[2/8] Compiling all implementations...${NC}"
make all

if [ $? -ne 0 ]; then
    echo -e "${RED}❌ Compilation failed!${NC}"
    exit 1
fi

echo -e "${GREEN}✓ Compilation successful!${NC}"

# Run Part A tests
echo -e "\n${BLUE}=== PART A: Binary Search Trees ===${NC}"
echo -e "\n${YELLOW}[3/8] Running BST basic tests...${NC}"
./bin/bst_test > /dev/null 2>&1

if [ $? -ne 0 ]; then
    echo -e "${RED}❌ BST tests failed!${NC}"
    exit 1
fi

echo -e "${GREEN}✓ BST tests passed!${NC}"

# Run Part A experiments
echo -e "\n${YELLOW}[4/8] Running BST experiments...${NC}"
./bin/bst_experiments > data/bst_results.csv

if [ $? -ne 0 ]; then
    echo -e "${RED}❌ BST experiments failed!${NC}"
    exit 1
fi

echo -e "${GREEN}✓ BST experiments completed!${NC}"
echo -e "${GREEN}  Data saved to data/bst_results.csv${NC}"

# Run Part B tests
echo -e "\n${BLUE}=== PART B: Order-Statistic Trees ===${NC}"
echo -e "\n${YELLOW}[5/8] Running OS-Tree basic tests...${NC}"
./bin/os_test > /dev/null 2>&1

if [ $? -ne 0 ]; then
    echo -e "${RED}❌ OS-Tree tests failed!${NC}"
    exit 1
fi

echo -e "${GREEN}✓ OS-Tree tests passed!${NC}"

# Run Part B experiments
echo -e "\n${YELLOW}[6/8] Running OS-Tree experiments...${NC}"
./bin/os_experiments > data/os_results.csv

if [ $? -ne 0 ]; then
    echo -e "${RED}❌ OS-Tree experiments failed!${NC}"
    exit 1
fi

echo -e "${GREEN}✓ OS-Tree experiments completed!${NC}"
echo -e "${GREEN}  Data saved to data/os_results.csv${NC}"

# Generate plots
echo -e "\n${YELLOW}[7/8] Generating plots for Part A...${NC}"
if [ -f "scripts/plot_bst.py" ]; then
    python scripts/plot_bst.py > /dev/null 2>&1
    if [ $? -ne 0 ]; then
        echo -e "${RED}❌ Part A plot generation failed!${NC}"
        echo -e "${YELLOW}Make sure matplotlib is installed: pip install matplotlib numpy pandas${NC}"
    else
        echo -e "${GREEN}✓ Part A plots generated!${NC}"
    fi
else
    echo -e "${YELLOW}⚠ scripts/plot_bst.py not found, skipping Part A plots${NC}"
fi

echo -e "\n${YELLOW}[8/8] Generating plots for Part B...${NC}"
if [ -f "scripts/plot_os.py" ]; then
    python scripts/plot_os.py > /dev/null 2>&1
    if [ $? -ne 0 ]; then
        echo -e "${RED}❌ Part B plot generation failed!${NC}"
        echo -e "${YELLOW}Make sure matplotlib is installed: pip install matplotlib numpy pandas${NC}"
    else
        echo -e "${GREEN}✓ Part B plots generated!${NC}"
    fi
else
    echo -e "${YELLOW}⚠ scripts/plot_os.py not found, skipping Part B plots${NC}"
fi

echo -e "\n${BLUE}========================================${NC}"
echo -e "${GREEN}✓ ALL EXPERIMENTS COMPLETED SUCCESSFULLY!${NC}"
echo -e "${BLUE}========================================${NC}"
echo -e "\n📊 Results:"
echo -e "  ${YELLOW}Raw Data:${NC}"
echo -e "    - data/bst_results.csv (Part A)"
echo -e "    - data/os_results.csv (Part B)"
echo -e "\n  ${YELLOW}Graphs:${NC}"
echo -e "    - Check the graphs/ directory for all plots"