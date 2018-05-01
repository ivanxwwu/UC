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

#define DEFAULT_STACK_SIZE (1024*128)
#define MAX_COROUTINE_SIZE 1024

enum CoroutineState{FREE, RUNNABLE, RUNNING, SUSPEND};
typedef void (*Fun)(void *arg);
typedef struct Coroutine_t{
    ucontext_t ctx;
    Fun func;
    void *arg;
    enum CoroutineState state;
    char stack[DEFAULT_STACK_SIZE];
}Coroutine_t;


typedef struct Schedule_t{
    ucontext_t main;
    int running_coroutine;
    Coroutine_t * coroutines;
    int max_index;

    Schedule_t():running_coroutine(-1), max_index(0){
        coroutines = new Coroutine_t[MAX_COROUTINE_SIZE];
        for(int i=0; i<MAX_COROUTINE_SIZE; i++){
            coroutines[i].state = FREE;
        }
    }
    ~Schedule_t(){
        delete [] coroutines;
    }
}Schedule_t;


static void coroute_body(Schedule_t*ps){
    int id = ps->running_coroutine;
    if (id != -1)
    {
        Coroutine_t * t = &(ps->coroutines[id]);
        t->func(t->arg);
        t->state = FREE;
        ps->running_coroutine = -1;
    }
}

int coroutine_create(Schedule_t &schedule, Fun func, void* arg){
    int id = 0;
    for(id = 0; id<schedule.max_index; ++id){
        if (schedule.coroutines[id].state == FREE)
            break;
    }
    if (id == schedule.max_index)
        schedule.max_index++;
    Coroutine_t* t = &(schedule.coroutines[id]);
    t->state = RUNNABLE;
    t->func = func;
    t->arg = arg;
    getcontext(&(t->ctx));
    t->ctx.uc_stack.ss_sp = t->stack;
    t->ctx.uc_stack.ss_size = DEFAULT_STACK_SIZE;
    t->ctx.uc_stack.ss_flags = 0;
    t->ctx.uc_link = &(schedule.main);
    schedule.running_coroutine = id;
    makecontext(&(t->ctx), (void(*)(void))(coroute_body), 1, &schedule);
    swapcontext(&(schedule.main), &(t->ctx));
    return id;
}


void coroutine_yield(Schedule_t &schedule){
    printf("%d\n", schedule.running_coroutine);
    if (schedule.running_coroutine != -1)
    {
        Coroutine_t *t = &(schedule.coroutines[schedule.running_coroutine]);
        t->state = SUSPEND;
        schedule.running_coroutine = -1;
        swapcontext(&(t->ctx), &(schedule.main));
    }
}

void coroutine_resume(Schedule_t &schedule, int id){
    if (id<0 || id >= schedule.max_index)
        return ;
    Coroutine_t* t = &(schedule.coroutines[id]);
//    if (t->state == SUSPEND){
//        swapcontext(&(schedule.main), &(t->ctx));
//    }
    switch(t->state){
        case RUNNABLE:
            getcontext(&(t->ctx));

            t->ctx.uc_stack.ss_sp = t->stack;
            t->ctx.uc_stack.ss_size = DEFAULT_STACK_SIZE;
            t->ctx.uc_stack.ss_flags = 0;
            t->ctx.uc_link = &(schedule.main);
            t->state = RUNNING;

            schedule.running_coroutine = id;

        case SUSPEND:
            swapcontext(&(schedule.main), &(t->ctx));
            break;
        default:;
    }
}

int schedule_finished(const Schedule_t & schedule){
    if (schedule.running_coroutine != -1){
        return 0;
    }
    else{
        for(int i=0; i<schedule.max_index; ++i){
            if (schedule.coroutines[i].state != FREE)
                return 0;
        }
    }
    return 1;
}

void checkServer(void* arg){
    int fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    sockaddr_in server;
    server.sin_family = AF_INET;
//    server.sin_port = htons(16888);
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
    printf("ret:%d  %d %d\n", ret, errno, EINPROGRESS);

    if (ret<0&& errno == EINPROGRESS){
        int maxfds;
        fd_set wset;
        maxfds = fd + 1;
        struct timeval timeout;
        timeout.tv_sec = 0;
        timeout.tv_usec = 0;
        bool isConnect = false;
        while(true){
            FD_ZERO(&wset);
            FD_SET(fd, &wset);
            ret = select(maxfds, NULL, &wset, NULL, &timeout);
            if (ret == 0){
                //coroutine
                coroutine_yield(*(Schedule_t*)arg);
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
                    coroutine_yield(*(Schedule_t*)arg);
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


void func2(void * arg)
{
    while(true){
        puts("21");
        puts("22");
        coroutine_yield(*(Schedule_t *)arg);
        //printf("%d\n", ((Schedule_t *)arg)->running_coroutine);
        puts("23");
        puts("24");
        sleep(1);

    }

}

void func3(void *arg)
{
    while(true){
        puts("31");
        puts("32");
        coroutine_yield(*(Schedule_t *)arg);
        //printf("%d\n", ((Schedule_t *)arg)->running_coroutine);
        puts("33");
        puts("34");
        sleep(1);
    }
}

void checkServer(struct schedule *S, void * ud){
    int fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    sockaddr_in server;
    server.sin_family = AF_INET;
//    server.sin_port = htons(16888);
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
    printf("ret:%d  %d %d\n", ret, errno, EINPROGRESS);

    if (ret<0&& errno == EINPROGRESS){
        int maxfds;
        fd_set wset;
        maxfds = fd + 1;
        struct timeval timeout;
        timeout.tv_sec = 0;
        timeout.tv_usec = 0;
        bool isConnect = false;
        while(true){
            FD_ZERO(&wset);
            FD_SET(fd, &wset);
            ret = select(maxfds, NULL, &wset, NULL, &timeout);
            if (ret == 0){
                //coroutine
                coroutine_yield(*(Schedule_t*)arg);
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
                    coroutine_yield(*(Schedule_t*)arg);
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

void test(struct schedule* S){
    struct
}

int  main(){
    struct schedule * S = coroutine_open();
    test(S);
    coroutine_close(S);
    return 0;
}

