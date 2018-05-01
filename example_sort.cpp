//
// Created by root on 17-9-28.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void insertSort(int arr[], int size){
    int j, i;
    int tmp;
    for(i=1; i<size; i++){
        tmp = arr[i];
        for(j=i; j>0 && arr[j-1]>tmp; j--)
            arr[j] = arr[j-1];
        arr[j] = tmp;
    }
}

void shellSort(int arr[], int size){
    int i, j, increment;
    int tmp;
    for(increment = size/2; increment>0; increment/=2){
        for(i=increment; i<size;i++){
            tmp = arr[i];
            for(j=i; j>=increment; j-=increment){
                if (tmp<arr[j-increment])
                    arr[j] = arr[j-increment];
                else
                    break;
            }
            arr[j] = tmp;
        }
    }
}

void merge(int arr[], int tmpArray[], int lpos, int rpos, int rightend){
    int i, leftEnd, numELements, tmpPos;
    leftEnd = rpos - 1;
    tmpPos = lpos;
    numELements = rightend - lpos + 1;
    while(lpos <= leftEnd && rpos <= rightend) {
        if (arr[lpos] <= arr[rpos])
            tmpArray[tmpPos++] = arr[lpos++];
        else
            tmpArray[tmpPos++] = arr[rpos++];
    }
    while(lpos <= leftEnd)
        tmpArray[tmpPos++] = arr[lpos++];
    while(rpos <= rightend)
        tmpArray[tmpPos++] = arr[rpos++];
    for(i=0; i<numELements; i++, rightend--)
        arr[rightend] = tmpArray[rightend];
}

void mSort(int arr[], int tmpArray[], int left, int right){
    int center;
    if (left < right){
        center = (left + right) / 2;
        mSort(arr, tmpArray, left, center);
        mSort(arr, tmpArray, center + 1, right);
        merge(arr, tmpArray, left, center+1, right);
    }
}

void mergeSort(int arr[], int size){
    int *tmpArray;
    tmpArray = (int *)malloc(size*sizeof(int));
    if (tmpArray != NULL)
    {
        mSort(arr, tmpArray, 0, size-1);
    }
    else
        perror("no space");
}


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

typedef struct node {
    int key;
    struct node *next;
}KeyNode;

void bucketsort(int keys[],int size,int bucket_size) {
    int i,j;
    KeyNode **bucket_table = (KeyNode **)malloc(bucket_size * sizeof(KeyNode*));
    for(i = 0;i < bucket_size;i++) {
        bucket_table[i] = (KeyNode*)malloc(sizeof(KeyNode));
        bucket_table[i]->key = 0;
        bucket_table[i]->next = NULL;
    }
    for(j = 0;j < size;j++) {
        KeyNode *node = (KeyNode *)malloc(sizeof(KeyNode));
        node->key = keys[j];
        node->next = NULL;
        int index = keys[j]/10;
        KeyNode *p = bucket_table[index];
        if(p->key == 0) {
            bucket_table[index]->next = node;
            (bucket_table[index]->key)++;
        }else {
            while(p->next != NULL && p->next->key <= node->key)
                p = p->next;
            node->next = p->next;
            p->next = node;
            (bucket_table[index]->key)++;
        }
    }
    KeyNode * k = NULL;
    int tmp = 0;
    for(i = 0;i < bucket_size;i++)
        for(k = bucket_table[i]->next;k!=NULL;k=k->next)
            keys[tmp++] = k->key;
}

void print(int arr[], int size){
    for(int i=0; i<size; i++)
        printf("%d ", arr[i]);
    printf("\n");
}

int main(){
    int arr[] = {81, 94, 11, 96, 12, 35, 17, 95, 28, 58, 41, 75, 15};
    int size = sizeof(arr)/sizeof(arr[0]);
    //insertSort(arr, 6);
    //shellSort(arr, size);
    //mergeSort(arr, size);
    //heapsort(arr, size);
    bucketsort(arr, size,10);
    print(arr, size);
}