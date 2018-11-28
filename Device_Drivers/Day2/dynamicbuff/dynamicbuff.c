#include<linux/init.h>
#include<linux/module.h>
#include<linux/fs.h>
#include<linux/cdev.h>
#include<linux/circ_buf.h>
#include<asm/uaccess.h>
#include<linux/wait.h>
#include<linux/sched.h>
#include<linux/slab.h>
#define SIZE 64
#define DEVCOUNT 1
static dev_t devno;
static struct cdev _cdev;
static wait_queue_head_t wq;
static struct circ_buf cbuf;
MODULE_LICENSE("GPL");
MODULE_AUTHOR("DESD");
int open_func(struct inode *inodp,struct file *filep)
{
	printk("Inside open func\n");
	return 0;
	
}

ssize_t write_func(struct file *filep,const char __user *ubuff,size_t count,loff_t *oft)
{
	int ret,i;
	int cbuff_count=count;
	cbuf.buf=kmalloc(cbuff_count,GFP_KERNEL);
        if(!cbuf.buf)
        {
                printk("error while allocating memory\n");
                unregister_chrdev_region(devno,DEVCOUNT);
                return -1;
        }

	printk("Inside write\n");
	if((count > SIZE) || (count == 0))
	{
		return -1;
	}
	for(i=0;i<count;i++)
	{
		ret=copy_from_user(cbuf.buf+cbuf.head,ubuff+i,1);	
	
		if(ret)
		{
		printk("error while writing\n");
		return -1;
		}
		printk("%c",*(char *)(cbuf.buf+cbuf.head));
		cbuf.head=(cbuf.head+1) & (cbuff_count -1);
	}
	printk("\ndriver received %d bytes\n",i);
	kfree(cbuf.buf);
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
	.write = write_func,
};

static __init int init_func(void)
{
	printk("Initialising driver\n");
	int ret;
	devno = MKDEV(104,0);
	ret = register_chrdev_region(devno,DEVCOUNT,"test3");
	if(ret)
	{
		printk("Error registering device\n");
		return -1;
	}
	cbuf.buf=kmalloc(SIZE,GFP_KERNEL);
	if(!cbuf.buf)
	{
		printk("error while allocating memory\n");
		unregister_chrdev_region(devno,DEVCOUNT);
		return -1;
	}

	
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
	kfree(cbuf.buf);
	unregister_chrdev_region(devno,DEVCOUNT);
	return 0;
}

module_init(init_func);
module_exit(exit_func);

		
	
	
