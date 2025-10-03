#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "../include/os_tree.h"
#include "../include/utils.h"

int main(void) {
    srand(time(NULL));

    printf("Order-Statistic Tree Test Program\n");
    printf("==================================\n\n");

    // Test 1: Basic operations
    printf("Test 1: Building OS-Tree with keys: 15, 6, 18, 3, 7, 17, 20, 2, 4, 13, 9\n");

    OSTree* T = os_create_tree();
    int keys[] = {15, 6, 18, 3, 7, 17, 20, 2, 4, 13, 9};
    int n = sizeof(keys) / sizeof(keys[0]);

    OSNode* nodes[11];  // Keep track of nodes for testing OS-Rank

    for (int i = 0; i < n; i++) {
        nodes[i] = os_create_node(keys[i]);
        os_tree_insert(T, nodes[i]);
    }

    printf("Inorder traversal (key and size): ");
    os_inorder_tree_walk(T->root);
    printf("\n\n");

    // Test 2: OS-Select
    printf("Test 2: OS-SELECT operations\n");
    printf("%-15s %-15s %-15s\n", "i (rank)", "Expected", "OS-Select(i)");
    printf("-----------------------------------------------\n");

    int sorted[] = {2, 3, 4, 6, 7, 9, 13, 15, 17, 18, 20};
    for (int i = 1; i <= n; i++) {
        OSNode* result = os_select(T->root, i);
        printf("%-15d %-15d %-15d", i, sorted[i-1], result ? result->key : -1);
        if (result && result->key == sorted[i-1]) {
            printf(" ✓\n");
        } else {
            printf(" ✗\n");
        }
    }
    printf("\n");

    // Test 3: OS-Rank
    printf("Test 3: OS-RANK operations\n");
    printf("%-15s %-15s %-15s\n", "Key", "Expected Rank", "OS-Rank");
    printf("-----------------------------------------------\n");

    for (int i = 0; i < n; i++) {
        int rank = os_rank(T, nodes[i]);
        int expected_rank = -1;
        for (int j = 0; j < n; j++) {
            if (sorted[j] == keys[i]) {
                expected_rank = j + 1;
                break;
            }
        }
        printf("%-15d %-15d %-15d", keys[i], expected_rank, rank);
        if (rank == expected_rank) {
            printf(" ✓\n");
        } else {
            printf(" ✗\n");
        }
    }
    printf("\n");

    // Test 4: Size attribute verification
    printf("Test 4: Verify size attribute\n");
    printf("Root node (key=%d) should have size=%d: actual size=%d ",
           T->root->key, n, T->root->size);
    if (T->root->size == n) {
        printf("✓\n");
    } else {
        printf("✗\n");
    }
    printf("\n");

    // Test 5: Delete and verify size updates
    printf("Test 5: Delete operations and size maintenance\n");
    printf("Deleting key 6...\n");

    OSNode* to_delete = os_tree_search(T->root, 6);
    if (to_delete) {
        os_tree_delete(T, to_delete);
        free(to_delete);
    }

    printf("After deletion, inorder traversal: ");
    os_inorder_tree_walk(T->root);
    printf("\n");
    printf("Root size should be %d: actual=%d ", n-1, T->root->size);
    if (T->root->size == n-1) {
        printf("✓\n");
    } else {
        printf("✗\n");
    }
    printf("\n");

    // Test 6: OS-Select after deletion
    printf("Test 6: OS-SELECT after deletion (5th smallest should be 9)\n");
    OSNode* fifth = os_select(T->root, 5);
    printf("OS-Select(5) = %d ", fifth ? fifth->key : -1);
    if (fifth && fifth->key == 9) {
        printf("✓\n");
    } else {
        printf("✗\n");
    }

    printf("\n");
    printf("All tests completed!\n");

    // Cleanup
    os_destroy_tree(T->root);
    free(T);

    return 0;
}
