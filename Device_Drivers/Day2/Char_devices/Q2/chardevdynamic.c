#include<linux/init.h>
#include<linux/module.h>
#include<linux/slab.h>
#include<linux/fs.h>
#include<linux/sched.h>
#include<linux/types.h>
#include<linux/circ_buf.h>
#include<linux/kdev_t.h>
#include<linux/cdev.h>
#include<asm/uaccess.h>
#define MAXDEVICES 1
#define SIZE 64

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Roy");

static struct circ_buf cbuf;
static wait_queue_head_t wq;
static dev_t devno;


int open_func(struct inode *myinode,struct file *filep)
{
	printk("Inside open function\n");
	return 0;
}
ssize_t read_func(struct file *filep,char __user *ubuff,size_t count,loff_t *oft)
{
	
	printk("Inside read\n");
	int ret,i,m;
	if(filep->f_flags & O_NONBLOCK)
	{
		goto label;
	}
	wait_event_interruptible(wq,CIRC_CNT(cbuf.head,cbuf.tail,SIZE)); 
label:	m = min(count,CIRC_CNT(cbuf.head,cbuf.tail,SIZE));
	for(i=0;i<m;i++)
	{
		ret=copy_to_user(ubuff+i,(cbuf.buf)+(cbuf.tail),1);
		if(ret)
		{
			printk("error while copying\n");
			return -1;
		}
		cbuf.tail=(cbuf.tail+1) & (SIZE-1);
	}
		printk("driver wrote %d bytes\n",m);
	
	return m;

}
ssize_t write_func(struct file *filep,const char __user *ubuff,size_t count,loff_t *oft)
{
	int ret,i;
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
		cbuf.head=(cbuf.head+1) & (SIZE -1);
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

static struct file_operations fileops = {
	.open = open_func,
	.read = read_func,
	.write = write_func,
	.release = close_func,
	.owner = THIS_MODULE,
};
static struct cdev my_cdev;
//my_cdev->ops = &fileops;





static int init_func(void)
{
	printk("Initialising driver\n");
	int ret;
	ret = alloc_chrdev_region(&devno,0,MAXDEVICES,"chardevdynamic");
	if(ret < 0)
	{
		printk("Error getting devno\n");
		return ret;
	}
	cbuf.buf = kmalloc(SIZE,GFP_KERNEL);
		
	if(!cbuf.buf)
	{
		printk("error allocating buffer\n");
		unregister_chrdev_region(devno,MAXDEVICES);
		return -1;
	}
	cdev_init(&my_cdev,&fileops);
	ret = cdev_add(&my_cdev,devno,MAXDEVICES);
	if(ret < 0)
	{
		printk("Error cdev_add\n");
		kfree(cbuf.buf);
		unregister_chrdev_region(devno,MAXDEVICES);
		cdev_del(&my_cdev);
		return ret;
	}
	return 0;
}
static __exit int exit_func(void)
{
	printk("Exiting!\n");
	cdev_del(&my_cdev);
	kfree(cbuf.buf);
	unregister_chrdev_region(devno,MAXDEVICES);
	return 0;
}


module_init(init_func);
module_exit(exit_func);
