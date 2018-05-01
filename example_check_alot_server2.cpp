//
// Created by root on 17-10-4.
//

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <errno.h>
#include <sys/select.h>
#include <fcntl.h>
#include <ucontext.h>
#include "coroutine.h"


void foo(struct schedule *S, void* ud){
    int port = *(int*)ud;
    int fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    sockaddr_in server;
    server.sin_family = AF_INET;
//    server.sin_port = htons(port);
//    if (inet_aton("127.0.0.1", &server.sin_addr)<0){
//        perror("inet fail!");
//        return ;
//    }
    server.sin_port = htons(80);
    if (inet_aton("146.20.110.251", &server.sin_addr)<0){
        perror("inet fail!");
        return ;
    }
    int ret;
    int flag = fcntl(fd, F_GETFL);
    if (flag == -1){
        perror("flag fail");
        return ;
    }
    flag |= O_NONBLOCK;
    ret = fcntl(fd, F_SETFL, flag);
    if (ret == -1){
        printf("f_setfd fail");
    }

    ret = connect(fd, (const sockaddr*)&server, sizeof(sockaddr));
    perror("");
    printf("connect ret:%d  %d %d\n", ret, errno, EINPROGRESS);

    if (ret<0&& errno == EINPROGRESS){
        int maxfds;
        fd_set wset;
        maxfds = fd + 1;
        struct timeval timeout;
        timeout.tv_sec = 0;
        timeout.tv_usec = 0;
        bool isConnect = false;
        while(true){
            printf("1");
            printf("%d\n", coroutine_running(S));
            //coroutine_yield(S);
            FD_ZERO(&wset);
            FD_SET(fd, &wset);
            ret = select(maxfds, NULL, &wset, NULL, &timeout);
            printf("ret:%d\n", ret);
            if (ret == 0){
                //coroutine
                coroutine_yield(S);
                continue;
            } else if (ret <0){
                perror("select error:");
                break;
            } else if (ret == 1){
                int err;
                socklen_t socklen = sizeof(err);
                int sockoptret = getsockopt(fd, SOL_SOCKET, SO_ERROR, &err, &socklen);
                if (sockoptret == -1){
                    perror("");
                    break;
                }
                if (err == 0){
                    //success
                    isConnect = true;
                    break;
                }
                else{
                    perror("socket error");
                    break;
                }
            }
        }
        if (isConnect){
            fd_set rset;
            char buf[4096] = {0};
            sprintf(buf,"HTTP/1.0 200 OK\r\nContent-type: text/plain\r\n\r\n%s","Hello world!\n");
            write(fd, buf, strlen(buf));
            while(true){
                FD_ZERO(&rset);
                FD_SET(fd, &rset);
                struct timeval timeout;
                timeout.tv_usec = 0;
                timeout.tv_sec = 0;
                ret = select(maxfds, &rset, NULL, NULL, &timeout);
                if (ret == 0){
                    //empty
                    coroutine_yield(S);
                    continue;
                }
                else if (ret < 0){
                    //select error...
                    perror("select error");
                    break;
                } else if(ret == 1){
                    //ok
                    memset(buf, 0, sizeof(buf));
                    read(fd, buf, sizeof(buf));//...
                    break;
                }
            }
        }
    }
    close(fd);
    return ;
}

void foo2(struct schedule *S, void* ud){
    int i=0;
    for(;i<5;i++){
        printf("%d %d\n", coroutine_running(S), i);
        coroutine_yield(S);
    }
}

void test(struct schedule * S){
    int port1 = 16870;
    int port2 = 16888;
    int co1 = coroutine_new(S, foo, &port1);
    int co2 = coroutine_new(S, foo, &port2);
    printf("main start\n");
    while (coroutine_status(S,co1) && coroutine_status(S,co2)) {
        coroutine_resume(S,co1);
        coroutine_resume(S,co2);
    }
    printf("main end\n");
}

int  main(){
    struct schedule * S = coroutine_open();
    test(S);
    coroutine_close(S);
}

