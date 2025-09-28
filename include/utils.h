#ifndef UTILS_H
#define UTILS_H

#include <time.h>

// Shuffling algorithms
void no_shuffle(int* A, int n);           // Sequential (worst case)
void fisher_yates(int* A, int n);         //  Fisher-Yates (backward)
void randomize_in_place(int* A, int n);   // RANDOMIZE-IN-PLACE from lecrure (forward)
void permute_by_sorting(int* A, int n);   // PERMUTE-BY-SORTING from lecture

// Generate array of integers from 1 to n
int* generate_sequence(int n);

// Timing functions
double get_time_ms();

// Random number generator
int random_range(int min, int max);

#endif