#include <stdio.h>
#include <stdlib.h>
typedef struct BinaryTreeNode BinaryTreeNode, *PBinaryTreeNode;

typedef struct BinaryTreeNode {
    int value;
    PBinaryTreeNode right;
    PBinaryTreeNode left;
} BinaryTreeNode, *PBinaryTreeNode;

PBinaryTreeNode createBinaryTree(){
    int value;
    scanf("%d", &value);
    if (value == -1)
        return NULL;
    else{
        BinaryTreeNode * pnode = (BinaryTreeNode*)malloc(sizeof(BinaryTreeNode));
        if (!pnode)
            return NULL;
        pnode->value = value;
        printf("please input %d lvalue:\n", value);
        pnode->left = createBinaryTree();
        printf("please input %d rvalue:\n", value);
        pnode->right = createBinaryTree();
        return pnode;
    }
}

void preTravelTree(PBinaryTreeNode tree){
    if (!tree)
        return ;
    printf("%d ", tree->value);
    preTravelTree(tree->left);
    preTravelTree(tree->right);
}

void inTravelTree(PBinaryTreeNode tree){
    if (!tree)
        return ;
    preTravelTree(tree->left);
    printf("%d ", tree->value);
    preTravelTree(tree->right);
}

void postTravelTree(PBinaryTreeNode tree){
    if (!tree)
        return ;
    preTravelTree(tree->left);
    preTravelTree(tree->right);
    printf("%d ", tree->value);
}

int treeDeep(PBinaryTreeNode tree){
    int deep = 0;
    if (tree){
        int ldeep = treeDeep(tree->left);
        int rdeep = treeDeep(tree->right);
        return ldeep>rdeep ? ldeep+1 : rdeep+1;
    }
    return deep;
}

int main() {
    PBinaryTreeNode tree = createBinaryTree();
    preTravelTree(tree);
    printf("\n");
    inTravelTree(tree);
    printf("\n");
    postTravelTree(tree);
    printf("\n");
    printf("deep: %d\n", treeDeep(tree));
    return 0;
}