//
// Created by root on 17-9-1.
//
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

void set_fd_nonblock(int fd){
    int flags = fcntl(fd, F_GETFL, 0);
    if(flags == -1){
        perror("get flags error!");
        return ;
    }
    flags |= O_NONBLOCK;
    fcntl(fd, F_SETFD, flags);
    if (flags == -1){
        perror("set flags error!");
        return ;
    }
}

void worker(int listenfd){
    while(true){
        struct sockaddr_in client;
        socklen_t socklen = sizeof(client);
        int clientfd = accept(listenfd, (sockaddr*)&client, &socklen);
        if (clientfd< 0){
            perror("accept fail");
            continue;
        }
        printf("client ip:%s, port:%d\n", inet_ntoa(client.sin_addr), ntohs(client.sin_port));
        char data[4096] = {0};
        set_fd_nonblock(clientfd);
        int len = read(clientfd, data, sizeof(data));
        printf("data:%s", data);
        write(clientfd, data, len);
        close(clientfd);
    }
}

int main(){
    int fd = socket(PF_INET, SOCK_STREAM, 0);
    if(fd < 0)
        perror("socket error!");
    struct sockaddr_in in;
    in.sin_family = PF_INET;
    in.sin_port = htons(12345);
    in.sin_addr.s_addr = INADDR_ANY;
    if (bind(fd, (const sockaddr*)&in, sizeof(struct sockaddr_in)) < 0){
        perror("bind error!");
        exit(1);
    }
    listen(fd, 5);
    worker(fd);
//    for(int i=0; i<5; i++){
//        pid_t pid = fork();
//        if (pid == 0){
//            worker(fd);
//        }
//        else if (pid > 0){
//            continue;
//        }
//        else{
//            perror("fork erorr");
//        }
//    }
//    int status;
//    wait(&status);
    close(fd);
}