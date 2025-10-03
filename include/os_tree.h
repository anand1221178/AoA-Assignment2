#ifndef OS_TREE_H
#define OS_TREE_H

//Order-stat Tree node + size attrr
typedef struct OSNode {
    int key;                //key val
    int size;               // Size of subtree
    struct OSNode* left;    // left chiled
    struct OSNode* right; // right child
    struct OSNode* p; // parent
} OSNode;

// Tree struct
typedef struct OSTree{
    OSNode* root;
} OSTree;

//tree manaagement
OSTree* os_create_tree(void);
OSNode* os_create_node(int key);
void os_destroy_tree(OSNode* root);

// OSTree operations 
void os_tree_insert(OSTree* T, OSNode* z);
void os_tree_delete(OSTree* T, OSNode* z);
void os_transplant(OSTree* T, OSNode* u, OSNode* v);

// Order-stats operations
OSNode* os_select(OSNode* x, int i);     
int os_rank(OSTree* T, OSNode* x);      

//Helpers
OSNode* os_tree_search(OSNode* x, int k);
OSNode* os_tree_min(OSNode* x);
int os_get_size(OSNode* x);              // Helper to get size (0 if nil)
int os_tree_height(OSNode* node);        // For testing

// Tree traversal
void os_inorder_tree_walk(OSNode* x);
void os_inorder_tree_walk_silent(OSNode* x);

#endif