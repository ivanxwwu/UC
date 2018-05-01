//
// Created by root on 17-9-18.
//

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>


typedef int ElemType; //数据类型
typedef int Status; //返回值类型


//定义二叉树结构
typedef struct BinaryTreeNode{
    ElemType data; //数据域
    struct BinaryTreeNode*lChild, *rChlid; //左右子树域
}BinaryTreeNode, *PBinaryTree;


//先序创建二叉树
Status createBiTree(PBinaryTree *T)
{
    ElemType ch;
    ElemType temp;


    scanf("%d", &ch);
    temp = getchar();


    if (-1 == ch)
        *T = NULL;
    else
    {
        *T = (PBinaryTree)malloc(sizeof(BinaryTreeNode));
        if (!(*T))
            exit(-1);


        (*T)->data = ch;
        printf("输入%d的左子节点：", ch);
        createBiTree(&(*T)->lChild);
        printf("输入%d的右子节点：", ch);
        createBiTree(&(*T)->rChlid);
    }


    return 1;
}


//先序遍历二叉树
void traverseBiTree(PBinaryTree T)
{
    if (NULL == T)
        return ;
    printf("%d ", T->data);
    traverseBiTree(T->lChild);
    traverseBiTree(T->rChlid);


}


//中序遍历二叉树
void inOrderBiTree(PBinaryTree T)
{
    if (NULL == T)
        return ;
    inOrderBiTree(T->lChild);
    printf("%d ", T->data);
    inOrderBiTree(T->rChlid);


}


//后序遍历二叉树
void postOrderBiTree(PBinaryTree T)
{
    if (NULL == T)
        return ;
    postOrderBiTree(T->lChild);
    postOrderBiTree(T->rChlid);
    printf("%d ", T->data);


}


//二叉树的深度


int treeDeep(PBinaryTree T)
{
    int deep = 0;
    if(T)
    {
        int leftdeep = treeDeep(T->lChild);
        int rightdeep = treeDeep(T->rChlid);
        deep = leftdeep>=rightdeep?leftdeep+1:rightdeep+1;
    }
    return deep;
}
//主函数
int main(void)
{
    PBinaryTree T;
    PBinaryTree *p = (PBinaryTree*)malloc(sizeof(PBinaryTree));
    int deepth;
    printf("请输入第一个结点的值,-1表示没有叶结点:\n");
    createBiTree(&T);
    printf("先序遍历二叉树:\n");
    traverseBiTree(T);
    printf("\n");
    printf("中序遍历二叉树:\n");
    inOrderBiTree(T);
    printf("\n");
    printf("后序遍历二叉树:\n");
    postOrderBiTree(T);
    printf("\n");
    deepth=treeDeep(T);
    printf("树的深度为:%d",deepth);
    printf("\n");
    return 0;
}