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

int main(int argc,char *argv[])
{
	int fd,delay,ch;
	
	fd = open("/dev/test3",O_RDONLY,S_IRUSR|S_IWUSR);
	if(-1 == fd)
	{
		perror("Open");
		exit(0);
	}
	while(1)
	{
	printf("Enter choice:\n1.Setup and start timer\n2.Delete timer\n3.Exit\n");
	scanf("%d",&ch);
	switch(ch)
	{
		case 1:
			printf("Enter delay in milliseconds\n");
			scanf("%d",&delay);
			ioctl(fd,SETUP,&delay);
			break;
		case 2:
			ioctl(fd,DELETE);
			printf("Timer deleted!\n");
			break;
		case 3:
			ioctl(fd,DELETE);
			printf("Timer deleted!\n");
			break;
		default: printf("Invalid input\n");
	}
	}
	return 0;	
}

