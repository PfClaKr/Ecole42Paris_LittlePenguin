#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/init.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("ychun");
MODULE_DESCRIPTION("ychun made for misc char device driver :)");

#define LOGIN "ychun"

static ssize_t ft_read(struct file *file, char __user *buf, size_t count, loff_t *ppos) {
	return simple_read_from_buffer(buf, count, ppos, LOGIN, strlen(LOGIN));
}

static ssize_t ft_write(struct file *file, const char __user *buf, size_t count, loff_t *ppos) {
	char kernel_buf[32];
	ssize_t len;

	len = simple_write_to_buffer(kernel_buf, sizeof(kernel_buf) - 1, ppos, buf, count);
	if (len < 0)
		return len;

	kernel_buf[len] = '\0';

	if (strcmp(kernel_buf, LOGIN) == 0)
		return count;
	else
		return -EINVAL;
}

static const struct file_operations ft_fops = {
	.owner = THIS_MODULE,
	.read = ft_read,
	.write = ft_write,
};

static struct miscdevice ft_device;

static int __init ft_init(void) {
	int	ret;

	ft_device.minor = MISC_DYNAMIC_MINOR;
	ft_device.name = "fortytwo";
	ft_device.fops = &ft_fops;

	ret = misc_register(&ft_device);
	if (ret)
		printk(KERN_ERR "Can not register misc device. name : %s\n", "fortytwo");
	else
		printk(KERN_INFO "misc device registed. name : %s\n", "fortytwo");

	return ret;
}

static void __exit ft_exit(void) {
	misc_deregister(&ft_device);
	printk(KERN_INFO "misc device deregisted. name : %s\n", "fortytwo");
}

module_init(ft_init);
module_exit(ft_exit);
