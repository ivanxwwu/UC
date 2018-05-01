//
// Created by root on 17-8-30.
//

#include <stdio.h>
#include <sys/mman.h>
#include <stdlib.h>
#include <zconf.h>
int main()
{
    const int PSZ = getpagesize();
    printf("PROT_READ=%d\n", PROT_READ);
    printf("PROT_WRITE=%d\n", PROT_WRITE);
    printf("PROT_EXEC=%d\n", PROT_EXEC);
    int* p = (int*)mmap(
            0,/*指定内存空间的首地址，如果为0则由系统自动选择，一般为0*/
            1*PSZ,/*映射内存空间的大小，应为页面的整数倍，如果不是整数倍，系统也会自动补齐到整数倍*/
            PROT_READ|PROT_WRITE,/*保护方式,读，写，执行*/
            MAP_SHARED|MAP_ANONYMOUS,/*设置映射内存的特性*/
            0,/*文件描述符，用于映射文件用，如果指定了MAP_ANONYNOUS，此参数无效*/
            0);/*映射文件时指定的文件偏移量*/
    if(*p==-1){
        perror("映射失败");
        exit(-1);
    }
    int i;
    for(i=0; i<100; i++){
        p[i] = i+1; //*(p+i) = i+1;
    }
    for(i=0; i<200; i++){
        printf("%d ", p[i]);
        if((i+1)%10==0)printf("\n");
    }
    *(p+1024+1) = 1234;
    printf("pid=%d\n", getpid());
    printf("p=%p\n", p);
    sleep(20);
    munmap(p, 1*PSZ);
    while(1);
}