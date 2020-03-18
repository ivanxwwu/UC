//
// Created by root on 19-12-5.
//

#include <aio.h>
#include "stdio.h"
#include "unistd.h"
#include "stdlib.h"
#include "string.h"
#include "fcntl.h"
#include "errno.h"


#define BUFFER_SIZE 1024

int MAX_LIST = 2;

void example1() {
    //aio操作所需结构体
    struct aiocb rd;

    int fd,ret,couter;

    fd = open("/root/cppworkspace/ClionProjects/UC/1.txt",O_RDONLY);
    if(fd < 0)
    {
        perror("1.txt");
    }



    //将rd结构体清空
    bzero(&rd,sizeof(rd));


    //为rd.aio_buf分配空间
    rd.aio_buf = malloc(BUFFER_SIZE + 1);

    //填充rd结构体
    rd.aio_fildes = fd;
    rd.aio_nbytes =  BUFFER_SIZE;
    rd.aio_offset = 0;

    //进行异步读操作
    ret = aio_read(&rd);
    if(ret < 0)
    {
        perror("aio_read");
        exit(1);
    }

    couter = 0;
//  循环等待异步读操作结束
    while(aio_error(&rd) == EINPROGRESS)
    {
        printf("第%d次\n",++couter);

    }
    //获取异步读返回值
    ret = aio_return(&rd);
    printf("%s\n", rd.aio_buf);

    printf("\n\n返回值为:%d",ret);
}

void example2() {
    //定义aio控制块结构体
    struct aiocb wr;

    int ret,fd;

    char str[20] = {"hello,world"};

    //置零wr结构体
    bzero(&wr,sizeof(wr));

    fd = open("/root/cppworkspace/ClionProjects/UC/1.txt",O_WRONLY | O_APPEND);
    if(fd < 0)
    {
        perror("test.txt");
    }

    //为aio.buf申请空间
    wr.aio_buf = (char *)malloc(BUFFER_SIZE);
    if(wr.aio_buf == NULL)
    {
        perror("buf");
    }

    wr.aio_buf = str;

    //填充aiocb结构
    wr.aio_fildes = fd;
    wr.aio_nbytes = sizeof(str);

    //异步写操作
    ret = aio_write(&wr);
    if(ret < 0)
    {
        perror("aio_write");
    }

    //等待异步写完成
    while(aio_error(&wr) == EINPROGRESS)
    {
        printf("hello,world\n");
    }

    //获得异步写的返回值
    ret = aio_return(&wr);
    printf("\n\n\n返回值为:%d\n",ret);
}

void example3() {
    //aio操作所需结构体
    struct aiocb rd;
    //将rd结构体清空
    bzero((char*)&rd, sizeof(struct aiocb));

    int fd,ret;

    //cblist链表
    struct aiocb *aiocb_list[2] = {NULL};

    fd = open("/root/cppworkspace/ClionProjects/UC/1.txt",O_RDONLY);
    if(fd < 0)
    {
        perror("test.txt");
    }

    //为rd.aio_buf分配空间
    rd.aio_buf = malloc(64);

    //填充rd结构体
    rd.aio_fildes = fd;
    rd.aio_nbytes =  64;
    rd.aio_offset = 0;

    //将读fd的事件注册
    aiocb_list[0] = &rd;

    //进行异步读操作
    ret = aio_read(&rd);
    if(ret < 0)
    {
        perror("aio_read");
        exit(1);
    }

    //循环等待异步读操作结束
    printf("我要开始等待异步读事件完成\n");
    //阻塞等待异步读事件完成
    ret = aio_suspend(aiocb_list, MAX_LIST, NULL);
    if (ret < 0) {
        perror("aio_suspend");
        exit(1);
    }
    puts("aio_suspend return");

    // 实际上，这个 while 循环我们不可能看到它执行的。写在这里只是为了演示。
    while(aio_error(&rd) == EINPROGRESS) {
        puts("EINPROGRESS");
    }
//    while(aio_error(&rd) == EINPROGRESS)
//    {
//        printf("第%d次\n",++couter);
//    }

    //获取异步读返回值
    ret = aio_return(&rd);
    if (ret< 0) {
        perror("aio_return");
        exit(1);
    }

    printf("\n\n返回值为:%d\n",ret);

}

void example4() {
    struct aiocb *listio[2];
    struct aiocb rd,wr;
    int fd,ret;

    //异步读事件
    fd = open("/root/cppworkspace/ClionProjects/UC/1.txt",O_RDONLY);
    if(fd < 0)
    {
        perror("test1.txt");
    }

    bzero(&rd,sizeof(rd));

    rd.aio_buf = (char *)malloc(BUFFER_SIZE);
    if(rd.aio_buf == NULL)
    {
        perror("aio_buf");
    }

    rd.aio_fildes = fd;
    rd.aio_nbytes = 1024;
    rd.aio_offset = 0;
    rd.aio_lio_opcode = LIO_READ;   ///lio操作类型为异步读

    //将异步读事件添加到list中
    listio[0] = &rd;

    //异步些事件
    fd = open("/root/cppworkspace/ClionProjects/UC/2.txt",O_WRONLY | O_APPEND);
    if(fd < 0)
    {
        perror("test2.txt");
    }

    bzero(&wr,sizeof(wr));

    wr.aio_buf = (char *)malloc(BUFFER_SIZE);
    if(wr.aio_buf == NULL)
    {
        perror("aio_buf");
    }

    wr.aio_fildes = fd;
    wr.aio_nbytes = 1024;

    wr.aio_lio_opcode = LIO_WRITE;   ///lio操作类型为异步写

    //将异步写事件添加到list中
    listio[1] = &wr;

    //使用lio_listio发起一系列请求
    ret = lio_listio(LIO_WAIT,listio,MAX_LIST,NULL);

    //当异步读写都完成时获取他们的返回值

    ret = aio_return(&rd);
    printf("\n读返回值:%d",ret);

    ret = aio_return(&wr);
    printf("\n写返回值:%d",ret);

}


void aio_completion_handler(sigval_t sigval) {
    //用来获取读aiocb结构的指针
    struct aiocb *prd;
    int ret;

    prd = (struct aiocb *) sigval.sival_ptr;

    printf("hello\n");

    //判断请求是否成功
    if (aio_error(prd) == 0) {
        //获取返回值
        ret = aio_return(prd);
        printf("读返回值为:%d\n", ret);
    }
}


void example5() {
    int fd,ret;
    struct aiocb rd;

    fd = open("/root/cppworkspace/ClionProjects/UC/1.txt",O_RDONLY);
    if(fd < 0)
    {
        perror("test.txt");
    }

    //填充aiocb的基本内容
    bzero(&rd,sizeof(rd));

    rd.aio_fildes = fd;
    rd.aio_buf = (char *)malloc(sizeof(BUFFER_SIZE + 1));
    rd.aio_nbytes = BUFFER_SIZE;
    rd.aio_offset = 0;

    //填充aiocb中有关回调通知的结构体sigevent
    rd.aio_sigevent.sigev_notify = SIGEV_THREAD;//使用线程回调通知
    rd.aio_sigevent.sigev_notify_function = aio_completion_handler;//设置回调函数
    rd.aio_sigevent.sigev_notify_attributes = NULL;//使用默认属性
    rd.aio_sigevent.sigev_value.sival_ptr = &rd;//在aiocb控制块中加入自己的引用

    //异步读取文件
    ret = aio_read(&rd);
    if(ret < 0)
    {
        perror("aio_read");
    }

    printf("异步读以开始\n");
    sleep(1);
    printf("异步读结束\n");
}

//https://blog.csdn.net/summer_zgh/article/details/82416427
int main() {
    //example1();
    //example2();
    //example3();
    //example4();
    example5();
    return 0;
}

