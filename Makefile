obj-m += deviced.o

KDIR = /usr/src/linux-headers-3.13.0-67-generic


all:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules

clean:
	rm -rf *.o *.ko *.mod.*	*.symvers *.order
