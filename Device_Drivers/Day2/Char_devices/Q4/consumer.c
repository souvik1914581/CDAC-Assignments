#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<errno.h>
#include <sys/types.h>
       #include <sys/stat.h>
       #include <fcntl.h>

int main()
{
	int ret,fd;
	char *ch = (char*)malloc(sizeof(char));
	fd = open("/dev/testdevice0",O_RDONLY);
	if( -1 == fd)
	{
		perror("open");
		exit(0);
	}
	ret = read(fd,ch,1);
	if(ret < 1)
	{
		perror("Read");
		exit(0);
	}
	printf("Read %s from Driver\n",ch);
	return 0;
}

