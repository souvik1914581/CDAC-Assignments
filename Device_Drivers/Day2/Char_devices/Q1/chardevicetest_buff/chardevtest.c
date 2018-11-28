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
ssize_t read_func(struct file *filep,char __user *ubuff,size_t count,loff_t  *oft)
{
	printk("Inside read\n");
	int rw_ret,i,m;
	if(filep->f_flags & O_NONBLOCK)
	{
		goto label;
	}
	wait_event_interruptible(wq,CIRC_CNT(cbuf.head,cbuf.tail,SIZE)); 
label:	m = min(count,CIRC_CNT(cbuf.head,cbuf.tail,SIZE));
	for(i=0;i<m;i++)
	{
		rw_ret=copy_to_user(ubuff+i,(cbuf.buf)+(cbuf.tail),1);
		if(rw_ret)
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
	int rw_ret,i;
	printk("Inside write\n");
	if((count > SIZE) || (count == 0))
	{
		return -1;
	}
	for(i=0;i<count;i++)
	{
		rw_ret=copy_from_user(cbuf.buf+cbuf.head,ubuff+i,1);	
	
		if(rw_ret)
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
	int rw_ret;
	devno = MKDEV(101,0);
	rw_ret = register_chrdev_region(devno,DEVCOUNT,"chardev");
	if(rw_ret)
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
	rw_ret = cdev_add(&_cdev,devno,DEVCOUNT);
	if(rw_ret)
	{
		printk("Error adding to cdev\n");
		unregister_chrdev_region(devno,DEVCOUNT);
		return rw_ret;
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

		
	
	
