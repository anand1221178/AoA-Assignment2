#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <limits.h>
#include <math.h>
#include "bst.h"
#include "utils.h"

// Experiment parameters
const int TREE_SIZES[] = {100, 500, 1000, 5000, 10000, 50000, 100000};
const int NUM_SIZES = 7;

// Number of repetitions for each size (more for smaller trees)
int get_repetitions(int size) {
    if (size <= 1000) return 1000;
    if (size <= 5000) return 500;
    if (size <= 10000) return 100;
    return 50;
}

// Experiment 1: Tree height analysis
void experiment_tree_heights() {
    printf("Running BST height experiments...\n");

    write_csv_header("data/bst_heights.csv", "size,avg_height,min_height,max_height");

    for (int s = 0; s < NUM_SIZES; s++) {
        int size = TREE_SIZES[s];
        int repetitions = get_repetitions(size);

        int total_height = 0;
        int min_height = INT_MAX;
        int max_height = 0;

        printf("Testing size %d with %d repetitions...\n", size, repetitions);

        for (int rep = 0; rep < repetitions; rep++) {
            // Create tree and shuffled keys
            Tree* tree = create_tree();
            int* keys = create_shuffled_array(size);

            // Build tree
            for (int i = 0; i < size; i++) {
                Node* node = create_node(keys[i]);
                tree_insert(tree, node);
            }

            // Measure height
            int height = tree_height(tree->root);
            total_height += height;
            if (height < min_height) min_height = height;
            if (height > max_height) max_height = height;

            // Cleanup
            destroy_tree(tree->root);
            free(tree);
            free(keys);
        }

        double avg_height = (double)total_height / repetitions;
        double data[] = {avg_height, (double)min_height, (double)max_height};
        append_csv_data("data/bst_heights.csv", size, data, 3);

        printf("Size %d: avg=%.2f, min=%d, max=%d\n",
               size, avg_height, min_height, max_height);
    }
    printf("Height experiments completed!\n\n");
}

// Experiment 2: Build time analysis
void experiment_build_times() {
    printf("Running BST build time experiments...\n");

    write_csv_header("data/bst_build_times.csv", "size,avg_build_time,min_build_time,max_build_time");

    for (int s = 0; s < NUM_SIZES; s++) {
        int size = TREE_SIZES[s];
        int repetitions = get_repetitions(size);

        double total_time = 0.0;
        double min_time = HUGE_VAL;
        double max_time = 0.0;

        printf("Testing size %d with %d repetitions...\n", size, repetitions);

        for (int rep = 0; rep < repetitions; rep++) {
            // Prepare shuffled keys
            int* keys = create_shuffled_array(size);
            Tree* tree = create_tree();

            // Time the build process
            double start_time = get_time();

            for (int i = 0; i < size; i++) {
                Node* node = create_node(keys[i]);
                tree_insert(tree, node);
            }

            double end_time = get_time();
            double build_time = time_diff(start_time, end_time);

            total_time += build_time;
            if (build_time < min_time) min_time = build_time;
            if (build_time > max_time) max_time = build_time;

            // Cleanup
            destroy_tree(tree->root);
            free(tree);
            free(keys);
        }

        double avg_time = total_time / repetitions;
        double data[] = {avg_time, min_time, max_time};
        append_csv_data("data/bst_build_times.csv", size, data, 3);

        printf("Size %d: avg=%.6f, min=%.6f, max=%.6f seconds\n",
               size, avg_time, min_time, max_time);
    }
    printf("Build time experiments completed!\n\n");
}

// Experiment 3: Delete time analysis (destroying tree by deleting root)
void experiment_delete_times() {
    printf("Running BST delete time experiments...\n");

    write_csv_header("data/bst_delete_times.csv", "size,avg_delete_time,min_delete_time,max_delete_time");

    for (int s = 0; s < NUM_SIZES; s++) {
        int size = TREE_SIZES[s];
        int repetitions = get_repetitions(size);

        double total_time = 0.0;
        double min_time = HUGE_VAL;
        double max_time = 0.0;

        printf("Testing size %d with %d repetitions...\n", size, repetitions);

        for (int rep = 0; rep < repetitions; rep++) {
            // Build tree first
            Tree* tree = create_tree();
            int* keys = create_shuffled_array(size);

            for (int i = 0; i < size; i++) {
                Node* node = create_node(keys[i]);
                tree_insert(tree, node);
            }

            // Time the deletion process (delete root repeatedly)
            double start_time = get_time();

            while (tree->root != NULL) {
                Node* root_to_delete = tree->root;
                tree_delete(tree, root_to_delete);
                free(root_to_delete);
            }

            double end_time = get_time();
            double delete_time = time_diff(start_time, end_time);

            total_time += delete_time;
            if (delete_time < min_time) min_time = delete_time;
            if (delete_time > max_time) max_time = delete_time;

            // Cleanup
            free(tree);
            free(keys);
        }

        double avg_time = total_time / repetitions;
        double data[] = {avg_time, min_time, max_time};
        append_csv_data("data/bst_delete_times.csv", size, data, 3);

        printf("Size %d: avg=%.6f, min=%.6f, max=%.6f seconds\n",
               size, avg_time, min_time, max_time);
    }
    printf("Delete time experiments completed!\n\n");
}

// Experiment 4: Inorder walk time analysis
void experiment_inorder_walk_times() {
    printf("Running Inorder-Tree-Walk time experiments...\n");

    write_csv_header("data/inorder_walk_times.csv", "size,avg_walk_time,min_walk_time,max_walk_time");

    for (int s = 0; s < NUM_SIZES; s++) {
        int size = TREE_SIZES[s];
        int repetitions = get_repetitions(size);

        double total_time = 0.0;
        double min_time = HUGE_VAL;
        double max_time = 0.0;

        printf("Testing size %d with %d repetitions...\n", size, repetitions);

        for (int rep = 0; rep < repetitions; rep++) {
            // Build tree
            Tree* tree = create_tree();
            int* keys = create_shuffled_array(size);

            for (int i = 0; i < size; i++) {
                Node* node = create_node(keys[i]);
                tree_insert(tree, node);
            }

            // Time the inorder walk
            double start_time = get_time();

            // Note: For timing, we'll modify inorder_walk to not print
            // or we could count operations instead of printing
            inorder_tree_walk_silent(tree->root);

            double end_time = get_time();
            double walk_time = time_diff(start_time, end_time);

            total_time += walk_time;
            if (walk_time < min_time) min_time = walk_time;
            if (walk_time > max_time) max_time = walk_time;

            // Cleanup
            destroy_tree(tree->root);
            free(tree);
            free(keys);
        }

        double avg_time = total_time / repetitions;
        double data[] = {avg_time, min_time, max_time};
        append_csv_data("data/inorder_walk_times.csv", size, data, 3);

        printf("Size %d: avg=%.6f, min=%.6f, max=%.6f seconds\n",
               size, avg_time, min_time, max_time);
    }
    printf("Inorder walk time experiments completed!\n\n");
}


int main() {
    printf("BST Experiments (Part A)\n");
    printf("========================\n\n");

    // Seed random number generator
    seed_random(time(NULL));

    // Run all experiments
    experiment_tree_heights();
    experiment_build_times();
    experiment_delete_times();
    experiment_inorder_walk_times();

    printf("All BST experiments completed!\n");
    printf("Results saved to data/ directory as CSV files.\n");
    printf("Run 'make graphs' to generate plots.\n");

    return 0;
}