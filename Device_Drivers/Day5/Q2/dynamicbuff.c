#include<linux/init.h>
#include<linux/module.h>
#include<linux/fs.h>
#include<linux/cdev.h>
#include<linux/circ_buf.h>
#include<asm/uaccess.h>
#include<linux/wait.h>
#include<linux/sched.h>
#include<linux/slab.h>
#include<linux/delay.h>
#include<linux/ioctl.h>
#include<linux/jiffies.h>
#include "my_ioctl.h"
#include<linux/sched.h>
#include<linux/semaphore.h>
#include<linux/timer.h>
#define SIZE 64
#define DEVCOUNT 3

static dev_t devno,temp;

struct device 
{
	
	struct cdev _cdev;
//	int delay;
	unsigned long d;
	struct timer_list timer;
	//wait_queue_head_t wq;
	//struct circ_buf ;
};
unsigned int delay_ms;
struct device my_dev[DEVCOUNT];

MODULE_LICENSE("GPL");
MODULE_AUTHOR("DESD");


int open_func(struct inode *inodp,struct file *filep)
{
	printk("Inside open func\n");
	struct device *dev = container_of(inodp->i_cdev,struct device,_cdev);
	filep->private_data = dev;
	return 0;
	
}
/*ssize_t read_func(struct file *filep,char __user *ubuff,size_t count,loff_t *oft)
{
	
	struct device *dev = filep->private_data;
	dev->d = jiffies + (dev->delay * HZ);
	
	while(time_before(jiffies,dev->d))
	{
		schedule();
	}
	
	if(filep->f_flags & O_NONBLOCK)
	{	
		goto label;
	}
	/*if(wait_event_interruptible(dev->wq,CIRC_CNT(dev->.head,dev->.tail,SIZE)))
	{
		return -ERESTARTSYS;
	}*/
	
/*label:	printk("Inside read\nDelay end\n");
	int ret,i;
	char message[] = "Hello CDAC!";
	//m = min(count,CIRC_CNT(dev->.head,dev->.tail,SIZE));
	//printk("count = %d, circ_cnt = %d\n",count,CIRC_CNT(cbuf.head,cbuf.tail,SIZE));
	/*if(0 == m)
	{
		return -1;
	}*/
/*	for(i=0;i<11 ;i++)
        {
                ret = copy_to_user( ubuff+i,message + i ,1);       
        
                if(ret)
                {
                printk("error while writing\n");
                return -1;
                }
              
        }
	ret = copy_to_user(ubuff + i ,"\0",1);
	if(ret)
                {
                printk("error while writing\n");
                return -1;
                }

	printk("Driver wrote %d bytes to user\n",i);
	return i;
	
	

}*/
/*ssize_t write_func(struct file *filep,const char __user *ubuff,size_t count,loff_t *oft)
{
	int ret,i;
	struct device *dev = filep->private_data;

	printk("Inside write\n");
	if((count > SIZE) || (count == 0))
	{
		return -1;
	}
	for(i=0;i<count;i++)
	{
		ret=copy_from_user(dev->cbuf.buf+ dev->cbuf.head,ubuff+i,1);	
	
		if(ret)
		{
		printk("error while writing\n");
		return -1;
		}
		printk("%c",*(char *)(dev->cbuf.buf+ dev->cbuf.head));
		dev->cbuf.head=(dev->cbuf.head+1) & (SIZE -1);
	}
	//wake_up_interruptible(&dev->wq);
	printk("\ndriver received %d bytes\n",i);
	
	return i;
}*/

int close_func(struct inode *inodp,struct file *filep)
{
//	kfree(cbuf.buf);
	printk("Inside close\n");
	return 0;
}
void display(unsigned long arg)
{
        printk("Timer tick\n");
        struct device *dev=(struct device *)arg;
        mod_timer(&dev->timer,jiffies+msecs_to_jiffies(delay_ms));
}
long ioctl_func(struct file *filep,unsigned int cmd,unsigned long arg)
{
	struct device *dev=filep->private_data;
	switch(cmd)
	{
		case SETUP:
			get_user(delay_ms,(int __user*)arg);
        		setup_timer(&dev->timer,display,(long int)dev);
			mod_timer(&dev->timer,jiffies+msecs_to_jiffies(delay_ms));
			//add_timer(&dev->timer);
			printk("Timer started with delay %dms!\n",delay_ms);
			break;
		case DELETE:
			del_timer(&dev->timer);
			break;
	}
	return 0;

}





static struct file_operations fops = {
	.owner = THIS_MODULE,
	.open = open_func,
	.release = close_func,
	//.write = write_func,
//	.read = read_func,
	.unlocked_ioctl = ioctl_func,
};

static __init int init_func(void)
{
	int i = 0;
	printk("Initialising driver\n");
	int ret;
	devno = MKDEV(104,0);
	temp = devno;
	ret = register_chrdev_region(devno,DEVCOUNT,"test3");
	if(ret)
	{
		printk("Error registering device\n");
		return -1;
	}
	

	/*for(i = 0 ; i < DEVCOUNT ; i ++)
	{
		init_waitqueue_head(&my_dev[i].wq);
	}*/
	/*for(i = 0 ; i < DEVCOUNT ; i ++)
	{
		setup_timer(&my_dev[i].timer,display,(long int)&my_dev[i]);
	}*/
	for(i = 0 ; i < DEVCOUNT ; i ++)
	{
		cdev_init(&my_dev[i]._cdev,&fops);
	}
	
	for(i = 0 ; i < DEVCOUNT ; i++)
	{

		ret = cdev_add(&my_dev[i]._cdev,temp + i,DEVCOUNT);
		if(ret)
		{
			printk("Error adding to cdev\n");
			goto cdev_add_error;
		}
	}
	
		
	
	return 0;
	cdev_add_error: for(--i;i >= 0 ; i ++)
			{
				cdev_del(&my_dev[i]._cdev);
			}
			
			unregister_chrdev_region(devno,DEVCOUNT);

			return -1;			

}

static __exit int exit_func(void)
{
	printk("Exiting!\n");
	int i;
	/*for(i=0;i<DEVCOUNT;i++)
	{
		del_timer(&my_dev[i].timer);
	}*/
	for(i = 0 ; i < DEVCOUNT ; i ++)
	{
		cdev_del(&my_dev[i]._cdev);
		
	}
	unregister_chrdev_region(devno,DEVCOUNT);
	return 0;
}

module_init(init_func);
module_exit(exit_func);

		
	
	
