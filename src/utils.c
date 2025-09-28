#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../include/utils.h"

// Comparison function for qsort (used in PERMUTE-BY-SORTING)
typedef struct {
    int value;
    int priority;
} SortPair;

int compare_pairs(const void* a, const void* b) {
    SortPair* pa = (SortPair*)a;
    SortPair* pb = (SortPair*)b;
    return pa->priority - pb->priority;
}

// PERMUTE-BY-SORTING from CLRS
void permute_by_sorting(int* A, int n) {
    SortPair* P = (SortPair*)malloc(n * sizeof(SortPair));
    
    // Create array P with random priorities
    for (int i = 0; i < n; i++) {
        P[i].value = A[i];
        // RANDOM(1, n^3) - using n^3 to minimize probability of duplicates
        P[i].priority = random_range(1, n * n * n);
    }
    
    // Sort using P as sort keys
    qsort(P, n, sizeof(SortPair), compare_pairs);
    
    // Copy sorted values back to A
    for (int i = 0; i < n; i++) {
        A[i] = P[i].value;
    }
    
    free(P);
}

// RANDOMIZE-IN-PLACE from CLRS (forward Fisher-Yates)
void randomize_in_place(int* A, int n) {
    for (int i = 0; i < n; i++) {
        // swap A[i] with A[RANDOM(i, n)]
        int j = random_range(i, n - 1);
        int temp = A[i];
        A[i] = A[j];
        A[j] = temp;
    }
}

// Classic Fisher-Yates shuffle (backward version)
void fisher_yates(int* A, int n) {
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        // Swap A[i] and A[j]
        int temp = A[i];
        A[i] = A[j];
        A[j] = temp;
    }
}

// No shuffle - sequential order (worst case for BST)
void no_shuffle(int* A, int n) {
    // Array remains in sequential order 1, 2, 3, ..., n
    // This creates a degenerate BST with height n-1
    (void)A;  // Suppress unused parameter warning
    (void)n;  // Suppress unused parameter warning
}

// Generate sequence from 1 to n
int* generate_sequence(int n) {
    int* array = (int*)malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) {
        array[i] = i + 1;
    }
    return array;
}

// Get current time in milliseconds
double get_time_ms() {
    return ((double)clock() / CLOCKS_PER_SEC) * 1000.0;
}

// Generate random number in range [min, max]
int random_range(int min, int max) {
    return min + rand() % (max - min + 1);
}