#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "../include/bst.h"
#include "../include/utils.h"

#define MAX_TREES 50      // Number of trees to build for each size
#define MIN_SIZE 100      // Minimum tree size
#define MAX_SIZE 50000    // Maximum tree size (safe for all operations)
#define NUM_SIZES 18      // Number of different sizes to test
#define MAX_SEQUENTIAL_SIZE 10000  // Maximum size for sequential (worst-case) trees

typedef enum {
    SHUFFLE_NONE,
    SHUFFLE_FISHER_YATES,
    SHUFFLE_RANDOMIZE_INPLACE,
    SHUFFLE_PERMUTE_SORT
} ShuffleMethod;

const char* get_method_name(ShuffleMethod method) {
    switch(method) {
        case SHUFFLE_NONE: return "NoShuffle";
        case SHUFFLE_FISHER_YATES: return "FisherYates";
        case SHUFFLE_RANDOMIZE_INPLACE: return "RandomizeInPlace";
        case SHUFFLE_PERMUTE_SORT: return "PermuteBySorting";
        default: return "Unknown";
    }
}

void apply_shuffle(int* keys, int n, ShuffleMethod method) {
    switch(method) {
        case SHUFFLE_NONE:
            no_shuffle(keys, n);
            break;
        case SHUFFLE_FISHER_YATES:
            fisher_yates(keys, n);
            break;
        case SHUFFLE_RANDOMIZE_INPLACE:
            randomize_in_place(keys, n);
            break;
        case SHUFFLE_PERMUTE_SORT:
            permute_by_sorting(keys, n);
            break;
    }
}

// Run all experiments for a specific shuffle method
void run_experiments_for_method(ShuffleMethod method) {
    const char* method_name = get_method_name(method);
    
    // Experiment 1: Height
    printf("\n=== %s: Height Experiment ===\n", method_name);
    printf("n,avg_height\n");
    
    for (int size_idx = 0; size_idx < NUM_SIZES; size_idx++) {
        int n = MIN_SIZE * pow(2, size_idx * 0.4);
        if (n > MAX_SIZE) n = MAX_SIZE;
        
        // Skip very large sequential trees to avoid stack overflow
        if (method == SHUFFLE_NONE && n > MAX_SEQUENTIAL_SIZE) {
            printf("%d,%.2f\n", n, (double)(n - 1));  // Height is n-1 for sequential
            continue;
        }
        
        // Adjust number of trees based on size
        int num_trees;
        if (method == SHUFFLE_NONE) {
            num_trees = (n > 1000) ? 1 : 5;
        } else {
            if (n <= 1000) num_trees = MAX_TREES;
            else if (n <= 10000) num_trees = 30;
            else num_trees = 10;
        }
        
        double avg_height = 0;
        for (int tree_num = 0; tree_num < num_trees; tree_num++) {
            int* keys = generate_sequence(n);
            apply_shuffle(keys, n, method);
            
            Tree* T = create_tree();
            for (int i = 0; i < n; i++) {
                Node* z = create_node(keys[i]);
                tree_insert(T, z);
            }
            
            avg_height += tree_height(T->root);
            
            destroy_tree(T->root);
            free(T);
            free(keys);
        }
        avg_height /= num_trees;
        printf("%d,%.2f\n", n, avg_height);
    }
    
    // Experiment 2: Build Time
    printf("\n=== %s: Build Time Experiment ===\n", method_name);
    printf("n,avg_time_ms\n");
    
    for (int size_idx = 0; size_idx < NUM_SIZES; size_idx++) {
        int n = MIN_SIZE * pow(2, size_idx * 0.4);
        if (n > MAX_SIZE) n = MAX_SIZE;
        
        // Skip very large sequential trees
        if (method == SHUFFLE_NONE && n > MAX_SEQUENTIAL_SIZE) {
            continue;
        }
        
        int num_trees;
        if (method == SHUFFLE_NONE) {
            num_trees = (n > 1000) ? 1 : 5;
        } else {
            if (n <= 1000) num_trees = 30;
            else if (n <= 10000) num_trees = 20;
            else num_trees = 5;
        }
        
        double avg_time = 0;
        for (int tree_num = 0; tree_num < num_trees; tree_num++) {
            int* keys = generate_sequence(n);
            apply_shuffle(keys, n, method);
            
            Tree* T = create_tree();
            
            double start_time = get_time_ms();
            for (int i = 0; i < n; i++) {
                Node* z = create_node(keys[i]);
                tree_insert(T, z);
            }
            double end_time = get_time_ms();
            
            avg_time += (end_time - start_time);
            
            destroy_tree(T->root);
            free(T);
            free(keys);
        }
        avg_time /= num_trees;
        printf("%d,%.4f\n", n, avg_time);
    }
    
    // Experiment 3: Destroy Time
    printf("\n=== %s: Destroy Time Experiment ===\n", method_name);
    printf("n,avg_time_ms\n");
    
    for (int size_idx = 0; size_idx < NUM_SIZES; size_idx++) {
        int n = MIN_SIZE * pow(2, size_idx * 0.4);
        if (n > MAX_SIZE) n = MAX_SIZE;
        
        // Skip very large sequential trees
        if (method == SHUFFLE_NONE && n > MAX_SEQUENTIAL_SIZE) {
            continue;
        }
        
        int num_trees;
        if (method == SHUFFLE_NONE) {
            num_trees = (n > 1000) ? 1 : 5;
        } else {
            if (n <= 1000) num_trees = 30;
            else if (n <= 10000) num_trees = 20;
            else num_trees = 5;
        }
        
        double avg_time = 0;
        for (int tree_num = 0; tree_num < num_trees; tree_num++) {
            int* keys = generate_sequence(n);
            apply_shuffle(keys, n, method);
            
            Tree* T = create_tree();
            for (int i = 0; i < n; i++) {
                Node* z = create_node(keys[i]);
                tree_insert(T, z);
            }
            
            double start_time = get_time_ms();
            while (T->root != NULL) {
                Node* to_delete = T->root;
                tree_delete(T, to_delete);
                free(to_delete);
            }
            double end_time = get_time_ms();
            
            avg_time += (end_time - start_time);
            
            free(T);
            free(keys);
        }
        avg_time /= num_trees;
        printf("%d,%.4f\n", n, avg_time);
    }
    
    // Experiment 4: Inorder Walk
    printf("\n=== %s: Inorder Walk Experiment ===\n", method_name);
    printf("n,total_time_ms,time_per_node_ms\n");
    
    for (int size_idx = 0; size_idx < NUM_SIZES; size_idx++) {
        int n = MIN_SIZE * pow(2, size_idx * 0.4);
        if (n > MAX_SIZE) n = MAX_SIZE;
        
        // Skip very large sequential trees
        if (method == SHUFFLE_NONE && n > MAX_SEQUENTIAL_SIZE) {
            continue;
        }
        
        int num_trees = (method == SHUFFLE_NONE && n > 1000) ? 2 : 10;
        
        double total_time = 0;
        
        for (int tree_num = 0; tree_num < num_trees; tree_num++) {
            int* keys = generate_sequence(n);
            apply_shuffle(keys, n, method);
            
            Tree* T = create_tree();
            for (int i = 0; i < n; i++) {
                Node* z = create_node(keys[i]);
                tree_insert(T, z);
            }
            
            // Multiple runs for smaller trees to get measurable time
            int runs = (n < 1000) ? 100 : (n < 10000) ? 10 : 1;
            double start_time = get_time_ms();
            for (int r = 0; r < runs; r++) {
                inorder_tree_walk_silent(T->root);
            }
            double end_time = get_time_ms();
            
            total_time += (end_time - start_time) / runs;
            
            destroy_tree(T->root);
            free(T);
            free(keys);
        }
        
        double avg_total_time = total_time / num_trees;
        double time_per_node = avg_total_time / n;
        printf("%d,%.6f,%.8f\n", n, avg_total_time, time_per_node);
    }
}

// Comparison experiments - all methods together
void run_comparison_experiments() {
    printf("\n=== COMPARISON: All Four Methods ===\n");
    
    // Height comparison
    printf("\n=== Height Comparison ===\n");
    printf("n,no_shuffle,fisher_yates,randomize_inplace,permute_sort\n");
    
    for (int size_idx = 0; size_idx < NUM_SIZES; size_idx++) {
        int n = MIN_SIZE * pow(2, size_idx * 0.4);
        if (n > MAX_SIZE) n = MAX_SIZE;
        
        printf("%d", n);
        
        for (ShuffleMethod method = SHUFFLE_NONE; method <= SHUFFLE_PERMUTE_SORT; method++) {
            // Skip very large sequential trees
            if (method == SHUFFLE_NONE && n > MAX_SEQUENTIAL_SIZE) {
                printf(",%.2f", (double)(n - 1));
                continue;
            }
            
            int num_trees = (method == SHUFFLE_NONE) ? 1 : 20;
            
            double avg_height = 0;
            for (int tree_num = 0; tree_num < num_trees; tree_num++) {
                int* keys = generate_sequence(n);
                apply_shuffle(keys, n, method);
                
                Tree* T = create_tree();
                for (int i = 0; i < n; i++) {
                    Node* z = create_node(keys[i]);
                    tree_insert(T, z);
                }
                
                avg_height += tree_height(T->root);
                
                destroy_tree(T->root);
                free(T);
                free(keys);
            }
            avg_height /= num_trees;
            printf(",%.2f", avg_height);
        }
        printf("\n");
    }
    
    // Build time comparison
    printf("\n=== Build Time Comparison ===\n");
    printf("n,no_shuffle,fisher_yates,randomize_inplace,permute_sort\n");
    
    for (int size_idx = 0; size_idx < NUM_SIZES; size_idx++) {
        int n = MIN_SIZE * pow(2, size_idx * 0.4);
        if (n > MAX_SIZE) n = MAX_SIZE;
        
        printf("%d", n);
        
        for (ShuffleMethod method = SHUFFLE_NONE; method <= SHUFFLE_PERMUTE_SORT; method++) {
            // Skip very large sequential trees
            if (method == SHUFFLE_NONE && n > MAX_SEQUENTIAL_SIZE) {
                printf(",0.0000");
                continue;
            }
            
            int num_trees = (method == SHUFFLE_NONE && n > 1000) ? 1 : 10;
            
            double avg_time = 0;
            for (int tree_num = 0; tree_num < num_trees; tree_num++) {
                int* keys = generate_sequence(n);
                apply_shuffle(keys, n, method);
                
                Tree* T = create_tree();
                
                double start_time = get_time_ms();
                for (int i = 0; i < n; i++) {
                    Node* z = create_node(keys[i]);
                    tree_insert(T, z);
                }
                double end_time = get_time_ms();
                
                avg_time += (end_time - start_time);
                
                destroy_tree(T->root);
                free(T);
                free(keys);
            }
            avg_time /= num_trees;
            printf(",%.4f", avg_time);
        }
        printf("\n");
    }
}

int main(void) {
    srand(time(NULL));
    
    printf("BST Experiments - Comparing Four Shuffling Methods\n");
    printf("===================================================\n");
    printf("1. No Shuffle (Sequential) - Worst Case O(n) height\n");
    printf("2. Fisher-Yates (Classic) - Random Case O(log n) height\n");
    printf("3. RANDOMIZE-IN-PLACE (CLRS) - Random Case O(log n) height\n");
    printf("4. PERMUTE-BY-SORTING (CLRS) - Random Case O(log n) height\n");
    printf("\nSize range: %d to %d\n", MIN_SIZE, MAX_SIZE);
    printf("Sequential trees limited to %d nodes (stack safety)\n", MAX_SEQUENTIAL_SIZE);
    printf("Number of size points: %d\n", NUM_SIZES);
    printf("\nEstimated runtime: 2-10 minutes depending on CPU\n");
    printf("Progress will be shown below...\n");
    
    // Run individual experiments for each method
    for (ShuffleMethod method = SHUFFLE_NONE; method <= SHUFFLE_PERMUTE_SORT; method++) {
        printf("\n[%d/4] Running %s experiments...\n", method + 1, get_method_name(method));
        run_experiments_for_method(method);
    }
    
    // Run comparison experiments
    printf("\n[5/5] Running comparison experiments...\n");
    run_comparison_experiments();
    
    printf("\nAll experiments completed!\n");
    
    return 0;
}