obj-m += evspy.o
evspy-objs := khashmap/khashmap.o evspy-core.o
#obj-m += test.o
#test-objs := khashmap.o test_khashmap.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
