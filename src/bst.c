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
    if (z->left == NULL)                    
        transplant(T, z, z->right);         
    else if (z->right == NULL)              
        transplant(T, z, z->left);          
    else {                                  
        Node* y = tree_min(z->right);  
        if (y->p != z) {                    
            transplant(T, y, y->right);     
            y->right = z->right;            
            y->right->p = y;                
        }
        transplant(T, z, y);                
        y->left = z->left;                  
        y->left->p = y;                    
    }
}


void inorder_tree_walk(Node* x) {
    if (x != NULL) {                    
        inorder_tree_walk(x->left);   
        printf("%d ", x->key);         
        inorder_tree_walk(x->right);   
    }
}


Node* tree_min(Node* x) {
    while (x->left != NULL)    // Keep going left
        x = x->left;           // Move to left child
    return x;                  // Return leftmost 
}


Node* tree_max(Node* x) {
    while (x->right != NULL)   // Keep going right
        x = x->right;          // Move to right child
    return x;                  // Return rightmost 
}


Node* tree_search(Node* x, int k) {
    if (x == NULL || k == x->key)  
        return x;                    
    if (k < x->key)                 
        return tree_search(x->left, k);  
    else                             
        return tree_search(x->right, k);
}


Node* tree_successor(Node* x) {
    if (x->right != NULL)                 
        return tree_min(x->right);     
    Node* y = x->p;                        
    while (y != NULL && x == y->right) {  
        x = y;                            
        y = y->p;                          
    }
    return y;                              
}

// Recursively free all nodes
void destroy_tree(Node* root) {
    if (root != NULL) {              
        destroy_tree(root->left);    
        destroy_tree(root->right);   
        free(root);                  
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
        volatile int temp __attribute__((unused)) = x->key;  // Access key without printing -> got this from the internet 
        inorder_tree_walk_silent(x->right);
    }
}


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
    if (x == NULL) count = 0; 
}