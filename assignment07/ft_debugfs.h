#ifndef FT_DEBUGFS_H
#define FT_DEBUGFS_H

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/debugfs.h>
#include <linux/uaccess.h>
#include <linux/jiffies.h>
#include <linux/slab.h>
#include <linux/mutex.h>

extern const struct file_operations	id_fops;
extern const struct file_operations	foo_fops;

#endif