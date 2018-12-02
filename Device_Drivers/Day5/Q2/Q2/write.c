
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include<stdio.h>
int main()
{

//	char buff[20];
        int fd;
        char buff[20];
        fd=open("/dev/q1_0",O_RDWR,S_IRWXU);
        read(fd,buff,23);
        return 0;
}



