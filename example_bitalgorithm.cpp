//
// Created by root on 17-9-3.
//

#include <stdio.h>

bool isPowerOf2(int n){
    return (n&n-1)==0 ? true : false;
}

int main(){
    printf("%d\n", isPowerOf2(4));
}