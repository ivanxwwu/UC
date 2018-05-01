//
// Created by root on 17-9-28.
//

#include <stdio.h>

struct LeftHeapNode;

typedef struct LeftHeapNode *PriorityQueue, *Position;

PriorityQueue init(void);
int findMin(PriorityQueue queue);
int isEmpty(PriorityQueue queue);
PriorityQueue merge(PriorityQueue queue1, PriorityQueue queue2);
PriorityQueue insert(int x, PriorityQueue queue);
PriorityQueue deleteMin(PriorityQueue queue);

struct LeftHeapNode{
    int element;
    PriorityQueue left;
    PriorityQueue right;
    int npl;
};

void swapChild(Position pos){
    Position tmp = pos->left;
    pos->left = pos->right;
    pos->right = tmp;
}

PriorityQueue merge(PriorityQueue queue1, PriorityQueue queue2){
    if (queue1 == NULL)
        return queue2;
    if (queue2 == NULL)
        return queue1;
    if (queue1->element < queue2->element)
        return merge1(queue1, queue2);
    else
        return merge1(queue2, queue1);
}

PriorityQueue merge1(PriorityQueue queue1, PriorityQueue queue2){
    if (queue1->left == NULL)
        queue1->left = queue2;
    else{
        queue1->right = merge(queue1->right, queue2);
        if (queue1->left->npl < queue1->right->npl)
            swapChild(queue1);
        queue1->npl = queue1->right->npl + 1;
    }
    return queue1;
}

PriorityQueue insert1(int x, PriorityQueue queue){
    PriorityQueue singleNode;
    singleNode = (PriorityQueue)malloc(sizeof(struct LeftHeapNode));
    if (singleNode == NULL)
        perror("out of space!!");
    else {
        singleNode->element = x;
        singleNode->npl = 0;
        singleNode->left = singleNode->right = NULL;
        queue = merge(singleNode, queue);
    }
    return queue;
}

PriorityQueue deleteMin(PriorityQueue queue){
    PriorityQueue leftHeap, rightHeap;
    if (isEmpty(queue)){
        perror("Priority queue is empty");
        return queue;
    }
    leftHeap = queue->left;
    rightHeap = queue->right;
    free(queue);
    return merge(leftHeap, rightHeap);
}

int main(){

}