#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("ychun");
MODULE_DESCRIPTION("ychun made for usb keyboard in :)");
MODULE_VERSION("1");

static int __init hello_init(void) {
	printk(KERN_INFO "usb keyboard in!\n");
	//pr_info(KERN_INFO "usb keyboard in!\n");
	return 0;
}

static void __exit hello_exit(void) {
	printk(KERN_INFO "usb keyboard out!\n");
	//pr_info(KERN_INFO "usb keyboard out!\n");
}

module_init(hello_init);
module_exit(hello_exit);
