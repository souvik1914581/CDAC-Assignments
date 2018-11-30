#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<errno.h>
#include<stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include<string.h>
#include <sys/ioctl.h>
#include "my_ioctl.h"
#define SIZE 15
int main(int argc,char *argv[])
{
	int fd,rw_ret,delay;
	char ubuff[SIZE];
	fd = open("/dev/test3",O_RDONLY,S_IRUSR|S_IWUSR);
	if(-1 == fd)
	{
		perror("Open");
		exit(0);
	}
	printf("Enter delay in seconds\n");
	scanf("%d",&delay);
	ioctl(fd,SET_DELAY,&delay);
	rw_ret = read(fd,ubuff,12);
	if(rw_ret > 0)
	{
	printf("read %d bytes!\n%s\n",rw_ret,ubuff);
	return 0;
	}
	else
	{
		perror("read");
		return -1;
	}
}

