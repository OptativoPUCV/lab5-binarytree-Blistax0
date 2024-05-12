#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;


struct TreeNode {
    Pair* pair;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}


TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->pair = (Pair *)malloc(sizeof(Pair));
    new->pair->key = key;
    new->pair->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)) {
    TreeMap * new = (TreeMap *)malloc(sizeof(TreeMap));

    new->root = NULL;
    new->current = NULL;
    new->lower_than = lower_than;
    return new;
}


void insertTreeMap(TreeMap * tree, void* key, void * value) {
    if (tree == NULL)
        exit(EXIT_FAILURE);

    TreeNode* nodoNuevo = createTreeNode(key, value);

    if (tree->root == NULL){
        tree->root = nodoNuevo;
        tree->current = nodoNuevo;
        return;
    }

    TreeNode* current = tree->root;
    TreeNode* parent = NULL;

    while (current != NULL){
        parent = current;
        if (is_equal(tree, key, current->pair->key)) 
            return;

        if (tree->lower_than(key, current->pair->key))
            current = current->left;
        
        else
            current = current->right;
        
    }

    if (tree->lower_than(key, parent->pair->key))
        parent->left = nodoNuevo;
    else
        parent->right = nodoNuevo;

    nodoNuevo->parent = parent;
    tree->current = current;
}

TreeNode * minimum(TreeNode * x){
    while (x->left != NULL){
        x = x->left;
    }
    return x;
}


void removeNode(TreeMap * tree, TreeNode* node) {
    if (node->left == NULL && node->right == NULL){
        if (node->parent == NULL){
            tree->root = NULL;
        }
        else {
            if (node->parent->left == node){
                node->parent->left = NULL;
            }
            else{
                node->parent->right = NULL;
            }
        }
    }    
    else {
        if (node->left != NULL && node->right != NULL){
            TreeNode *aux = minimum(node->right);

            node->pair->key = aux->pair->key;
            node->pair->value = aux->pair->value;
            removeNode(tree, aux);
        }
        else {
            TreeNode *child;
            if (node->left != NULL){
                child = node->left;
            }
            else{
                child = node->right;
            }
            if (node->parent == NULL){
                tree->root = child;
            }
            else {
                if (node->parent->left == node){
                    node->parent->left = child;
                }
                else{
                    node->parent->right = child;
                }
            }
            child->parent = node->parent;
        }
    }
}


void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);
}




Pair * searchTreeMap(TreeMap * tree, void* key) {
    TreeNode *current = tree->root;
    TreeNode *parent = NULL;
    
    while (current != NULL){
        if (is_equal(tree, current->pair->key, key)){
            tree->current = current;
            return current->pair;
        }
        if (tree->lower_than(key, current->pair->key) == 1){
            parent = current;
            current = current->left;
        }
        else {
            parent = current;
            current = current->right;
        }
    }
    tree->current = parent;
    return NULL;
}


Pair * upperBound(TreeMap * tree, void* key) {
    TreeNode *current = tree->root;
    TreeNode *parent = NULL;

    while (current != NULL){
        if (is_equal(tree, current->pair->key, key)){
            tree->current = current;
            return current->pair;
        }
        if (tree->lower_than(key, current->pair->key) == 1){
            parent = current;
            current = current->left;
        }
        else {
            parent = current;
            current = current->right;
        }
    }
    tree->current = parent;
    return NULL;
}

Pair * firstTreeMap(TreeMap * tree) {
    TreeNode * current = tree->root;
    TreeNode * parent = NULL;

    while (current != NULL){
        parent = current;
        current = current->left;
    }
    tree->current = parent;
    return parent->pair;
}

Pair * nextTreeMap(TreeMap * tree) {
    if (tree == NULL || tree->current == NULL) 
        return NULL;

    TreeNode *current = tree->current;

    if (current != NULL && current->right != NULL) {
        current = current->right;
        while (current->left != NULL){
            current = current->left;
        }
    }
    else {
        while (current->parent != NULL && current->parent->right == current){
            current = current->parent;
        }
        if (current == NULL || current->parent == NULL)
            return NULL;
        current = current->parent;
    }
    tree->current = current;

    return current->pair;
}
