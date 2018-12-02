#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<unistd.h>
#include <sys/types.h>
       #include <sys/stat.h>
       #include <fcntl.h>

#define BUFFSIZE 32
int main()
{
	char *buff = (char*)malloc(sizeof(char)*BUFFSIZE);
	int fd,ret;
	fd = open("/dev/tasklet_device0",O_RDWR,S_IRUSR|S_IWUSR);
	printf("fd = %d\n",fd);
	if(-1 == fd)
	{
		perror("Open");
		exit(1);
	}
	ret = read(fd,buff,BUFFSIZE);
	if(ret < 0)
	{
		perror("read");
		exit(1);
	}
	printf("Message from tasklet:\n%s\n",buff);
	return 0;
}
