KERNELDIR ?=~/work/Kernel/Raspberry/buildroot-2022.02.4/output/build/linux-custom

obj-m := led_mod.o

all: clean build

build:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules

clean:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) clean
