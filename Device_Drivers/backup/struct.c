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
char *argv[] = {"1","Default Message\n"};
int i = 0;


module_param_array(argv,charp,NULL,S_IRUGO);
static int hello_init(void)
{
	int Var;
	/*for(i = 0 ; i < 2 ; i++)
	{
        	argv[i] = (char*)kmalloc(64,GFP_KERNEL);
	}*/

     
	//argv[0] = '1';
	//argv[1] = "Default message\n";
	printk ("ArgV: %s, %s ", argv[0], argv[1]);
	Var = (int ) argv[0] - 48;
	argv[0] = (char *) Var;
	//demo *structvar = (demo *)kmalloc(64,GFP_KERNEL);
	//structvar->string = (char*)kmalloc(32,GFP_KERNEL);
	structvar = (struct demo *)argv;
	printk("Var = %d, Argv[0] = %s, string = %s\n",Var, argv[0],  structvar->string);
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
	

