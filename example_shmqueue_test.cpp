//
// Created by root on 18-12-15.
//

#include <iostream>
#include <string.h>
#include "shmqueue/shm_queue.h"
using namespace std;

int main() {
    // 创建或打开队列

    long shmkey = 0x1234;

    int element_size = 64; // 基本块的大小，如果数据小于1个块，就按1个块存储，否则，存储到连续多个块中，只有第一个块有块头部信息

    int element_count = 1024; // 队列的长度，总共有多少个块

    struct shm_queue *sq = NULL;
    sq = sq_open(shmkey);
    if (sq == NULL) {
        sq = sq_create(shmkey, element_size, element_count, 1, 1); // 后面两个参数指定每写一个数据就通知一个读进程
    }

    char *data = "要写什么就写什么，毫不犹豫";

    if(sq_put(sq, data, strlen(data))<0)
    {
        // 队列满了。。。
        cout << "1232" << endl;
    }
    int event_fd = sq_get_eventfd(sq);
    while(1)
    {
        char buffer[1024];
        fd_set fdset;
        FD_ZERO(&fdset);
        FD_SET(event_fd, &fdset);
        struct timeval to = {1, 0}; // 1s
        sq_sigon(sq); // now we are entering into sleeping sys call
        int ret = select(event_fd+1, &fdset, NULL, NULL, &to);
        sq_sigoff(sq); // no longer needs signal
        if(ret<0)
        {
            continue;// error
        }
        if(FD_ISSET(event_fd, &fdset)) // 把通知消耗掉
            sq_consume_event(sq);
        int len = sq_get(sq, buffer, sizeof(buffer), NULL);

        if(len<0) // 读失败
        {
            printf("1111");
        }
        else if(len==0) // 没有数据
        {

        }
        else // 收到数据了
        {
            printf("%s\n", buffer);
        }
    }

}