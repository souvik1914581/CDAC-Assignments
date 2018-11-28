#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<errno.h>
#include<stdlib.h>
#include <sys/wait.h>
#include <unistd.h>
#include<string.h>
#define SIZE 1
int main(int argc,char *argv[])
{
	int fd,rw_ret;
	char ubuff[SIZE];
	fd = open("/dev/tempchar",O_RDONLY);
	if(-1 == fd)
	{
		perror("Open");
		exit(0);
	}
	
	rw_ret = read(fd,ubuff,1);
	if(rw_ret <= 0)
	{
		perror("read");
		exit(1);
	}
	printf("received msg is %c\n",ubuff[0]);
	
	return 0;
}

