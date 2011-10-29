obj-m += evspy.o
evspy-objs := kmap/kmap.o evspy-core.o cbuf.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
