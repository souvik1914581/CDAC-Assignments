#include<linux/init.h> 
#include<linux/module.h> 
#include<linux/kernel.h>
MODULE_AUTHOR("DESD");
static int intvar = 10;
static char* charvar = "default string";

module_param(charvar,charp,S_IRUGO);
module_param(intvar,int,S_IRUGO);


static int hello_init(void)
{
	printk("Hello World\nRunning init code\n");
	printk("intvar = %d\ncharvar = %s\n",intvar,charvar);
	return 0;
}
module_init(hello_init);

static int hello_exit(void)
{
	printk("Bye World!\n");
	return 0;
}
module_exit(hello_exit);

