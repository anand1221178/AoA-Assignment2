#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "../include/bst.h"
#include "../include/os_tree.h"
#include "../include/utils.h"

#define MIN_SIZE 10
#define MAX_SIZE 100000
#define NUM_SIZES 80


int* generate_sizes(int* count) {
    int* sizes = (int*)malloc(NUM_SIZES * sizeof(int));
    int idx = 0;

    for (int i = 0; i < NUM_SIZES && idx < NUM_SIZES; i++) {
        int n = MIN_SIZE * pow(1.15, i);
        if (n > MAX_SIZE) {
            if (idx == 0 || sizes[idx-1] < MAX_SIZE) {
                sizes[idx++] = MAX_SIZE;
            }
            break;
        }
        sizes[idx++] = n;
    }

    *count = idx;
    return sizes;
}


void experiment_insert_comparison() {
    printf("\n=== Experiment 1: INSERT Time Comparison (OS-Tree vs BST) ===\n");
    printf("n,bst_time_ms,os_tree_time_ms,overhead_ratio\n");

    int size_count;
    int* sizes = generate_sizes(&size_count);

    for (int size_idx = 0; size_idx < size_count; size_idx++) {
        int n = sizes[size_idx];
        int num_trees = (n <= 100) ? 100 : (n <= 1000) ? 50 : (n <= 10000) ? 30 : 15;

   
        double final_bst_avg = 0.0;
        double final_os_avg = 0.0;

        for (int run = 0; run < 3; run++) {
            double bst_total = 0.0;
            double os_total = 0.0;

            for (int tree_num = 0; tree_num < num_trees; tree_num++) {
                int* keys = generate_sequence(n);
                fisher_yates(keys, n);

                // Timeing BST insertion
                Tree* bst_tree = create_tree();
                double bst_start = get_time_ms();

                for (int i = 0; i < n; i++) {
                    Node* node = create_node(keys[i]);
                    tree_insert(bst_tree, node);
                }

                double bst_end = get_time_ms();
                bst_total += (bst_end - bst_start);

                // Timeing os-tree
                OSTree* os_tree = os_create_tree();
                double os_start = get_time_ms();

                for (int i = 0; i < n; i++) {
                    OSNode* node = os_create_node(keys[i]);
                    os_tree_insert(os_tree, node);
                }

                double os_end = get_time_ms();
                os_total += (os_end - os_start);

                // Cleanup
                destroy_tree(bst_tree->root);
                free(bst_tree);
                os_destroy_tree(os_tree->root);
                free(os_tree);
                free(keys);
            }

            bst_total /= num_trees;
            os_total /= num_trees;
            final_bst_avg += bst_total;
            final_os_avg += os_total;
        }

        final_bst_avg /= 3.0;
        final_os_avg /= 3.0;
        double overhead = final_os_avg / final_bst_avg;

        printf("%d,%.4f,%.4f,%.3f\n", n, final_bst_avg, final_os_avg, overhead);
    }

    free(sizes);
}

void experiment_delete_comparison() {
    printf("\n=== Experiment 2: DELETE Time Comparison (OS-Tree vs BST) ===\n");
    printf("n,bst_time_ms,os_tree_time_ms,overhead_ratio\n");

    int size_count;
    int* sizes = generate_sizes(&size_count);

    for (int size_idx = 0; size_idx < size_count; size_idx++) {
        int n = sizes[size_idx];
        int num_trees = (n <= 100) ? 100 : (n <= 1000) ? 50 : (n <= 10000) ? 30 : 15;

        
        double final_bst_avg = 0.0;
        double final_os_avg = 0.0;

        for (int run = 0; run < 3; run++) {
            double bst_total = 0.0;
            double os_total = 0.0;

            for (int tree_num = 0; tree_num < num_trees; tree_num++) {
                int* keys = generate_sequence(n);
                fisher_yates(keys, n);

    
                Tree* bst_tree = create_tree();
                for (int i = 0; i < n; i++) {
                    Node* node = create_node(keys[i]);
                    tree_insert(bst_tree, node);
                }

                double bst_start = get_time_ms();
                while (bst_tree->root != NULL) {
                    Node* to_delete = bst_tree->root;
                    tree_delete(bst_tree, to_delete);
                    free(to_delete);
                }
                double bst_end = get_time_ms();
                bst_total += (bst_end - bst_start);

                OSTree* os_tree = os_create_tree();
                for (int i = 0; i < n; i++) {
                    OSNode* node = os_create_node(keys[i]);
                    os_tree_insert(os_tree, node);
                }

                double os_start = get_time_ms();
                while (os_tree->root != NULL) {
                    OSNode* to_delete = os_tree->root;
                    os_tree_delete(os_tree, to_delete);
                    free(to_delete);
                }
                double os_end = get_time_ms();
                os_total += (os_end - os_start);

                // Cleanup
                free(bst_tree);
                free(os_tree);
                free(keys);
            }

            bst_total /= num_trees;
            os_total /= num_trees;
            final_bst_avg += bst_total;
            final_os_avg += os_total;
        }

        final_bst_avg /= 3.0;
        final_os_avg /= 3.0;
        double overhead = final_os_avg / final_bst_avg;

        printf("%d,%.4f,%.4f,%.3f\n", n, final_bst_avg, final_os_avg, overhead);
    }

    free(sizes);
}

//os select
void experiment_os_select() {
    printf("\n=== Experiment 3: OS-SELECT Runtime ===\n");
    printf("n,avg_time_ms,time_per_operation_us\n");

    int size_count;
    int* sizes = generate_sizes(&size_count);

    for (int size_idx = 0; size_idx < size_count; size_idx++) {
        int n = sizes[size_idx];
        int num_trees = (n <= 1000) ? 20 : 10;
        int num_operations = 100;  


        double final_avg_time = 0.0;

        for (int run = 0; run < 3; run++) {
            double total_time = 0.0;

            for (int tree_num = 0; tree_num < num_trees; tree_num++) {
                int* keys = generate_sequence(n);
                fisher_yates(keys, n);


                OSTree* os_tree = os_create_tree();
                for (int i = 0; i < n; i++) {
                    OSNode* node = os_create_node(keys[i]);
                    os_tree_insert(os_tree, node);
                }

       
                double start = get_time_ms();
                for (int op = 0; op < num_operations; op++) {
                    int rank = random_range(1, n);
                    OSNode* result = os_select(os_tree->root, rank);
                    volatile int temp __attribute__((unused)) = result ? result->key : 0;
                }
                double end = get_time_ms();

                total_time += (end - start);

       
                os_destroy_tree(os_tree->root);
                free(os_tree);
                free(keys);
            }

            total_time /= num_trees;
            final_avg_time += total_time;
        }

        final_avg_time /= 3.0;
        double time_per_op = (final_avg_time * 1000.0) / num_operations;  // Convert to microseconds

        printf("%d,%.4f,%.3f\n", n, final_avg_time, time_per_op);
    }

    free(sizes);
}


void experiment_os_rank() {
    printf("\n=== Experiment 4: OS-RANK Runtime ===\n");
    printf("n,avg_time_ms,time_per_operation_us\n");

    int size_count;
    int* sizes = generate_sizes(&size_count);

    for (int size_idx = 0; size_idx < size_count; size_idx++) {
        int n = sizes[size_idx];
        int num_trees = (n <= 1000) ? 20 : 10;
        int num_operations = 100;  // Number of os rank calls per tree


        double final_avg_time = 0.0;

        for (int run = 0; run < 3; run++) {
            double total_time = 0.0;

            for (int tree_num = 0; tree_num < num_trees; tree_num++) {
                int* keys = generate_sequence(n);
                fisher_yates(keys, n);


                OSTree* os_tree = os_create_tree();
                OSNode** nodes = (OSNode**)malloc(n * sizeof(OSNode*));

                for (int i = 0; i < n; i++) {
                    nodes[i] = os_create_node(keys[i]);
                    os_tree_insert(os_tree, nodes[i]);
                }


                double start = get_time_ms();
                for (int op = 0; op < num_operations; op++) {
                    int idx = random_range(0, n - 1);
                    int rank = os_rank(os_tree, nodes[idx]);
                    volatile int temp __attribute__((unused)) = rank;
                }
                double end = get_time_ms();

                total_time += (end - start);

                // Cleanup
                os_destroy_tree(os_tree->root);
                free(os_tree);
                free(keys);
                free(nodes);
            }

            total_time /= num_trees;
            final_avg_time += total_time;
        }

        final_avg_time /= 3.0;
        double time_per_op = (final_avg_time * 1000.0) / num_operations;  // Convert to microseconds

        printf("%d,%.4f,%.3f\n", n, final_avg_time, time_per_op);
    }

    free(sizes);
}

int main(void) {
    srand(time(NULL));

    printf("Order-Statistic Tree Experiments (Part B)\n");
    printf("==========================================\n");
    printf("Comparing OS-Tree vs BST performance\n");
    printf("Size range: %d to %d\n", MIN_SIZE, MAX_SIZE);
    printf("Number of size points: up to %d\n", NUM_SIZES);
    printf("\nEstimated runtime: 5-15 minutes depending on CPU\n");
    printf("Progress will be shown below...\n");

    // Run all experiments
    experiment_insert_comparison();
    experiment_delete_comparison();
    experiment_os_select();
    experiment_os_rank();

    printf("\nAll OS-Tree experiments completed!\n");
    printf("Results can be plotted to show:\n");
    printf("  1. OS-Tree INSERT overhead vs BST\n");
    printf("  2. OS-Tree DELETE overhead vs BST\n");
    printf("  3. OS-SELECT runtime (should be O(log n))\n");
    printf("  4. OS-RANK runtime (should be O(log n))\n");

    return 0;
}
