//
// Created by root on 17-9-4.
//

//
// Created by root on 8/14/16.
//

#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include <stdio.h>
#include <pthread.h>

char store[8];//仓库
int size=0;//库存

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
/*当仓库满时,用来停止生产*/
pthread_cond_t full = PTHREAD_COND_INITIALIZER;
/*当仓库空时,用来停止消费*/
pthread_cond_t empty = PTHREAD_COND_INITIALIZER;
void print(){
    int i;
    for(i=0; i<size; i++){
        printf("%c ", store[i]);
    }
    printf("\n");
}

void* producer(void* p){
    char c;
    for(c='A'; c<='Z'; c++){
        pthread_mutex_lock(&lock);
        /*当条件满足，线程进入条件变量full,同时释放互斥锁lock*/
        /*建议用while判断条件是否成立*/
        while(size==8)pthread_cond_wait(&full, &lock);
        store[size] = c;
        printf("PUSH:%c\n", c);
        size++;
        print();
        pthread_cond_broadcast(&empty);
        pthread_mutex_unlock(&lock);
    }
}

void* customer(void* p){
    int i;
    for(i=0; i<52; i++){
        pthread_mutex_lock(&lock);
        /*当条件满足，线程进入条件变量empty,同时释放互斥锁lock*/
        while(size==0)pthread_cond_wait(&empty, &lock);
        size--;
        printf("POP:%c\n", store[size]);
        print();
        pthread_cond_broadcast(&full);
        pthread_mutex_unlock(&lock);
    }
}

int main()
{
    pthread_t t1, t2, t3;
    pthread_create(&t1, 0, producer, 0);
    pthread_create(&t2, 0, customer, 0);
    pthread_create(&t3, 0, producer, 0);
    pthread_join(t1, 0);
    pthread_join(t2, 0);
    pthread_join(t3, 0);
}
