#ifndef BST_H
#define BST_H

// Make node structure for the BST
typedef struct Node {
    int key;  //Actual data value in the node we are in
    struct Node* left; // Pointer to left child of node
    struct Node* right; // ptr to right child of node
    struct Node* p; //Parent pointer.
} Node;

//Tree structure defination
typedef struct Tree{
    Node* root; // ptr to root node
}Tree;

//Main Bst funcs:
void tree_insert(Tree* T, Node* z); // Insert node z into tree
void tree_delete(Tree* T, Node* z); //delete node z from tree
void inorder_tree_walk(Node* x); // Gives sorted keys from tree
void transplant(Tree* T, Node*u , Node* v); // Replace subtree at root u with v


//Main BST function helpers:
Node* tree_min(Node* x); //find min at subtree x
Node* tree_max(Node* x); // find max at subtree x
Node* tree_search(Node* x, int key); // search for key key in subtree x
Node* tree_successor(Node* x); // get successor of node x

// mem management - ewww
Node* create_node(int key); //allocate and make new node
Tree* create_tree(void); // allocate and make new tree
void destroy_tree(Node* root); // free all nodes in tree -> recusrive

// Additional functions needed for experiments
int tree_height(Node* node); // calculate height of tree
void inorder_tree_walk_silent(Node* x); // inorder walk without printing (for timing)

// Test helper function
void inorder_tree_walk_limit(Node* x, int limit); // inorder walk with output limit

#endif