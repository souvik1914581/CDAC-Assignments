TARGET_DEVICE=q2
obj-m=$(TARGET_DEVICE).o
KERN_DIR=/lib/modules/$(shell uname -r)/build
PWD=$(shell pwd)
modules:
	$(MAKE) -C ${KERN_DIR} M=${PWD} modules
clean:
	$(MAKE) -C ${KERN_DIR} M=${PWD} clean
add:
	insmod $(TARGET_DEVICE).ko
	mknod /dev/$(TARGET_DEVICE)_0 c 43 0
	mknod /dev/$(TARGET_DEVICE)_1 c 43 1
	mknod /dev/$(TARGET_DEVICE)_2 c 43 2
	dmesg -C
remove:
	rmmod $(TARGET_DEVICE)
	rm /dev/$(TARGET_DEVICE)_0
	rm /dev/$(TARGET_DEVICE)_1
	rm /dev/$(TARGET_DEVICE)_2
write:
	gcc -o write0 write0.c
	gcc -o write1 write1.c
read:
	gcc -o read0 read.c

