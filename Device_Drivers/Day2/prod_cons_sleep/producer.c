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
	char ubuff;
	fd = open("/dev/tempchar",O_WRONLY);
	if(-1 == fd)
	{
		perror("Open");
		exit(0);
	}
	
	ubuff=argv[1][0];
	rw_ret = write(fd,&ubuff,1);
	if(rw_ret <= 0)
	{
		perror("write");
		exit(1);
	}
	printf("wrote successfully\n");
	
	return 0;
}

