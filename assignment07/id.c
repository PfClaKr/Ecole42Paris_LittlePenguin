#include "ft_debugfs.h"

#define LOGIN "ychun"

ssize_t id_read(struct file *file, char __user *buf, size_t count, loff_t *ppos) {
	return simple_read_from_buffer(buf, count, ppos, LOGIN, strlen(LOGIN));
}

ssize_t id_write(struct file *file, const char __user *buf, size_t count, loff_t *ppos) {
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

static const struct file_operations id_fops = {
	read : id_read,
	write : id_write,
};