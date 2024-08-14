// SPDX-License-Identifier: BSD-3-Clause

#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/delay.h>
#include <linux/slab.h>

/*
 *  That was a long sleep, tell userspace about it
 */
int do_work(int *my_int)
{
	int y = *my_int;

	for (int i = 0; i < my_int; ++i)
		usleep_range(0, 10);

	if (y < 10)
		pr_info("We slept a long time!");
	return y * y;
}

int my_init(void)
{
	int x = 10;

	x = do_work(&x);
	return x;
}

void my_exit(void)
{
}

module_init(my_init);
module_exit(my_exit);
