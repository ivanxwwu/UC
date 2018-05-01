//
// Created by root on 17-9-24.
//

#include <stdio.h>
#include <stdlib.h>

struct AvlNode;
typedef struct AvlNode *Position, *AvlTree;

AvlTree makeEmpty(AvlTree tree);
Position find(int value, AvlTree tree);
Position findMin(AvlTree tree);
Position findMax(AvlTree tree);
AvlTree insNode(int value, AvlTree tree);
AvlTree delNode(int value, AvlTree tree);
Position singleRotateWithLeft(Position position);
Position doubleRotateWithLeft(Position position);
Position singleRotateWithRight(Position position);
Position doubleRotateWithRight(Position position);
int treeDeep(AvlTree tree);
int retrieve(Position p);

struct AvlNode{
    int value;
    AvlTree left;
    AvlTree right;
    int height;
};

AvlTree makeEmpty(AvlTree tree){
    if (tree){
        makeEmpty(tree->left);
        makeEmpty(tree->right);
        free(tree);
    }
    return NULL;
}

Position findMin(AvlTree tree){
    if (tree){
        while(tree->left) tree=tree->left;
        return tree;
    }
}

int treeDeep(AvlTree tree){
    if(tree==NULL)
        return -1;
    else
        return tree->height;
}

int max(int a, int b){
    return a>b ? a : b;
}

AvlTree insNode(int value, AvlTree tree){
    if(tree==NULL) {
        tree = (AvlTree) malloc(sizeof(struct AvlNode));
        if (tree == NULL)
            perror("out of space");
        else {
            tree->value = value;
            tree->height = 0;
            tree->left = tree->right = 0;
        }
    }
    else if(value < tree->value){
        tree->left = insNode(value, tree->left);
        if(treeDeep(tree->left) - treeDeep(tree->right) == 2){
            if(value < tree->left->value)
                tree = singleRotateWithLeft(tree);
            else
                tree = doubleRotateWithLeft(tree);
        }
    }
    else if(value > tree->value){
        tree->right = insNode(value, tree->right);
        if(treeDeep(tree->right) - treeDeep(tree->left) == 2){
            if(value > tree->right->value)
                tree = singleRotateWithLeft(tree);
            else
                tree = doubleRotateWithLeft(tree);
        }
    }
    tree->height = max(treeDeep(tree->left), treeDeep(tree->right))+1;
    return tree;
}

Position singleRotateWithLeft(Position position){
    Position tmp;
    tmp = position->left;
    position->left = tmp->right;
    tmp->right = position;
    position->height = max(treeDeep(position->left), treeDeep(position->right)) + 1;
    tmp->height = max(treeDeep(position->left), treeDeep(position->right)) + 1;
    return tmp;
}

Position doubleRotateWithLeft(Position position){
    position->left = singleRotateWithRight(position->left);
    return singleRotateWithLeft(position);
}

Position singleRotateWithRight(Position position){
    Position  tmp;
    tmp = position->right;
    position->right = tmp->left;
    tmp->left = position;
    position->height = max(treeDeep(position->left), treeDeep(position->right)) + 1;
    tmp->height = max(treeDeep(position->left), treeDeep(position->right)) + 1;
    return tmp;
}

Position doubleRotateWithRight(Position position){
    position->right = singleRotateWithLeft(position->right);


    return singleRotateWithLeft(position);
}

int main(){
    AvlTree avlTree = makeEmpty(NULL);
    avlTree = insNode(10, avlTree);
    avlTree = insNode(15, avlTree);
    avlTree = insNode(6, avlTree);
    avlTree = insNode(3, avlTree);
    avlTree = insNode(4, avlTree);
    Position position = findMin(avlTree);
    printf("%d\n", avlTree->height);
    printf("%d\n", position->value);
//    avlTree = insNode(3, avlTree);
//    avlTree = insNode(3, avlTree);
//    avlTree = insNode(3, avlTree);
}

