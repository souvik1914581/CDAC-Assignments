#include<stdio.h>
#include<unistd.h>
#include <sys/types.h>
 #include <sys/stat.h>
 #include <fcntl.h>
#include<errno.h>
#include<stdlib.h>
#include<stdlib.h>
#include<string.h>
#include <sys/types.h>
       #include <sys/wait.h>
int main(int argc,char **argv)
{
	if(argc < 2)
	{
		printf("Insufficient arguements\n");
		exit(0);
	}
	int ret,fd;
	char *buff = (char*)malloc(sizeof(char)*64);
	pid_t id;
	fd = open("/dev/testdevice0",O_RDWR);
	if(-1 == fd)
	{
		perror("open");
		exit(0);
	}
	id = fork();
	if(-1 == id)
	{
		perror("fork");
		exit(0);
	}
	else if(0 == id)
	{
		//child
		ret = write(fd,argv[1],strlen(argv[1]));
		if(-1 == ret)
		{
			perror("write\n");
			exit(0);
		}
		printf("Wrote %d bytes to driver\n",strlen(argv[1]));
	}
	else
	{
		//parent
		int status;
		ret = wait(&status);
		ret = read(fd,buff,64);
		if(-1 == ret)
		{
			perror("read");
			exit(0);
		}
		printf("Read %s from driver\n",buff);
		exit(0);
	}
	return 0;
}
		
