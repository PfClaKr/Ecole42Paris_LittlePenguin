#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>

static void	test_id(void)
{
	int	fd;
	int	res;
	char	id[8];

	printf("----- id -----\n");
	fd = open("/sys/kernel/debug/fortytwo/id", O_RDWR);
	if (fd == -1)
	{
		printf("Faild to open file for id test\n");
		exit(-1);
	}

	res = write(fd, "vism", 4);
	printf("res - %d\n", res);

	res = write(fd, "vismaily", 8);
	printf("res - %d\n", res);

	memset(id, 0, 8);
	res = read(fd, id, 2);
	printf("res - %s | %d\n", id, res);

	res = read(fd, id, 2);
	printf("res - %s | %d\n", id, res);

	res = read(fd, id, 8);
	printf("res - %s | %d\n", id, res);

	res = read(fd, id, 8);
	printf("res - %s | %d\n", id, res);
	close(fd);
}

static void	test_jiffies(void)
{
	int	fd;
	char	jiffies[21];
	int	res;
	int	j;

	printf("\n----- jiffies -----\n");
	for (int i = 3; i > 0; --i)
	{
		sleep(1);
		fd = open("/sys/kernel/debug/fortytwo/jiffies", O_RDONLY);
		if (fd == -1)
		{
			printf("Faild to open file for jiffies test\n");
			exit(-1);
		}
		memset(jiffies, 0, 21);
		res = read(fd, &jiffies, 1);
		j = 1;
		while (res > 0)
		{
			res = read(fd, jiffies + j, 1);
			if (jiffies[j] == '\n')
			{
				jiffies[j] = '\0';
				break ;
			}
			++j;
		}
		printf("jiffies - %s | res - %d\n", jiffies, res);
		close(fd);
	}
}

static void	*thread_routine(void *args)
{
	int	res;
	int	fd;
	char	*foo_buff;

	int *page_size = (int *)args;
	foo_buff = (char *)calloc(*page_size, sizeof(char));
	if (!foo_buff)
		pthread_exit(NULL);
	memset(foo_buff, 0, *page_size);
	do {
		usleep(1000);
		fd = open("/sys/kernel/debug/fortytwo/foo", O_RDONLY);
		if (fd == -1)
			pthread_exit(NULL);
		res = read(fd, foo_buff, 8);
		close(fd);
		printf("foo - read res - %d, %s\n", res, foo_buff);
	} while (res > 0);
	free(foo_buff);
	return (0);
}

static void	test_foo(void)
{
	int		fd;
	int		res;
	long		page_size;
	pthread_t	thread;

	printf("\n----- foo -----\n");
	fd = open("/sys/kernel/debug/fortytwo/foo", O_WRONLY);
	if (fd == -1)
		exit(-1);
	page_size = sysconf(_SC_PAGESIZE);
	res = write(fd, "vismaily", 8);
	printf("foo - write res - %d, vismaily\n", res);
	pthread_create(&thread, NULL, thread_routine, &page_size);
	for (int i = 0; i < 100; i += 8)
	{
		usleep(1000);
		res = write(fd, "vismaily", 8);
		printf("foo - write res - %d, vismaily\n", res);
		res = write(fd, "42School", 8);
		printf("foo - write res - %d, 42School\n", res);
	}
	close(fd);
}

int	main(void)
{
	test_id();
	test_jiffies();
	test_foo();

	return (0);
}