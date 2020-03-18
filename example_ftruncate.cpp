//
// Created by root on 19-2-11.
//

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <stdint.h>

uint64_t file_size = 10*1024*1024*1024ULL;

int main()
{
    int fd = -1;

    fd = open("test.data.fruncate.txt", O_RDWR | O_CREAT, 0666);
    if(fd < 0){
        printf("open failed\n");
        return -1;
    }

    if(ftruncate(fd, file_size)){
        printf("ftruncate error\n");
        return -1;
    }

    lseek(fd, file_size - 1,SEEK_CUR);
    write(fd, "1", 1);

    close(fd);
    return 0;
}
