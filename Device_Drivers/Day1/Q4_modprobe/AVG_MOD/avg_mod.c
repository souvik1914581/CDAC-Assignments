#include<linux/init.h>
#include<linux/module.h>
#include "add_header.h" 
static int avg;
static int init_func(void)
{
	int sum=add(5,6);
	avg=sum/2;
	printk("average is %d\n",avg);
	return 0;
}
module_init(init_func);
static int exit_func(void)
{
	printk("exit func:exit\n");
	return 0;
}
module_exit(exit_func);

