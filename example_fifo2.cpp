//
// Created by root on 18-12-15.
//

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>
#define _PATH_ "/tmp/file.tmp"
#define _SIZE_ 100
int main()
{
    int fd = open(_PATH_, O_RDONLY);
    if(fd < 0){
        printf("open file error!\n");
        return 1;
    }
    char buf[_SIZE_];
    memset(buf, '\0', sizeof(buf));
    while(1){
        int ret = read(fd, buf, sizeof(buf));
        if (ret <= 0)//error or end of file
        {
            printf("read end or error!\n");
            break;
        }
        printf("%s\n", buf);
        if( strncmp(buf, "quit", 4) == 0 )
        {
            break;
        }
    }
    close(fd);
    return 0;
}
