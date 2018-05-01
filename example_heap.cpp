//
// Created by root on 17-9-27.
//
#include <stdio.h>
#include <limits.h>
#include <stdlib.h>

const int MINPQSIZE=1;
struct Heap;
typedef struct Heap *PriorityQueue;
PriorityQueue init(int max);
void destroy(PriorityQueue queue);
void makeEmpty(PriorityQueue queue);
void insert(int x, PriorityQueue queue);
int deleteMin(PriorityQueue queue);
int findMin(PriorityQueue queue);
int isEmpty(PriorityQueue queue);
int isFull(PriorityQueue queue);

struct Heap{
    int capacity;
    int *elements;
    int size;
};

int isEmpty(PriorityQueue queue){
    return queue->size == 0;
}

int isFull(PriorityQueue queue){
    return queue->size == queue->capacity;
}

PriorityQueue init(int max){
    PriorityQueue queue;
    if (max < MINPQSIZE){
        perror("Priority queue size is too small");
    }
    queue = (PriorityQueue )malloc(sizeof(struct Heap));
    if (queue == NULL)
        perror("out of space!!");
    queue->elements = (int*)malloc((max + 1) * sizeof(int));
    if (queue->elements == NULL)
        perror("out of space");
    queue->capacity = max;
    queue->size = 0;
    //queue->elements[0] = INT_MIN;
    return queue;
}

void insert(int x, PriorityQueue queue){
    int i;
    if (isFull(queue)){
        perror("Priority queue is Full");
        return ;
    }
    for(i = queue->size++; queue->elements[(i-1)/2] > x && i>0; i=(i-1)/2)
        queue->elements[i] = queue->elements[(i-1)/2];

    queue->elements[i] = x;
}

void print(PriorityQueue queue){
    for(int i=0; i<queue->size; i++){
        printf("%d\n", queue->elements[i]);
    }
}

int deleteMin(PriorityQueue queue){
    int i, child;
    int minElement, lastElement;
    if (isEmpty(queue)){
        perror("priority queue is empty");
        return 0;
    }
    minElement = queue->elements[0];
    lastElement = queue->elements[--queue->size];
    for(i=0; i*2<queue->size; i=child){
        child = i*2+1;
        if (child != queue->size && queue->elements[child+1] < queue->elements[child])
            child++;
        if (lastElement > queue->elements[child])
            queue->elements[i] = queue->elements[child];
        else
            break;
    }
    queue->elements[i] = lastElement;
    return minElement;
}

int main(){
    PriorityQueue queue = init(300);
    insert(2, queue);
    insert(4, queue);
    insert(3, queue);
    insert(6, queue);
    insert(5, queue);
    insert(100, queue);
    insert(10, queue);
    insert(8, queue);
    insert(7, queue);
    print(queue);
    printf("===========================\n");
    deleteMin(queue);
    print(queue);
}












