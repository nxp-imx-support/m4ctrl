KDIR ?= /work/platforms/imx/yocto/imx8/bld-x11/tmp/work/imx8qmlpddr4arm2-poky-linux/linux-imx/4.9.11-r0/build/
ARCH ?= arm64
CROSS_COMPILE ?= aarch64-linux-gnu-
kbuild:
	make ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) -C $(KDIR) M=`pwd`

clean:
	make ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) -C $(KDIR) M=`pwd` clean
	-rm -f *~ Module.symvers Module.markers modules.order
