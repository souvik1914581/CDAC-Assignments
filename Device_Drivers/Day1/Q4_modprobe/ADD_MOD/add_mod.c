#include<linux/init.h>
#include<linux/module.h>
#include<linux/moduleparam.h>
#include "add_header.h"
static int a = 5;
static int b = 6;

static int add(int a,int b)
{
	return a+b;
}

EXPORT_SYMBOL(add);

static int init_func(void)
{
	
	printk("Init function,Sum = %d\n",add(a,b));
	return 0;
}
module_init(init_func);
static int exit_func(void)
{
	printk("Add func exit!");
	return 0;
}
module_exit(exit_func);


