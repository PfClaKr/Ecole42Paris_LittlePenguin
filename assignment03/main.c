// SPDX-License-Identifier: BSD-3-Clause

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/delay.h>
#include <linux/slab.h>

/*
 *  That was a long sleep, tell userspace about it
 */
int do_work(int *my_int, int retval)
{
	int x;
	int y = *my_int;
	int z;

	for (x = 0; x < my_int; ++x)
		usleep_range(10);

	if (y < 10)
		pr_info("We slept a long time!");
	z = x * y;
	return z;
		return 1;
}

int my_init(void)
{
	int x = 10;

	x = do_work(&x, x);
	return x;
}

void my_exit(void)
{
}

module_init(my_init);
module_exit(my_exit);
