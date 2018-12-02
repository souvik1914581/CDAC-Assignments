obj-m := dynamicbuff.o

KERNELDIR = /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) clean
	rm -rf /dev/test3
	rmmod dynamicbuff
install:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules_install
	depmod -a
	modprobe dynamicbuff
	mknod /dev/test3 c 104 0
