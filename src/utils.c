#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../include/utils.h"

typedef struct {
    int value;
    int priority;
} SortPair;

int compare_pairs(const void* a, const void* b) {
    SortPair* pa = (SortPair*)a;
    SortPair* pb = (SortPair*)b;
    return pa->priority - pb->priority;
}

// permute by sorting
void permute_by_sorting(int* A, int n) {
    SortPair* P = (SortPair*)malloc(n * sizeof(SortPair));
    
    
    for (int i = 0; i < n; i++) {
        P[i].value = A[i];
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

// randomise in place
void randomize_in_place(int* A, int n) {
    for (int i = 0; i < n; i++) {
        // swap A[i] with A[RANDOM(i, n)]
        int j = random_range(i, n - 1);
        int temp = A[i];
        A[i] = A[j];
        A[j] = temp;
    }
}

//  Fisher-Yates shuffle 
void fisher_yates(int* A, int n) {
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        // Swap A[i] and A[j]
        int temp = A[i];
        A[i] = A[j];
        A[j] = temp;
    }
}

// No shuffle - sequential order 
void no_shuffle(int* A, int n) {

    // This creates a degenerate BST with height n-1
    (void)A;  // 
    (void)n;  //
}

// Generate sequence from 1 to n
int* generate_sequence(int n) {
    int* array = (int*)malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) {
        array[i] = i + 1;
    }
    return array;
}

//timer
double get_time_ms() {
    return ((double)clock() / CLOCKS_PER_SEC) * 1000.0;
}

//generate rand num
int random_range(int min, int max) {
    return min + rand() % (max - min + 1);
}