
#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/gpio.h>
#include <linux/interrupt.h>


#define GPIO_INT_PIN 115
#define GPIO_LED 49
#define MAJORNO 42
#define N_MINORS 1
#define SIZE 32

MODULE_LICENSE("GPL");
MODULE_AUTHOR("soni sneha");

static dev_t devno;
static struct cdev _cdev;
static int irq_num;
int value = 0;

irq_handler_t gpio_handler(int irq, void* dev_id, struct pt_regs *regs)
{
        printk("\nGot interrupt\n");
        value = (!value);
        gpio_set_value(GPIO_LED, value);
        return (irq_handler_t)IRQ_HANDLED;
}

static __init int char_dev_init(void)
{
        int ret, ret_I, ret_L;
        devno = MKDEV(MAJORNO, 0);

        printk("Inside Init");
        ret = register_chrdev_region(devno, 1, "testdev");

        if(ret)
        {
           printk("Error registering device number\n");
           return ret;
        }

        ret_I = gpio_is_valid(GPIO_INT_PIN);
        ret_L = gpio_is_valid(GPIO_LED);
       
        ret_I = gpio_request(GPIO_INT_PIN,"SYS_115");
        ret_L = gpio_request(GPIO_LED,"SYS_49");
       
        if(ret_I & ret_L)
        {
           printk("\nError in requesting gpio\n");
        }

        gpio_direction_input(GPIO_INT_PIN);
        gpio_direction_output(GPIO_LED, 0);

        ret_I = gpio_set_debounce(GPIO_INT_PIN,200);

        if(ret_I)
        {
            printk("\nError in setting debounce\n");
            return ret_I;
        }

        irq_num= gpio_to_irq(GPIO_INT_PIN);

        if(irq_num<0)
        {
            printk("\n Error in getting irq number\n");
            return -1;
        }

        ret=request_irq(irq_num,(irq_handler_t) gpio_handler,IRQF_TRIG$

        ret = cdev_add(&_cdev, devno, 1);
        if(ret)
        {
           unregister_chrdev_region(devno, 1);
           return ret;
        }

        return 0;
}

static __exit void char_dev_exit(void)
{
        printk("\nExit\n");

        cdev_del(&_cdev);
        gpio_free(GPIO_INT_PIN);
        gpio_free(GPIO_LED);

        unregister_chrdev_region(devno, 1);
}

module_init(char_dev_init);
module_exit(char_dev_exit);

