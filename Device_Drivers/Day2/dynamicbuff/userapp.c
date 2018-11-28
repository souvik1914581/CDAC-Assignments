#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<errno.h>
#include<stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include<string.h>
#define SIZE 64
int main(int argc,char *argv[])
{
	int fd,rw_ret;
	char ubuff[SIZE];
	fd = open("/dev/dynamic",O_RDWR,S_IRUSR|S_IWUSR);
	if(-1 == fd)
	{
		perror("Open");
		exit(0);
	}
	
	rw_ret = write(fd,argv[1],strlen(argv[1]));
	if(rw_ret >0)
	{
	printf("wrote %d bytes!\n",strlen(argv[1]));
	return 0;
	}
	else
	{
		perror("write");
		return -1;
	}
}

