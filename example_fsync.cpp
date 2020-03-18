//
// Created by root on 19-12-5.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/time.h>
#include <cerrno>
#include <iostream>
#include <string>
using namespace std;

//https://blog.csdn.net/zhouxinlin2009/article/details/89633464
int main()
{
    struct timeval start, end;
    gettimeofday(&start, NULL);
    FILE* fp = NULL;
    fp = fopen("/data1/root/cppworkspace/ClionProjects/UC/1.txt", "w+");
    if (fp == NULL) {
        printf("fuck empty file!\n");
        exit(1);
    }

    for(int i=0; i<1000; i++) {
        char buf[1024*1024] = {64,65};
        int writeLen = fwrite(buf, sizeof(buf), 1, fp);
        //printf("writelen:%d\n", writeLen);
        int ret = fdatasync(fileno(fp));
        if (ret < 0) {
            printf("fdatasync err:%d", errno);
        }
//        int ret = fsync(fileno(fp));
//        if (ret < 0) {
//            printf("fsync err:%d", errno);
//        }

    }
    fsync(fileno(fp));

    fclose(fp);
    fp = NULL;
    gettimeofday(&end, NULL);
    printf("use time:%d", (end.tv_sec*1000000+end.tv_usec)-(start.tv_sec*1000000+start.tv_usec));
}
