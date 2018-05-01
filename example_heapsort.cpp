//
// Created by root on 17-9-1.
//

//
// Created by root on 17-8-20.
//
//https://www.2cto.com/kf/201403/284007.html
#include <stdio.h>
#include <stdlib.h>

/*
 利用最大堆实现。

最大堆：最大堆性质是除了根结点意外的所有结点 i 都要满足A[parent[i]] >= A[i]

需要利用到的一个性质：当用数组表示存储n个元素的堆时，叶结点的下标分别是n/2, n/2+1, n/2 + 2, ......,n - 1. （下标从0开始）

需要用到的函数有：

void max_heapify(int *a, int i) //通过让a[i]的值在最大堆中“逐级下降”，从而使得以下标i为根结点的字数重新遵循最大堆的性质。O(lgn)

void build_max_heap(int *a) //对树中非叶结点都调用一次max_heapify(a, i)。 O(n)

void heapsort(int *a, int n) //对长度为n的数组a调用堆排序。 O(nlgn)
 */

void swap(int * a, int * b);
void maxheapify(int arr[], int size, int pos);
void buildheap(int arr[], int size);
void heapsort(int arr[], int size);

void swap(int* a, int* b){
    int tmp;
    tmp = *a;
    *a = *b;
    *b = tmp;
}

void maxheapify(int arr[], int size, int pos){
    int left = (pos<<1) + 1;
    int right = (pos<<1) + 2;
    int maxpos;
    if (left < size && arr[left] > arr[pos])
        maxpos = left;
    else
        maxpos = pos;
    if (right < size && arr[right] > arr[maxpos])
        maxpos = right;
    if (maxpos != pos){
        swap(&arr[maxpos], &arr[pos]);
        maxheapify(arr, size, maxpos);
    }
}
void buildheap(int arr[], int size){
    for(int i=size/2-1; i>=0; --i){
        maxheapify(arr, size, i);
    }
}
void heapsort(int arr[], int size){
    buildheap(arr, size);
    int heapsize = size;
    for(int i = size-1; i>=1; --i){
        swap(&arr[0], &arr[i]);
        --heapsize;
        maxheapify(arr, heapsize, 0);
    }
}

int main(){
    int array[]={0,14,10,4,7,9,3,2,8,1};
    int len=10;
    heapsort(array,len);
    for(int i=0; i<10; i++){
        printf("%d ", array[i]);
    }
}

