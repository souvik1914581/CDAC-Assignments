#include<stdio.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include<errno.h>
#include<string.h>
#include<stdlib.h>
#include "my_ioctl.h"
int main(int argc,char *argv[])
{
	
	if(argc < 3)
	{
		printf("Invalid inputs\n");
		exit(0);
	}
	int *buff = (int *)malloc(sizeof(int)*2);
	buff[0] = atoi(argv[1]);
	buff[1] = atoi(argv[2]);
	int ch,ret,fd,result;
	printf("Entered numbers : %d %d\n",buff[0],buff[1]);
	while(1)
	{
		printf("Enter choice\n1.Write add\n2.Read add\n3.Write Mul\n4.Read Mul\n5.Write sub\n6.Read sub\n7.Write div\n8.Read div\n9.Exit");
		scanf("%d",&ch);
		switch(ch)
		{
			case 1:
				fd = open("/dev/add",O_WRONLY);
				if(-1 == fd)
				{
					perror("open");
					exit(1);
				}
				ret = ioctl(fd,WRITE_ADD,buff);
				bzero(buff,12);
				if(-1 == ret)
				{
					perror("ioctl");
					exit(1);
				}
				break;
		 	case 2:
                        	fd = open("/dev/add",O_RDONLY);
                        	if(-1 == fd)
                        	{
                                	perror("open");
                                	exit(1);
                        	}
                        	ret = ioctl(fd,READ_ADD,&result);
                        	if(-1 == ret)
                        	{
                                	perror("ioctl");
                                	exit(1);
                        	}
				printf("Sum = %d",result);
				bzero(buff,12);
                        	break;

		case 3:
                        fd = open("/dev/mul",O_WRONLY);
                        if(-1 == fd)
                        {
                                perror("open");
                                exit(1);
                        }
                        ret = ioctl(fd,WRITE_MUL,buff);
                        if(-1 == ret)
                        {
                                perror("ioctl");
                                exit(1);
                        }
			
                        break;
		case 4:
			  fd = open("/dev/mul",O_RDONLY);
                                if(-1 == fd)
                                {
                                        perror("open");
                                        exit(1);
                                }
                                ret = ioctl(fd,READ_MUL,&result);
                                if(-1 == ret)
                                {
                                        perror("ioctl");
                                        exit(1);
                                }
                                printf("product= %d",result);
                                bzero(buff,12);
                                break;
		case 5:
                        fd = open("/dev/sub",O_WRONLY);
                        if(-1 == fd)
                        {
                                perror("open");
                                exit(1);
                        }
                        ret = ioctl(fd,WRITE_SUB,buff);
                        if(-1 == ret)
                        {
                                perror("ioctl");
                                exit(1);
                        }
                        
                        break;


		case 6:
                          fd = open("/dev/sub",O_RDONLY);
                                if(-1 == fd)
                                {
                                        perror("open");
                                        exit(1);
                                }
                                ret = ioctl(fd,READ_SUB,&result);
                                if(-1 == ret)
                                {
                                        perror("ioctl");
                                        exit(1);
                                }
                                printf("difference= %d",result);
                                bzero(buff,12);
                                break;
		
		case 7:
                        fd = open("/dev/div",O_WRONLY);
                        if(-1 == fd)
                        {
                                perror("open");
                                exit(1);
                        }
                        ret = ioctl(fd,WRITE_DIV,buff);
                        if(-1 == ret)
                        {
                                perror("ioctl");
                                exit(1);
                        }

                        break;
		case 8:
                          fd = open("/dev/div",O_RDONLY);
                                if(-1 == fd)
                                {
                                        perror("open");
                                        exit(1);
                                }
                                ret = ioctl(fd,READ_DIV,&result);
                                if(-1 == ret)
                                {
                                        perror("ioctl");
                                        exit(1);
                                }
                                printf("division= %d",result);
                                bzero(buff,12);
                                break;
		case 9: 	exit(0);
		default:	printf("invalid choice\n");
		}//switch close
	
	}//while close
	return 0;
}//main close
