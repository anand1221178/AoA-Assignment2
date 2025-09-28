#include "stdio.h"
#include "stdlib.h"
#include "../include/bst.h"

//Create node with given key val
Node* create_node(int key){
    Node* z = (Node*)malloc(sizeof(Node)); //mem alloc for new code
    z->key = key; // set key val for node
    z->left = NULL; // left child null
    z->right = NULL; // right child null
    z->p = NULL; // parent null
    return z; // ptr back to new node
}

//make empty tree for holding the ndoes
Tree* create_tree(void){
    Tree* T = (Tree*)malloc(sizeof(Tree)); //mem aloc for new tree
    T->root = NULL; // root is null
    return T;
}

void tree_insert(Tree* T, Node* z){
    Node* y = NULL;
    Node* x = T->root;

    while(x != NULL){
        y=x;
        if(z->key < x->key){
            x = x->left;
        }
        else{
            x = x->right;
        }
    }

    z->p = y;
    if (y == NULL){
        T->root = z;
    }
    else if (z->key < y->key){
        y->left = z;
    }
    else{
        y->right = z;
    }
}

void transplant(Tree* T, Node* u, Node* v) {
    if (u->p == NULL)         
        T->root = v;          
    else if (u == u->p->left) 
        u->p->left = v;      
    else                     
        u->p->right = v;      

    if (v != NULL)            
        v->p = u->p;          
}

void tree_delete(Tree* T, Node* z) {
    if (z->left == NULL)                    // Case 1: z has no left child
        transplant(T, z, z->right);         // Replace z with its right child
    else if (z->right == NULL)              // Case 2: z has no right child
        transplant(T, z, z->left);          // Replace z with its left child
    else {                                  // Case 3: z has two children
        Node* y = tree_min(z->right);   // Find successor (min in right subtree)
        if (y->p != z) {                    // If successor is not direct child
            transplant(T, y, y->right);     // Replace y with its right child
            y->right = z->right;            // Take over z's right subtree
            y->right->p = y;                // Update parent pointer
        }
        transplant(T, z, y);                // Replace z with y
        y->left = z->left;                  // Take over z's left subtree
        y->left->p = y;                     // Update parent pointer
    }
    // TODO: Actual memory freeing should be done after this function
}

// INORDER-TREE-WALK(x) - Exactly from textbook
void inorder_tree_walk(Node* x) {
    if (x != NULL) {                    // Line 1: if x ≠ NIL
        inorder_tree_walk(x->left);     // Line 2: INORDER-TREE-WALK(x.left)
        printf("%d ", x->key);          // Line 3: print x.key
        inorder_tree_walk(x->right);    // Line 4: INORDER-TREE-WALK(x.right)
    }
}

// TREE-MINIMUM(x) - Find node with minimum key
Node* tree_min(Node* x) {
    while (x->left != NULL)    // Keep going left
        x = x->left;           // Move to left child
    return x;                  // Return leftmost node (minimum)
}

// TREE-MAXIMUM(x) - Find node with maximum key
Node* tree_max(Node* x) {
    while (x->right != NULL)   // Keep going right
        x = x->right;          // Move to right child
    return x;                  // Return rightmost node (maximum)
}

// TREE-SEARCH(x, k) - Search for key k
Node* tree_search(Node* x, int k) {
    if (x == NULL || k == x->key)   // Base case: not found or found
        return x;                    // Return NULL or found node
    if (k < x->key)                  // If k less than current key
        return tree_search(x->left, k);  // Search left subtree
    else                             // If k greater than current key
        return tree_search(x->right, k); // Search right subtree
}

// TREE-SUCCESSOR(x) - Find successor of node x
Node* tree_successor(Node* x) {
    if (x->right != NULL)                  // If right subtree exists
        return tree_min(x->right);     // Successor is min in right subtree
    Node* y = x->p;                        // Start with parent
    while (y != NULL && x == y->right) {   // Go up until we turn left
        x = y;                             // Move up
        y = y->p;                          // Move to parent
    }
    return y;                              // Return successor (or NULL)
}

// Recursively free all nodes in tree
void destroy_tree(Node* root) {
    if (root != NULL) {              // If node exists
        destroy_tree(root->left);    // First destroy left subtree
        destroy_tree(root->right);   // Then destroy right subtree
        free(root);                  // Finally free this node
    }
}

// Calculate height of tree
int tree_height(Node* node) {
    if (node == NULL)
        return 0;

    int left_height = tree_height(node->left);
    int right_height = tree_height(node->right);

    return 1 + (left_height > right_height ? left_height : right_height);
}

// Silent inorder traversal for timing experiments
void inorder_tree_walk_silent(Node* x) {
    if (x != NULL) {
        inorder_tree_walk_silent(x->left);
        volatile int temp __attribute__((unused)) = x->key;  // Access key without printing
        inorder_tree_walk_silent(x->right);
    }
}

// Helper function for testing - inorder walk with limit
void inorder_tree_walk_limit(Node* x, int limit) {
    static int count = 0;
    if (x != NULL && count < limit) {
        inorder_tree_walk_limit(x->left, limit);
        if (count < limit) {
            printf("%d ", x->key);
            count++;
        }
        inorder_tree_walk_limit(x->right, limit);
    }
    if (x == NULL) count = 0; // Reset for next call
}