#include<linux/init.h>
#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/moduleparam.h>
#include<linux/slab.h>
struct demo
{
	 int intval;
	 char string[20];
};

struct demo *structvar;
char *argv[] = {"1234","Default Message\n"};
int i = 0;


module_param_array(argv,charp,NULL,S_IRUGO);
static int hello_init(void)
{
	int Var;
	printk ("ArgV: %s, %s\n ", argv[0], argv[1]);
	Var =  argv[0][0] - 48;
	argv[0] =  Var;
	argv[1][0] = '\0';
	structvar = (struct demo *)argv;
	printk("string = %s\n", structvar->string);
	printk("inval = %d\n",structvar->intval);
	return 0;
}
module_init(hello_init);
//module_param_array(argv,charp,NULL,S_IRUGO);
static int hello_exit(void)
{
	typedef struct demo demo;

	printk("Exit code!\n");
	return 0;
}
module_exit(hello_exit);
	

