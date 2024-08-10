obj-m += ft_module.o

ft_module-y := srcs/ft_debugfs.o srcs/foo.o srcs/id.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

re:	clean all

.PHONY : all clean re
