#include<linux/module.h>
#include<linux/kernel.h>
#include<linux/fs.h>
#include<linux/cdev.h>
#include<linux/semaphore.h>
#include<asm/uaccess.h>


struct fake_device {
	char data[100];
	struct semaphore sem; //device corruption prevention
}virtual_device;


struct cdev *mcdev; //device driver
int major_number; //major number
int ret; //check the errors

dev_t dev_num;

#define DEVICE_NAME 'vivekdevice' //name of device driver


int device_open(struct inode *inode, struct file *filep)
{
	if(down_interruptible(&virtual_device.sem) != 0){
		printk(KERN_ALERT "Couldnt lock device");
		return -1;
	}
	printk(KERN_INFO "Opened device");
	return 0;
}


ssize_t device_read(struct file* filp, char* bufStoreData, size_t bufCount, loff_t* curOffset){
	printk(KERN_INFO "Reading from device");
	ret = copy_to_user(bufStoreData, virtual_device.data,bufCount);
	return ret;
}

ssize_t device_write(struct file* filp, char* bufStoreData, size_t bufCount, loff_t* curOffset){
	printk(KERN_INFO "Reading from device");
	ret = copy_to_user(virtual_device.data,bufStoreData,bufCount);
	return ret;
}


int device_close(struct inode *inode, struct file *filep)
{
	up(&virtual_device.sem); 
	printk(KERN_INFO "Closed device");
	return 0;
}



struct file_operations fops ={
	.owner = THIS_MODULE,
	.open = device_open,
	.release= device_close,
	.write =  device_write,
	.read = device_read
};

static int driver_entry(void){
	ret=alloc_chrdev_region(&dev_num,0,1,DEVICE_NAME);
	if(ret < 0){
		printk(KERN_ALERT "Failed to allocate a major number");
		return ret;
	}
	major_number=MAJOR(dev_num);
	printk(KERN_INFO "crearte a device file of name %s and number %d ",DEVICE_NAME,major_number);
	mcdev = cdev_alloc();
	mcdev->ops=&fops;
	mcdev->owner=THIS_MODULE;
	ret=cdev_add(mcdev,dev_num,1);
	if(ret < 0)
	{
		printk(KERN_ALERT "Couldnt add the cdev to kernel");
		return ret;
	}
	sema_init(&virtual_device.sem,1);
	return 0;
}


static void driver_exit(void){
	cdev_del(mcdev);
	unregister_chrdev_region(dev_num,1);
	printk(KERN_ALERT "character device driver removed");
}
	
module_init(driver_entry);
module_exit(driver_exit);
