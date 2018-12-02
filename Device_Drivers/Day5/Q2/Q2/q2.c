
#include<linux/circ_buf.h>
#include<linux/slab.h>
#include<asm/uaccess.h>
#include<linux/init.h>
#include<linux/fs.h>
#include<linux/cdev.h>
#include<linux/module.h>
#include<linux/kdev_t.h>
#include<linux/types.h>
#include<linux/wait.h>
#include<linux/sched.h>
#include<linux/jiffies.h>
#include<linux/delay.h>
//#include "q1_ioctl.h"
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Q1");
#define SIZE 32
	unsigned long curr_time;
	static wait_queue_head_t wq;
	struct circ_buf cbuf;
	static dev_t devno;
	static struct timer_list ker_timer;
	static void timer_function(unsigned long expires)
	{
	printk("in kernel timers\n");
        mod_timer(&ker_timer,jiffies+msecs_to_jiffies(2000));
	//mod_timer(&ker_timer,expires);
	}
/*	long q1_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
	{
	switch(cmd)
	{
		case DELAY_DATA:
		{
			printk("in DELAY DATA\n");
			get_user(curr_time,(unsigned long __user *)arg);
			break;
		}
		default:
			printk("unknown choice\n");
	}
	return 0;
	}
*/
	static ssize_t q1_read (struct file *file, char __user *ubuff, size_t count, loff_t *off)		//creating rad

	{
		int i , ret;
		printk("in read\n");
		wait_event_interruptible(wq,CIRC_CNT(cbuf.head,cbuf.tail,SIZE));
		while(time_before(jiffies,curr_time*HZ));
		printk("In jiffies\n"); 
              	for( i=0;i<count;i++)
		{
			ret = copy_to_user(ubuff+i,cbuf.buf+cbuf.tail,1);//
			if(-1 == ret)
			{
				printk("\ncopy to user error\n");
				return -1;
			}
	//		printk("after buff=%c\n",buff[0]);
		cbuf.tail=(cbuf.tail+1)&(SIZE-1);
		}
		printk("\n read - %s \n",ubuff);
		return 0;
	}
	static ssize_t q1_write (struct file *file, char __user *ubuff, size_t count, loff_t *off)		//creating write
	{
		int i , ret;
//		char *buff;
//		buff=kmalloc(count,GFP_KERNEL);
		printk("in write\n");
		for(i=0;i<count;i++)
		{
			ret =  copy_from_user(cbuf.buf+cbuf.head, ubuff+i, 1);//
			if(-1 == ret)
                        {
                                printk("\ncopy from user error\n");
                               return -1;
                        }
                cbuf.head=(cbuf.head+1)&(SIZE-1);

		}
		 printk("\nwrote - %s %zd bytes\n", ubuff , count);
		wake_up(&wq);
		return 0;
	}
	static int q1_open(struct inode *inode, struct file *file)					//open
	{
		printk("in open\n");
		return 0;
	}
	static int q1_close(struct inode *inode, struct file *file)					//close
	{
		printk("in close\n");
		return 0;
	}
	static struct cdev cdev_s;									//assaigning structure for cdev
	static struct file_operations file_opers={							//structure for fops
	.owner= THIS_MODULE,
	.open= q1_open,
	.read= q1_read,
	.release = q1_close,
	.write=q1_write,
//	.unlocked_ioctl = q1_ioctl,
	};

	static int q1_init(void) {
	int ret;
	devno = MKDEV(43,0);


	register_chrdev_region(devno,1,"q1_driver");
	if(ret)
	{
		printk("error in registering a device\n");
		return ret;
	}
	cbuf.buf=kmalloc(SIZE,GFP_KERNEL);
	if(!cbuf.buf)
	{
		printk("memory is not alloted\n");
		return -1;
	}
	init_waitqueue_head(&wq);
	cdev_init(&cdev_s, &file_opers);
//	if(ret)
//	{
//		printk("error in initliasing cdev\n");
//		return ret;
//	}
	setup_timer(&ker_timer,timer_function,0);
	add_timer(&ker_timer);
	mod_timer(&ker_timer,jiffies+msecs_to_jiffies(2000));
	ret= cdev_add(&cdev_s,devno,1);
	if(ret)
	{
		printk("error\n");
		return ret;
	}
	return 0;

} 
static void q1_exit(void)
{
	cdev_del(&cdev_s);
	unregister_chrdev_region(devno,1);
	del_timer_sync(&ker_timer);
}
module_init(q1_init);
module_exit(q1_exit);
