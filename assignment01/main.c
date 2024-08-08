#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("ychun");
MODULE_DESCRIPTION("ychun made for hello to world :)");

static int __init hello_init(void) {
	printk(KERN_INFO "Hello world!\n");
	return 0;
}

static void __exit hello_exit(void) {
	printk(KERN_INFO "Cleaning up module.\n");
}

module_init(hello_init);
module_exit(hello_exit);

// https://blog.naver.com/jay_korea/30033163575