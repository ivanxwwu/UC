//
// Created by root on 18-12-14.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
char buf[10];
char *ptr;
int main()
{
    int fd;
    fd = shm_open("region", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    if (fd<0) {
        printf("error open region\n");
        return 0;
    }
    ftruncate(fd, 10);
    ptr = (char *)mmap(NULL, 10, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (ptr == MAP_FAILED) {
        printf("error map\n");
        return 0;
    }
    while (*ptr != 0x12);
    printf("ptr : %d\n", *ptr);
    return 0;
}
