#include<linux/init.h>
#include<linux/module.h>
#include<linux/fs.h>
#include<linux/cdev.h>
#define DEVCOUNT 1
static dev_t devno;
static struct cdev _cdev;
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
	return 0;

}
ssize_t write_func(struct file *filep,const char __user *ubuff,size_t count,loff_t *oft)
{

	printk("Inside write\n");
	return 0;
}

int close_func(struct inode *inodp,struct file *filep)
{
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
	devno = MKDEV(100,0);
	ret = register_chrdev_region(devno,DEVCOUNT,"chardev");
	if(ret)
	{
		printk("Error registering device\n");
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
	unregister_chrdev_region(devno,DEVCOUNT);
	return 0;
}

module_init(init_func);
module_exit(exit_func);

		
	
	
