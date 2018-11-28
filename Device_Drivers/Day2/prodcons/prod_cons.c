#include<linux/init.h>
#include<linux/module.h>
#include<linux/fs.h>
#include<linux/cdev.h>
#include<linux/circ_buf.h>
#include<asm/uaccess.h>
#include<linux/wait.h>
#include<linux/delay.h>
#include<linux/sched.h>
#include<linux/slab.h>
#include<linux/semaphore.h>
#define SIZE 1
#define DEVCOUNT 1
static dev_t devno;
//static struct semaphore sem1,sem2;
static struct cdev _cdev;
volatile static char msg = '\0';
MODULE_LICENSE("GPL");
MODULE_AUTHOR("DESD");
int open_func(struct inode *inodp,struct file *filep)
{
	printk("Inside open func\n");
	return 0;
	
}
ssize_t read_func(struct file *filep,char __user *ubuff,size_t count,loff_t  *oft)
{
	//down_interruptible(&sem2);
	while(msg == '\0')
	{
		msleep(3000);
	}
	printk("Inside read\n");
	
	int ret,i;
	for(i=0;i<1;i++)
	{
		ret=copy_to_user(ubuff+i,&msg,1);
		if(ret)
		{
			printk("error while copying\n");
			return -1;
		}
		msg = '\0';
		//up(&sem1);
	}
		printk("driver wrote %d bytes\n",i);
	
	return i;

}
ssize_t write_func(struct file *filep,const char __user *ubuff,size_t count,loff_t *oft)
{
	int ret,i;
	//down_interruptible(&sem1);
	printk("Inside write\n");
	while(msg != '\0')
	{
		msleep(3000);
	}
	
	if((count > SIZE) || (count == 0))
	{
		return -1;
	}
	for(i=0;i<count;i++)
	{
		ret=copy_from_user(&msg,ubuff+i,1);	
	
		if(ret)
		{
		printk("error while writing\n");
		return -1;
		}
		//up(&sem2);
	}
	printk("driver received %d bytes\n",i);
	return i;
}

int close_func(struct inode *inodp,struct file *filep)
{
//	kfree(cbuf.buf);
	printk("Inside close\n");
	return 0;
}





static struct file_operations fops = {
	.owner = THIS_MODULE,
	.open = open_func,
	.release = close_func,
	.read = read_func,
	.write = write_func,
};

static __init int init_func(void)
{
	printk("Initialising driver\n");
	int ret;
	ret = alloc_chrdev_region(&devno,0,DEVCOUNT,"tempchar");
	if(ret)
	{
		printk("Error registering device\n");
		return -1;
	}
	/*sema_init(&sem1,1);
	sema_init(&sem2,0);*/
	cdev_init(&_cdev,&fops);
	ret = cdev_add(&_cdev,devno,DEVCOUNT);
	if(ret)
	{
		printk("Error adding to cdev\n");
		unregister_chrdev_region(devno,DEVCOUNT);
		return ret;
	}
	return 0;

}

static __exit int exit_func(void)
{
	printk("Exiting!\n");
	cdev_del(&_cdev);
	unregister_chrdev_region(devno,DEVCOUNT);
	return 0;
}

module_init(init_func);
module_exit(exit_func);

		
	
	
