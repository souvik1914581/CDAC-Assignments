#include<linux/init.h>
#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/moduleparam.h>

static char *charvar = "default message";
static int intarr[3] = {1,2,3};

module_param(charvar,charp,S_IRUGO);
module_param_array(intarr,int,NULL,S_IRUGO);


static int hello_init(void)
{
	int i = 0;
	printk("Init code\nintarr = \n");
	for(i = 0 ; i < 3 ; i++)
	{
		printk("%d\t",intarr[i]);
	}
	printk("charvar = %s\n",charvar);
	return 0;
}
module_init(hello_init);

static int hello_exit(void)
{
	printk("Exit code!\n");
	return 0;
}
module_exit(hello_exit);
	

