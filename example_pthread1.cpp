//
// Created by root on 17-9-1.
//


#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <cstring>
pthread_mutex_t lock;

void* thread1(void* p){
    /*接受取消请求 PTHREAD_CANCEL_DISABLE不接受取消请求*/
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, 0);
    /*如果取消请求被接受，立即取消 PTHREAD_CANCEL_DEFERRED等到取消点再取消*/
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, 0);
    while(1){
        printf("thread1\n");
    }
}
void* thread2(void* p){
    sleep(60);
    pthread_t tid = *(pthread_t*)p;
    printf("准备取消另外一个线程\n");
    int res = pthread_cancel(tid);
    if(res){
        printf("取消线程操作失败,%s\n", strerror(res));
    }else{
        printf("取消线程操作成功\n");
    }
}
int main()
{
    pthread_t t1, t2;
    pthread_create(&t1, 0, thread1, 0);
    pthread_create(&t2, 0, thread2, &t1);
    pthread_join(t1, 0);
    pthread_join(t2, 0);
}