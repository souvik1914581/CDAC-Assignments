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
	int fd,data;
	unsigned int ch;
	fd = open("/dev/test3",O_RDWR,S_IRUSR|S_IWUSR);
	if(-1 == fd)
	{
		perror("Open");
		exit(0);
	}
	while(1)
	{
		printf("Enter command:\n1.BUF_CNT\n2.BUF_CLR\n3.BUF_DISP\n4.Exit\n");
		scanf("%d",&ch);
		switch(ch)
		{
			case 1:
				ioctl(fd,BUF_CNT,&data);
				printf("Size of circ_buf = %d\n",data);
				break;
			case 2:
				ioctl(fd,BUF_CLR);
				break;
			case 3:
				ioctl(fd,BUF_DISP);
				break;
			case 4: exit(0);
			default: printf("Invalid choice!\n");
				break;
		}
	}
	return 0;					
	
}


