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
#include "my_ioctl.h"
#define SIZE 64
#define DEVCOUNT 3

static dev_t devno,temp;
struct device 
{
	
	struct cdev _cdev;
	//wait_queue_head_t wq;
	struct circ_buf cbuf;
};
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
ssize_t read_func(struct file *filep,char __user *ubuff,size_t count,loff_t *oft)
{
	struct device *dev = filep->private_data;
	if(filep->f_flags & O_NONBLOCK)
	{	
		goto label;
	}
	/*if(wait_event_interruptible(dev->wq,CIRC_CNT(dev->cbuf.head,dev->cbuf.tail,SIZE)))
	{
		return -ERESTARTSYS;
	}*/
	
label:	printk("Inside read\n");
	int ret,i,m;
	m = min(count,CIRC_CNT(dev->cbuf.head,dev->cbuf.tail,SIZE));
	//printk("count = %d, circ_cnt = %d\n",count,CIRC_CNT(cbuf.head,cbuf.tail,SIZE));
	if(0 == m)
	{
		return -1;
	}
	for(i=0;i<m ;i++)
        {
                ret=copy_to_user( ubuff+i, dev->cbuf.buf+ dev->cbuf.tail,1);       
        
                if(ret)
                {
                printk("error while writing\n");
                return -1;
                }
               
                dev->cbuf.tail = (dev->cbuf.tail + 1) & (SIZE -1);
        }
	ret = copy_to_user(ubuff + i ,"\0",1);
	if(ret)
                {
                printk("error while writing\n");
                return -1;
                }

	printk("Driver wrote %d bytes to user\n",m);
	return m;
	
	

}
ssize_t write_func(struct file *filep,const char __user *ubuff,size_t count,loff_t *oft)
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
}

int close_func(struct inode *inodp,struct file *filep)
{
//	kfree(cbuf.buf);
	printk("Inside close\n");
	return 0;
}
long ioctl_func(struct file *filep,unsigned int cmd,unsigned long arg)
{
	int i = 0;
	char ch;
	struct device *dev = filep->private_data;
	switch(cmd)
	{
		case BUF_CNT:
				put_user(CIRC_CNT(dev->cbuf.head,dev->cbuf.tail,SIZE),(int __user*)arg);
				break;
		case BUF_CLR:
				
				if (!capable(CAP_SYS_ADMIN))
				return -EPERM;
				for(i = 0 ; i < SIZE ; i ++)
				{
					if(CIRC_CNT(dev->cbuf.head,dev->cbuf.tail,SIZE) > 0)
					{
						ch = dev->cbuf.buf[dev->cbuf.tail];
						dev->cbuf.tail = (dev->cbuf.tail + 1) & (SIZE - 1);
					}				   
				}
				break;		
			

		case BUF_DISP:
				for(i = 0 ; i < CIRC_CNT(dev->cbuf.head,dev->cbuf.tail,SIZE) ; i ++)
				{
					printk("%c",*(char *)(dev->cbuf.buf+ dev->cbuf.head));
                			dev->cbuf.head=(dev->cbuf.head + 1) & (SIZE -1);
				}
				break;
		
		default: printk("Invalid ioctl cmd\n");
	}
	return 0;

}





static struct file_operations fops = {
	.owner = THIS_MODULE,
	.open = open_func,
	.release = close_func,
	.write = write_func,
	.read = read_func,
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
	for(i = 0 ; i  < DEVCOUNT ; i ++)
	{
		my_dev[i].cbuf.buf=kmalloc(SIZE,GFP_KERNEL);
		if(!my_dev[i].cbuf.buf)
		{
			printk("error while allocating memory\n");
			unregister_chrdev_region(devno,DEVCOUNT);
			return -1;
		}
	}

	/*for(i = 0 ; i < DEVCOUNT ; i ++)
	{
		init_waitqueue_head(&my_dev[i].wq);
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
			for(i = 0 ; i < DEVCOUNT ; i ++)
			{
				kfree(my_dev[i].cbuf.buf);
			}
			unregister_chrdev_region(devno,DEVCOUNT);

			return -1;			

}

static __exit int exit_func(void)
{
	printk("Exiting!\n");
	int i;
	for(i = 0 ; i < DEVCOUNT ; i ++)
	{
		cdev_del(&my_dev[i]._cdev);
		kfree(my_dev[i].cbuf.buf);
	}
	unregister_chrdev_region(devno,DEVCOUNT);
	return 0;
}

module_init(init_func);
module_exit(exit_func);

		
	
	
