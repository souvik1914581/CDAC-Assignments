#include<linux/module.h>
#include<linux/init.h>
#include<linux/fs.h>
#include<linux/kdev_t.h>
#include<linux/types.h>
#include<linux/cdev.h>
#include<linux/interrupt.h>
#include<linux/workqueue.h>
#include<asm/uaccess.h>
#include<linux/semaphore.h>
#define DEVNO 1
MODULE_AUTHOR("Roy");
MODULE_LICENSE("GPL");
struct cdev _cdev;

struct semaphore sem1;

int tasklet_device_major,tasklet_device_minor;
char buff[32];
static dev_t devno;

static void workqueue_handler(void *arg );
DECLARE_WORK(workqueue,workqueue_handler);

static void workqueue_handler(void* arg)
{
	
	
	char str[] = "Hello User!";
	strcpy(buff,str);
	printk("Work completed\n");
	up(&sem1);
	
	
}

ssize_t read_func(struct file *filep,char __user *ubuff,size_t count,loff_t *offset)
{
	int ret,i;
	printk("Inside read\n");
	printk("scheduling tasklet\n");
	
	schedule_work(&workqueue);
	if(down_interruptible(&sem1))
	{
		return -ERESTARTSYS;
	}
	
	ret = copy_to_user(ubuff,buff,11);
	if(ret)
	{
		printk("%d characters not copied to user\n",ret);
	}
	for(i = 0 ; i < 32; i ++)
	{
		*(buff + i) = '\0';
	}
	printk("buff cleared\n");	
	
	return 0;
}

int open_func(struct inode *inodp,struct file *filep)
{
	printk("Inside open func\n");
	//spin_lock_bh(&my_spinlock);

	return 0;

}
int close_func(struct inode *inodp,struct file *filep)
{
	printk("Inside close func\n");
	flush_work(&workqueue);
	return 0;
}


static struct file_operations fops = {
	.owner = THIS_MODULE,
	.open = open_func,
	.read = read_func,
	.release = close_func,
};


static __init int init_func(void)
{
	int ret;
	printk("Initialising driver\n");
	ret = alloc_chrdev_region(&devno,0,DEVNO,"workq_device0");
	if(ret)
	{
		printk("Error allocating dev number\n");
		return -1;
	}
	tasklet_device_major = MAJOR(devno);
	tasklet_device_minor = MINOR(devno);
	printk("Major_no. = %d\nMinor_no. = %d\n",tasklet_device_major,tasklet_device_minor);
	//EXPORT_SYMBOL(tasklet_device_major);
	//EXPORT_SYMBOL(tasklet_device_minor);
	//tasklet_init(&my_tasklet,tasklet_handler,0);
	sema_init(&sem1,0);
	cdev_init(&_cdev,&fops);
	
	ret = cdev_add(&_cdev,devno,DEVNO);
	if(ret)
	{
		printk("Error adding _cdev\n");
		unregister_chrdev_region(devno,DEVNO);
		return -1;
	}
	printk("cdev init and add done\n");
	return 0;	
}
static __exit int exit_func(void)
{
	printk("Removing driver\n");

	
	
	cdev_del(&_cdev);
	unregister_chrdev_region(devno,DEVNO);

	return 0;
}




module_init(init_func);
module_exit(exit_func);
