#!/bin/bash

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

echo -e "${GREEN}BST Assignment - Part A${NC}"
echo "========================"

# Clean and compile
echo -e "\n${YELLOW}Cleaning previous builds...${NC}"
make clean

echo -e "\n${YELLOW}Compiling BST implementation...${NC}"
make all

if [ $? -ne 0 ]; then
    echo -e "${RED}Compilation failed!${NC}"
    exit 1
fi

# Run tests
echo -e "\n${YELLOW}Running basic tests...${NC}"
./bin/bst_test

if [ $? -ne 0 ]; then
    echo -e "${RED}Tests failed!${NC}"
    exit 1
fi

# Run experiments
echo -e "\n${YELLOW}Running experiments (this may take a few minutes)...${NC}"
./bin/bst_experiments > data/bst_results.csv

if [ $? -ne 0 ]; then
    echo -e "${RED}Experiments failed!${NC}"
    exit 1
fi

echo -e "${GREEN}Data saved to data/bst_results.csv${NC}"

# Generate plots
echo -e "\n${YELLOW}Generating plots...${NC}"
python3 scripts/plot_bst.py

if [ $? -ne 0 ]; then
    echo -e "${RED}Plot generation failed! Make sure matplotlib is installed.${NC}"
    echo "Install with: pip3 install matplotlib numpy"
    exit 1
fi

echo -e "\n${GREEN}All experiments completed successfully!${NC}"
echo -e "Check the ${YELLOW}graphs/${NC} directory for the generated plots."
echo -e "Check the ${YELLOW}data/${NC} directory for the raw CSV data."