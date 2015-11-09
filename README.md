Coding 1
*include linux/init.h and module.h

*two static functions
 
*print k for printing on kernal printk(KERN_ALERT "String")

```static int func(void){

return 0;
}
```

*to load the module function use module_init(func name)
*to call function on module removal use module_exit(func_exit)
*Create a make file to use the code
```
obj-m += file.o
KDIR= 'directory for compiler' //find by uname -r

all:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules

clean:
	rm -rf *.o *.ko *.mod.*	*.symvers *.order
```

*make
*insmod hello.ko
*rmmod hello.ko



