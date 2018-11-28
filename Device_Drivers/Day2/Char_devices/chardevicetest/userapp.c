#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<errno.h>
#include<stdlib.h>
int main()
{
	int fd,rw_ret;
	fd = open("/dev/test0",O_RDWR,S_IRUSR|S_IWUSR);
	if(-1 == fd)
	{
		perror("Open");
		exit(0);
	}
	rw_ret = read(fd,NULL,0);
	rw_ret = write(fd,NULL,0);
	return 0;
}

