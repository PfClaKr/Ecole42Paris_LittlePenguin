#include "ft_debugfs.h"

static char foo_page[PAGE_SIZE];
static size_t foo_page_len;
static DEFINE_MUTEX(foo_mutex);

static ssize_t foo_read(struct file *file, char __user *buf, size_t count, loff_t *ppos) {
	ssize_t	ret;

	mutex_lock(&foo_mutex);
	ret = simple_read_from_buffer(buf, count, ppos, foo_page, foo_page_len);
	mutex_unlock(&foo_mutex);

	return ret;
}

static ssize_t foo_write(struct file *file, const char __user *buf, size_t count, loff_t *ppos) {
	ssize_t ret;

	if (count > PAGE_SIZE)
		return -EINVAL;
	
	mutex_lock(&foo_mutex);
	foo_page_len = count;
	ret = simple_write_to_buffer(foo_page, PAGE_SIZE, ppos, buf, count);
	mutex_unlock(&foo_mutex);

	return ret;
}

const struct file_operations foo_fops = {
	read : foo_read,
	write : foo_write,
};
