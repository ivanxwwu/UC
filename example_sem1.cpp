//
// Created by root on 17-9-1.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <signal.h>
#include <unistd.h>
int semid;
void quit(int signo){
    semctl(semid, IPC_RMID, 0, 0);
    exit(0);
}
int main()
{
    /*生成key*/
    key_t key = ftok(".", 100);
    /*创建信号量集*/
    semid = semget(key,
                   1, /*信号量集(信号量数组)中有多少信号量 */
                   IPC_CREAT|0666);
    if(semid==-1)perror("sem create failed!"),exit(-1);
    /*初始化信号量值*/
//    int res = semctl(semid,
//                     0, /*设置的信号量的下标*/
//                     SETVAL,
//                     5);/*将下标为0的信号量值设置为5*/
    //if(res==-1)perror("init semval failed!"), exit(-1);
    /*操作信号量*/
    struct sembuf op;/*对信号量一个操作*/
    op.sem_num = 0;/*要操作的信号量的下标*/
    op.sem_op = -1;/*对信号量进行何种操作(对信号量进行加减)*/
    op.sem_flg = 0;/*标志值,可以为IPC_NOWAIT*/
    printf("Ctrl+C退出\n");
    signal(SIGINT, quit);
    while(1){
        printf("开始获取共享资源\n");
        if(semop(semid, &op, 1));
        printf("获取资源成功，使用资源\n");
        sleep(1);
    }
    /*删除信号量*/
}
