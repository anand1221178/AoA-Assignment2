#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../include/bst.h"
// #include "../include/utils.h"

// Demonstration of BST operations
void demo_bst() {
    printf("=== Binary Search Tree Demo ===\n\n");
    
    Tree* tree = create_tree();
    int keys[] = {15, 6, 18, 3, 7, 17, 20, 2, 4, 13, 9};
    int n = sizeof(keys) / sizeof(keys[0]);
    
    printf("Inserting keys: ");
    for (int i = 0; i < n; i++) {
        printf("%d ", keys[i]);
        Node* node = create_node(keys[i]);
        tree_insert(tree, node);
    }
    printf("\n");
    
    printf("Inorder traversal (sorted): ");
    inorder_tree_walk(tree->root);
    printf("\n");
    
    printf("Tree height: %d\n", tree_height(tree->root));
    
    // Test search
    int search_key = 13;
    Node* found = tree_search(tree->root, search_key);
    printf("Search for %d: %s\n", search_key, found ? "Found" : "Not found");
    
    // Test min/max
    Node* min_node = tree_min(tree->root);
    Node* max_node = tree_max(tree->root);
    printf("Minimum key: %d\n", min_node->key);
    printf("Maximum key: %d\n", max_node->key);
    
    // Test deletion
    printf("\nDeleting key 6...\n");
    Node* to_delete = tree_search(tree->root, 6);
    if (to_delete) {
        tree_delete(tree, to_delete);
        free(to_delete);
        printf("After deletion: ");
        inorder_tree_walk(tree->root);
        printf("\n");
    }
    
    // Clean up
    destroy_tree(tree->root);
    free(tree);
    printf("\n");
}

// Quick performance test
void quick_performance_test() {
    printf("=== Quick Performance Test ===\n\n");
    
    int test_size = 1000;
    printf("Building BST with %d random keys...\n", test_size);
    
    Tree* tree = create_tree();
    int* keys = create_shuffled_array(test_size);
    
    // Time the build
    double start = get_time();
    for (int i = 0; i < test_size; i++) {
        Node* node = create_node(keys[i]);
        tree_insert(tree, node);
    }
    double end = get_time();
    
    printf("Build time: %.6f seconds\n", time_diff(start, end));
    printf("Tree height: %d (expected ~%.1f for balanced)\n", 
           tree_height(tree->root), 2.0 * log2(test_size));
    
    // Time inorder walk
    start = get_time();
    inorder_tree_walk_silent(tree->root);
    end = get_time();
    printf("Inorder walk time: %.6f seconds\n", time_diff(start, end));
    
    // Clean up
    destroy_tree(tree->root);
    free(tree);
    free(keys);
    printf("\n");
}

int main() {
    printf("\nBST and OS-Tree Implementation Test\n");
    printf("====================================\n\n");
    
    // Seed random number generator
    seed_random(time(NULL));
    
    // Run demonstrations
    demo_bst();
    quick_performance_test();
    
    printf("Basic tests completed successfully!\n");
    printf("Run './bin/bst_experiments' for full Part A experiments\n");
    printf("Run './bin/os_experiments' for full Part B experiments\n\n");
    
    return 0;
}