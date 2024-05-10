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
    TreeNode *newNode = createTreeNode(key, value);

    TreeNode *aux = NULL;
    TreeNode *auxRoot = tree->root;

    while (auxRoot != NULL){
        aux = auxRoot;
        if (tree->lower_than(key, auxRoot->pair->key) == 1)
            auxRoot = auxRoot->left;
        
        else
            auxRoot = auxRoot->right;
    }
    
    newNode->parent = aux;
    
    if (aux == NULL){
        tree->root = newNode;
    }
        
    else {
        if (tree->lower_than(key, aux->pair->key) == 1){
            aux->left = newNode;
        }
        else{
            aux->right = newNode;
        }
    }
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
    TreeNode *current = tree->current;
    TreeNode *parent = NULL;

    if (current->right != NULL){
        current = current->right;
        while (current->left != NULL){
            current = current->left;
        }
        tree->current = current;
        return current->pair;
    }
    else {
        parent = current->parent;
        while (parent != NULL && current == parent->right){
            current = parent;
            parent = parent->parent;
        }
        tree->current = current;
        return current->pair;
    }
    return NULL;
}
