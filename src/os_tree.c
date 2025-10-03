#include "stdio.h"
#include "stdlib.h"
#include "../include/os_tree.h"

OSNode* os_create_node(int key){
    OSNode* z = (OSNode*)malloc(sizeof(OSNode));
    z->key = key;
    z->size = 1;
    z->left = NULL;
    z->right = NULL;
    z->p = NULL;
    return z;
}

//create empty OS tree.
OSTree* os_create_tree(void){
    OSTree* T = (OSTree*)malloc(sizeof(OSTree));
    T->root = NULL;
    return T;
}

//Size of subtree 
int os_get_size(OSNode* x){
    if(x == NULL){
        return 0;
    }
    return x->size;
}

//OS-Tree Insert + size maintanence
void os_tree_insert(OSTree* T, OSNode* z){
    //SETup
    OSNode* y = NULL;
    OSNode* x = T->root;

    //1)insertion point
    while(x!= NULL){
        y=x;
        x->size++; //increment size of  each node on this path to maintain size
        if(z->key < x->key){
            x = x->left;
        }
        else{
            x = x->right;
        }
    }

    //2) Normal BST insert now
    z->p = y;
    if (y == NULL){
        T->root = z; //empty tree case
    }
    else if (z->key < y->key){
        y->left = z;
    }
    else{
        y->right = z;
    }
}

//TRansplant
void os_transplant(OSTree* T, OSNode* u, OSNode* v){
    if(u->p == NULL){
        T->root = v;
    }
    else if (u == u->p->left){
        u->p->left = v;
    }
    else{
        u->p->right = v;
    }

    if (v!= NULL){
        v->p = u->p;
    }
}

//OS tree delete + maintaining size along deleted path
void os_tree_delete(OSTree* T, OSNode* z) {
    //1 Decrement size along path from root to z
    OSNode* current = T->root;
    while (current != NULL) {
        current->size--;
        if (z->key < current->key)
            current = current->left;
        else if (z->key > current->key)
            current = current->right;
        else
            break;  // break out since we have found z
    }

    // Normal bst delete
    if (z->left == NULL)
        os_transplant(T, z, z->right);
    else if (z->right == NULL)
        os_transplant(T, z, z->left);
    else {
        OSNode* y = os_tree_min(z->right);

        // succesor is not a direct child hence we update the nodes on the path
        if (y->p != z) {
            OSNode* temp = y->p;
            while (temp != z) {
                temp->size--;
                temp = temp->p;
            }
        }

        if (y->p != z) {
            os_transplant(T, y, y->right);
            y->right = z->right;
            y->right->p = y;
        }
        os_transplant(T, z, y);
        y->left = z->left;
        y->left->p = y;

        // Update y size
        y->size = os_get_size(y->left) + os_get_size(y->right) + 1;
    }
}

//OS_Select to find ith smallest element in subtree
OSNode* os_select(OSNode* x , int i){
    if (x == NULL){
        return NULL;    
    }

    int r = os_get_size(x->left) + 1;  // Rank of x is in this subtree

    if (i == r){
        return x;
    }
    else if (i < r){
        return os_select(x->left, i);
    }
    else{
        return os_select(x->right, i - r);   
    }
}

// OS-RANK
// Find the rank of node x in the tree
int os_rank(OSTree* T, OSNode* x) {
    int r = os_get_size(x->left) + 1;  // Rank in subtree @ at x
    OSNode* y = x;

    while (y != T->root) {
        if (y == y->p->right)
            r = r + os_get_size(y->p->left) + 1;
        y = y->p;
    }
    return r;
}


//Search for node in tree
OSNode* os_tree_search(OSNode* x, int k) {
    if (x == NULL || k == x->key)
        return x;
    if (k < x->key)
        return os_tree_search(x->left, k);
    else
        return os_tree_search(x->right, k);
}


//tree min -> just gaan left
OSNode* os_tree_min(OSNode* x){
    while(x->left != NULL){
        x = x->left;
    }
    return x;
}


//This one will have priting built in for output validation and testing
void os_inorder_tree_walk(OSNode* x) {
    if (x != NULL) {
        os_inorder_tree_walk(x->left);
        printf("%d(size=%d) ", x->key, x->size);
        os_inorder_tree_walk(x->right);
    }
}

//Silent version of above since we are timing it also
void os_inorder_tree_walk_silent(OSNode* x) {
    if (x != NULL) {
        os_inorder_tree_walk_silent(x->left);
        volatile int temp __attribute__((unused)) = x->key; // since i had to google this: volatile means the compiler 
                                                            //wont optimise it since we want to time it and forcing the compiler to 
                                                            //actually read this ptr from memory instead of skipping it since it is unused
        os_inorder_tree_walk_silent(x->right);
    }
}

//Height of tree
int os_tree_height(OSNode* node) {
    if (node == NULL)
        return 0;

    int left_height = os_tree_height(node->left);
    int right_height = os_tree_height(node->right);

    return 1 + (left_height > right_height ? left_height : right_height);
}


//free the tree
void os_destroy_tree(OSNode* root) {
    if (root != NULL) {
        os_destroy_tree(root->left);
        os_destroy_tree(root->right);
        free(root);
    }
}