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
	fd = open("/dev/testdevice0",O_WRONLY);
	if( -1 == fd)
	{
		perror("open");
		exit(0);
	}
	ret = write(fd,"A",1);
	if(ret < 1)
	{
		perror("Write");
		exit(0);
	}
	printf("Wrote 1 byte to driver\n");
	return 0;
}

