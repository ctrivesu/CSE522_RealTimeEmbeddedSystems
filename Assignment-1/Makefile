TOOLDIR = /opt/iot-devkit/1.7.2/sysroots/x86_64-pokysdk-linux/usr/bin/i586-poky-linux
ARCH = x86
CROSS_COMPILE = /opt/iot-devkit/1.7.2/sysroots/x86_64-pokysdk-linux/usr/bin/i586-poky-linux/i586-poky-linux-

ifeq	($(TEST_TARGET), Galileo2)
	CC = $(TOOLDIR)/i586-poky-linux-gcc
	MAKE = make ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE)
	KDIR=/opt/iot-devkit/1.7.2/sysroots/i586-poky-linux/usr/src/kernel
else
SDK_HOME = /opt/iot-devkit/1.7.2/sysroots
PATH := $(PATH):$(SDK_HOME)/x86_64-pokysdk-linux/usr/bin/i586-poky-linux
CC =gcc
ARCH = x86
CROSS_COMPILE = i586-poky-linux-
SROOT=$(SDK_HOME)/i586-poky-linux/

endif

all: Code-Final.c
	$(CC) -o CP.o Code-Final.c -fno-stack-protector -lpthread -Wall  --sysroot=$(SROOT)
	sudo ./CP.o
	

clean:
	rm -f *.o


