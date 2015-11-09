#include<linux/init.h>
#include<linux/module.h>
#include<linux/moduleparam.h>

int param_var=0;

module_param(param_var,int,S_IRUSR);

static int hello_init(void){
	printk(KERN_ALERT "Hello workd. this is soliduscode %d\n",param_var);
	return 0;
}


static void hello_exit(void)
{
	printk(KERN_ALERT "TEST, Goodbye");
}


module_init(hello_init);
module_exit(hello_exit);
