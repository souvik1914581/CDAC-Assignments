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
	pid_t id;
	char ubuff[SIZE];
	fd = open("/dev/test1",O_RDWR,S_IRUSR|S_IWUSR);
	if(-1 == fd)
	{
		perror("Open");
		exit(0);
	}
	printf("Message to be sent: %s\n",argv[1]);
	id=fork();
	if(-1 == id)
	{
		perror("fork");
		exit(1);
	}
	else if(0 == id)
	{
		rw_ret = write(fd,argv[1],strlen(argv[1]));
		if(rw_ret < strlen(argv[1]))
		{
			perror("write");
			exit(1);
		}
		printf("wrote %d bytes!\n",strlen(argv[1]));
		exit(0);
	}
	else
	{
		int status;
		pid_t id;
		id = wait(&status);
		rw_ret = read(fd,ubuff,SIZE);
		if(rw_ret < 0)
		{
			perror("read!");
			exit(1);
		}
		printf("read %s from driver\n",ubuff);
		
		
	}
	
	return 0;
}

