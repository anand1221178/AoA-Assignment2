#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "../include/bst.h"
#include "../include/utils.h"

void test_basic_operations() {
    printf("=== Testing Basic BST Operations ===\n");
    
    // Create a tree
    Tree* T = create_tree();
    
    // Test data
    int keys[] = {15, 6, 18, 3, 7, 17, 20, 2, 4, 13, 9};
    int n = sizeof(keys) / sizeof(keys[0]);
    
    printf("Inserting keys: ");
    for (int i = 0; i < n; i++) {
        printf("%d ", keys[i]);
        Node* z = create_node(keys[i]);
        tree_insert(T, z);
    }
    printf("\n");
    
    // Test inorder walk
    printf("Inorder traversal: ");
    inorder_tree_walk(T->root);
    printf("\n");
    
    // Test tree height
    printf("Tree height: %d\n", tree_height(T->root));
    
    // Test search
    int search_key = 7;
    Node* found = tree_search(T->root, search_key);
    if (found) {
        printf("Search for %d: Found\n", search_key);
    } else {
        printf("Search for %d: Not found\n", search_key);
    }
    
    // Test minimum and maximum
    Node* min_node = tree_min(T->root);
    Node* max_node = tree_max(T->root);
    printf("Minimum: %d\n", min_node->key);
    printf("Maximum: %d\n", max_node->key);
    
    // Test successor
    Node* node_6 = tree_search(T->root, 6);
    if (node_6) {
        Node* successor = tree_successor(node_6);
        if (successor) {
            printf("Successor of 6: %d\n", successor->key);
        }
    }
    
    // Test deletion
    printf("\nDeleting key 6...\n");
    Node* to_delete = tree_search(T->root, 6);
    if (to_delete) {
        tree_delete(T, to_delete);
        free(to_delete);
    }
    
    printf("Inorder after deletion: ");
    inorder_tree_walk(T->root);
    printf("\n");
    
    // Clean up
    destroy_tree(T->root);
    free(T);
    
    printf("\n=== Basic tests completed ===\n\n");
}

void test_shuffle_methods() {
    printf("=== Testing Shuffle Methods ===\n");
    
    int n = 20;
    
    // Test all 4 shuffle methods
    printf("\n1. No Shuffle (Sequential):\n");
    int* keys1 = generate_sequence(n);
    no_shuffle(keys1, n);
    printf("First 10 keys: ");
    for (int i = 0; i < 10 && i < n; i++) {
        printf("%d ", keys1[i]);
    }
    printf("...\n");
    
    printf("\n2. Fisher-Yates:\n");
    int* keys2 = generate_sequence(n);
    fisher_yates(keys2, n);
    printf("First 10 shuffled keys: ");
    for (int i = 0; i < 10 && i < n; i++) {
        printf("%d ", keys2[i]);
    }
    printf("...\n");
    
    printf("\n3. RANDOMIZE-IN-PLACE:\n");
    int* keys3 = generate_sequence(n);
    randomize_in_place(keys3, n);
    printf("First 10 shuffled keys: ");
    for (int i = 0; i < 10 && i < n; i++) {
        printf("%d ", keys3[i]);
    }
    printf("...\n");
    
    printf("\n4. PERMUTE-BY-SORTING:\n");
    int* keys4 = generate_sequence(n);
    permute_by_sorting(keys4, n);
    printf("First 10 shuffled keys: ");
    for (int i = 0; i < 10 && i < n; i++) {
        printf("%d ", keys4[i]);
    }
    printf("...\n");
    
    // Build a tree with one of the methods (RANDOMIZE-IN-PLACE)
    printf("\nBuilding tree with RANDOMIZE-IN-PLACE...\n");
    Tree* T = create_tree();
    for (int i = 0; i < n; i++) {
        Node* z = create_node(keys3[i]);
        tree_insert(T, z);
    }
    
    printf("First 10 elements (inorder): ");
    inorder_tree_walk_limit(T->root, 10);
    printf("...\n");
    
    printf("Tree height: %d (expected ~%.1f for random BST)\n", 
           tree_height(T->root), 2.0 * log2(n));
    
    // Random deletions
    printf("\nPerforming 5 random deletions...\n");
    for (int i = 0; i < 5 && T->root != NULL; i++) {
        int key_to_delete = keys3[i];
        Node* node = tree_search(T->root, key_to_delete);
        if (node) {
            printf("Deleting %d\n", key_to_delete);
            tree_delete(T, node);
            free(node);
        }
    }
    
    printf("Tree after deletions (first 10): ");
    inorder_tree_walk_limit(T->root, 10);
    printf("...\n");
    
    // Clean up
    destroy_tree(T->root);
    free(T);
    free(keys1);
    free(keys2);
    free(keys3);
    free(keys4);
    
    printf("\n=== Shuffle tests completed ===\n");
}

int main() {
    printf("Binary Search Tree Implementation Test\n");
    printf("======================================\n\n");
    
    test_basic_operations();
    test_shuffle_methods();
    
    printf("\nAll tests completed successfully!\n");
    return 0;
}