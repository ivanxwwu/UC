//
// Created by root on 17-10-9.
//

#include "ev.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

void connected_callback(struct ev_loop* reactor, ev_io* w, int events){
    int err;
    socklen_t socklen = sizeof(err);
    int sockoptret = getsockopt(w->fd, SOL_SOCKET, SO_ERROR, &err, &socklen);
    if (sockoptret == -1){
        perror("");
    }
    if (err == 0){
        //success
        printf("%d,connected\n", w->fd);
    }
    else{
        perror("socket error");
    }
    close(w->fd);
    ev_break(reactor, EVBREAK_CANCEL);
}

int main(){
    struct ev_loop* reactor = ev_loop_new(EVFLAG_AUTO);
    ev_io io;
    const int count = 10000;
    int fds[count] = {0};
    for(int i=0; i<count; i++){
        fds[i] = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        struct sockaddr_in server;
        server.sin_family = AF_INET;
        server.sin_port = htons(10000+i);
        inet_aton("127.0.0.1", &server.sin_addr);
        int flag = fcntl(fds[i], F_GETFL);
        if (flag == -1){
            perror("get fcntl...");
            return -1;
        }
        flag |= O_NONBLOCK;
        int ret = fcntl(fds[i], F_SETFL, flag);
        if (ret == -1){
            perror("set fcntl...");
            return -1;
        }
        ret = connect(fds[i], (const sockaddr*)&server, sizeof(server));
        if (ret<0&& errno == EINPROGRESS) {
            ev_io_init(&io, &connected_callback, fds[i], EV_WRITE);
            ev_io_start(reactor, &io);

        }
        //close(fds[i]);
    }
    ev_run(reactor, 0);
    ev_loop_destroy(reactor);
    return 0;
}