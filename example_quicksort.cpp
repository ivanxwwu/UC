//
// Created by root on 17-9-1.
//

#include <stdio.h>

void quicksort(int arr[], int left, int right){
    if (left>=right)
        return ;
    int i = left;
    int j = right;
    int key = arr[i];
    while(i<j){
        while(i<j && arr[j]>=key)j--;
        arr[i] = arr[j];
        while(i<j && arr[i]<=key)i++;
        arr[j] = arr[i];
    }
    arr[i] = key;
    quicksort(arr, i+1, right);
    quicksort(arr, left, i-1);
}

int main(){
    int arr[10] = {1,5,7,10,2,3,45,6,78,9};
    int size = sizeof(arr)/sizeof(arr[0]);
    quicksort(arr, 0, size-1);
    for(int i=0; i<size; i++){
        printf("%d ", arr[i]);
    }
}