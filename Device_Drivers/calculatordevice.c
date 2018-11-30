#include<linux/init.h>
#include<linux/module.h>
#include<linux/fs.h>
#include<linux/cdev.h>
#include<linux/circ_buf.h>
#include<asm/uaccess.h>
#include<linux/wait.h>
#include<linux/sched.h>
#include<linux/slab.h>
//#include<sys/ioctl.h>
#include "my_ioctl.h"
#include<linux/semaphore.h>

#define DEVCOUNT 4

dev_t devno,temp;
struct device_data
{
	struct cdev _cdev;
	int a,b,result;
	int buff[12];
	struct semaphore sem1,sem2;
};
struct device_data dev_data[DEVCOUNT];	//creating DEVCOUNT instances of struct device_data
MODULE_LICENSE("GPL");
MODULE_AUTHOR("DESD");
long ioctl_func(struct file *filep,unsigned int cmd,unsigned long arg)
{
	unsigned long ret;
	struct device_data *data = filep->private_data;
	printk("Inside iotl_func\n");
	switch(cmd)
	{
		case READ_ADD:
				if(down_interruptible(&data->sem2))
				{
					return -ERESTARTSYS; 
				}
				data->result = data->a + data->b;
				printk("a = %d\tb = %d\tresult = %d\n",data->a,data->b,data->result);

				put_user(data->result,(int __user*)arg);
				up(&data->sem1);
				break;
				
				
		case READ_SUB:
				if(down_interruptible(&data->sem2))
				{
					return -ERESTARTSYS;
				}

				data->result = data->a - data->b;
                                printk("a = %d\tb = %d\tresult = %d\n",data->a,data->b,data->result);

				put_user(data->result,(int __user*)arg);
				up(&data->sem1);
				break;
		case READ_MUL:
				if(down_interruptible(&data->sem2))
                                {
                                        return -ERESTARTSYS;
                                }

				

				data->result = data->a * data->b;
				printk("a = %d\tb = %d\tresult = %d\n",data->a,data->b,data->result);

                                put_user(data->result,(int __user*)arg);
				up(&data->sem1);
				break;
		case READ_DIV:
				if(down_interruptible(&data->sem2))
                                {
                                        return -ERESTARTSYS;
                                }

				
				data->result = data->a / data->b;
                                printk("a = %d\tb = %d\tresult = %d\n",data->a,data->b,data->result);
				put_user(data->result,(int __user*)arg);
				up(&data->sem1);
				break;
		case WRITE_ADD:
		case WRITE_SUB:
               	case WRITE_MUL:
                case WRITE_DIV:
				printk("Inside write\n");
				if(down_interruptible(&data->sem1))
				{
					return -ERESTARTSYS;
				}
                               	ret =  copy_from_user(data->buff,(int __user*)arg,64);
                                if(ret)
				{
					printk("Insufficient bytes read\n");
				}
				printk("a = %d\t b = %d\n",data->buff[0],data->buff[1]);
				data->a = data->buff[0];
                                data->b = data->buff[1];
                                up(&data->sem2);
                                break;
		default:
			printk("Invalid ioctl no");
			return -EINVAL; 
				

	}
	return 0;

}
int open_func(struct inode *inodp,struct file *filep)
{
	printk("Inside open func\n");
	struct device_data *data; 
	data = container_of(inodp->i_cdev,struct device_data,_cdev);
	filep->private_data = data;
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
	.unlocked_ioctl = ioctl_func,
};

static __init int init_func(void)
{
	printk("Initialising driver\n");
	int ret,i;
	
	ret = alloc_chrdev_region(&devno,0,DEVCOUNT,"calculatordevice");
	if(ret)
	{
		printk("Error registering device\n");
		return -1;
	}
	for(i=0;i<DEVCOUNT;i++)
	{	
		cdev_init(&dev_data[i]._cdev,&fops);
	}
	for(i = 0 ; i < DEVCOUNT ; i ++)
	{
		sema_init(&dev_data[i].sem1,1);
		sema_init(&dev_data[i].sem2,0);
	}
	temp = devno; 
	for(i = 0 ; i < DEVCOUNT ; i++)
	{
		ret = cdev_add(&dev_data[i]._cdev,temp + i,DEVCOUNT);
		if(ret)
		goto unreg;
	}	
	
	return 0;
	unreg:	for(--i ; i >=0 ; i ++)

		{
			printk("Error adding to cdev\n");
			cdev_del(&dev_data[i]._cdev);
			
			
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
		cdev_del(&dev_data[i]._cdev);
	}
	
	unregister_chrdev_region(devno,DEVCOUNT);
	return 0;
}

module_init(init_func);
module_exit(exit_func);

		
	
	
