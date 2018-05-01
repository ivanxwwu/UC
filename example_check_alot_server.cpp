//
// Created by root on 17-10-1.
//

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <errno.h>>
#include <fcntl.h>
int main(){
    //declare 10000 socket  port from 10000 to , set sockets non-block
    int socketfds[10000] = {0};
    const int count = 10000;
    for(int i=0; i<count; i++){
        socketfds[i] = socket(AF_INET, SOCK_STREAM, 0);

        int flags = fcntl(socketfds[i], F_GETFD);
        if (flags == -1){perror("fcntl get fail");}
        flags |= O_NONBLOCK;
        int ret = fcntl(socketfds[i], F_SETFD, flags);
        if (ret == -1){perror("fcntl set fail");}
    }
    //declare epollfd manager 10000 socket, listen write event
    int epollfd = epoll_create(count);
    for(int i=0; i<count; i++){
        struct epoll_event event;
        event.data.fd = socketfds[i];
        event.events = EPOLLOUT | EPOLLET;
        epoll_ctl(epollfd, EPOLL_CTL_ADD, socketfds[i], &event);
    }


    //connect...

    for(int i=0; i<count; i++){
        sockaddr_in server;
        server.sin_port = ntohs(10000+i);
        server.sin_family = AF_INET;
        if (inet_aton("127.0.0.1", &server.sin_addr)!=1){
            perror("inet fail");
            continue;
        }
        int ret = connect(socketfds[i], (const sockaddr*)&server, (socklen_t)sizeof(server));
        if (ret < 0 && errno == EINPROGRESS) {
            perror("error");
        }

    }

    epoll_event events[count];
    int curfds=count;
    int epollfd2 = epoll_create(count);
    int socketfds2[count] = {0};
    int count2 = 0;
    //epoll_wait,  get connected socket
    while(true){
        int nfds = epoll_wait(epollfd, events, curfds, 5000);
        for(int i=0; i<nfds; i++){
            if (events[i].events & EPOLLOUT && !(events[i].events & EPOLLHUP))
            {
                socketfds2[count2++] = events[i].data.fd;
                epoll_event event;
                event.events = EPOLLIN | EPOLLONESHOT | EPOLLET;
                event.data.fd = events[i].data.fd;
                epoll_ctl(epollfd2, EPOLL_CTL_ADD, events[i].data.fd, &event);
            }
        }
        if (nfds == 0)
            break;
    }

    //send data   epoll_wait

    for(int i=0; i<count2; i++){
        char buf[1000] = {0};
        sprintf(buf,"HTTP/1.0 200 OK\r\nContent-type: text/plain\r\n\r\n%s","Hello world!\n");
        write(socketfds2[i], buf, strlen(buf));
    }
    epoll_event events2[count2];
    int curfds2 = count2;
    for(;;){
        int nfds = epoll_wait(epollfd2, events2, curfds2, -1);
        for(int i=0; i<nfds; i++){
            char buf[4096] = {0};
            read(events2[i].data.fd, buf, 4096);
            printf("%s\n", buf);
        }
    }

}

