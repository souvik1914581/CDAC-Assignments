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
	fd = open("/dev/test3",O_RDONLY,S_IRUSR|S_IWUSR);
	if(-1 == fd)
	{
		perror("Open");
		exit(0);
	}
	
	rw_ret = read(fd,ubuff,atoi(argv[1]));
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

