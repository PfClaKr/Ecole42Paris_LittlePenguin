#include "ft_debugfs.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("ychun");
MODULE_DESCRIPTION("ychun made for debugfs module :)");

static struct dentry *dir;
static struct dentry *id_file;
static struct dentry *foo_file;

static int ft_error(void) {
	debugfs_remove_recursive(dir);
	return -ENOMEM;
}

static int	__init ft_debugfs_init(void) {
	if (!(dir = debugfs_create_dir("fortytwo", NULL)))
		return -ENOMEM;
	if (!(id_file = debugfs_create_file("id", 0666, dir, NULL, &id_fops)))
		return ft_error();
	#if BITS_PER_LONG == 64
		debugfs_create_u64("jiffies", 0444, dir, (u64 *) &jiffies);
	#else
		debugfs_create_u32("jiffies", 0444, dir, (u32 *) &jiffies);
	#endif
	if (!(foo_file = debugfs_create_file("foo", 0644, dir, NULL, &foo_fops)))
		return ft_error();
	
	printk(KERN_INFO "FortyTwo : fortytwo module loaded ! \n");

	return 0;
}

static void __exit ft_debugfs_exit(void) {
	debugfs_remove_recursive(dir);
	printk(KERN_INFO "FortyTwo : fortytwo module exit ! \n");
}

module_init(ft_debugfs_init);
module_exit(ft_debugfs_exit);

