#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/*
（1）若左子树不空，则左子树上所有结点的值均小于它的根结点的值；
（2）若右子树不空，则右子树上所有结点的值均大于它的根结点的值；
（3）左、右子树也分别为二叉排序树；
（4）没有键值相等的结点。

 */

typedef struct TreeNode *Position, *PSearchTree;

typedef struct TreeNode{
    int value;
    PSearchTree left;
    PSearchTree right;
};

Position findMin(PSearchTree tree);

PSearchTree makeEmpty(PSearchTree tree){
    if (tree){
        makeEmpty(tree->left);
        makeEmpty(tree->right);
        free(tree);
    }
    return NULL;
}

PSearchTree insNode(int value, PSearchTree tree){
    if (tree == NULL){
        tree = (PSearchTree)malloc(sizeof(TreeNode));
        if (tree==NULL){
            perror("malloc error");
            return ;
        }
        tree->value = value;
        tree->left = NULL;
        tree->right = NULL;
    }
    else if (value > tree->value){
        tree->right = insNode(value, tree->right);
    }
    else if (value < tree->value){
        tree->left = insNode(value, tree->left);
    }
    return tree;
}

PSearchTree delNode(int value, PSearchTree tree){
    if (tree==NULL){
        return NULL;
    }
    if (value > tree->value)
        return delNode(value, tree->right);
    else if (value < tree->value)
        return delNode(value, tree->left);
    else{
        Position tmpCell = NULL;
        if (tree->right && tree->left){
            Position min = findMin(tree->right);
            tree->value = min->value;
            tree->right = delNode(min->value, tree->right);
        }
        else{
            tmpCell = tree;
            if (tree->right)
                tree = tree->right;
            if (tree->left)
                tree = tree->left;
            free(tmpCell);
        }
    }
    return tree;
}

Position find(int value, PSearchTree tree){
    if (tree==NULL)
        return NULL;
    if (value > tree->value)
        return find(value, tree->right);
    else if (value < tree->value)
        return find(value, tree->left);
    else{
        return tree;
    }
}

Position findMin(PSearchTree tree){
    if (tree==NULL)
        return NULL;
    if (tree->left==NULL)
        return tree;
    return findMin(tree->left);
}

Position findMax(PSearchTree tree){
    if (tree)
    {
        while(tree->right) tree = tree->right;
        return tree;
    }
}

int main(){
    srand(time(NULL));
    PSearchTree tree = makeEmpty(NULL);
    insNode(5, tree);
    printf("root:%d\n", tree->value);
    for(int i=0; i<100; i++){
        tree = insNode(rand()%1000, tree);
    }
    Position position = findMin(tree);
    printf("%d\n", position->value);
    position = findMax(tree);
    printf("%d\n", position->value);
    delNode(5, tree);


}