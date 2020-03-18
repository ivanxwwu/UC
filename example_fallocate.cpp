//
// Created by root on 19-2-11.
//

#define _GNU_SOURCE             /* See feature_test_macros(7) */
#include <fcntl.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <sys/time.h>


uint64_t file_size = 10*1024*1024*10ULL;


//int fallocate(int fd, int mode, off_t offset, off_t len);
//int posix_fallocate(int fd, off_t offset, off_t len);


int main()
{
    int fd = -1;
    int ret = -1;

    fd = open("tmp.txt", O_CREAT|O_RDWR, 0666);
    if(fd < 0){
        printf("fd < 0");
        return -1;
    }

    //ret = fallocate(fd, 0, 0, file_size);
    ret = posix_fallocate(fd, 0, file_size);
    if(ret < 0 ){
        printf("ret = %d, errno = %d,  %s\n", ret, errno, strerror(errno));
        return -1;
    }
    struct timeval start, end;
    printf("fallocate create %.2fG file\n", file_size/1024/1024/1024.0);
    char buf[100] = {1};
    gettimeofday(&start, NULL);
    for (int i=0; i<1000000; i++) {
        write(fd, buf, 100);
    }
    gettimeofday(&end, NULL);
    printf("elplace %d\n", (end.tv_sec-start.tv_sec)*1000000 + (end.tv_usec+start.tv_usec));

    close(fd);

    return 0;
}
