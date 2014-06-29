obj-m += evspy.o
evspy-objs := kmap/kmap.o evspy-core.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean

install: uninstall dkms
	mkdir -p /usr/src
	rm -rf /usr/src/evspy-0.3
	cp -R . /usr/src/evspy-0.3
	dkms install evspy/0.3
	modprobe evspy

uninstall:
	dkms --quiet remove evspy/0.3 --all || true
	modinfo evspy > /dev/null 2>&1 && rmmod evspy || true
	rm -rf /usr/src/evspy-0.3

.PHONY: all clean dkms install uninstall
