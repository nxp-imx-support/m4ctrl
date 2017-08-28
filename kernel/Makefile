KDIR ?= /work/platforms/imx/yocto/imx8/build-fb/tmp/work/imx8qxplpddr4arm2-poky-linux/linux-imx/4.9.11-r0/build/
ARCH ?= arm64
CROSS_COMPILE ?= aarch64-linux-gnu-
SOC ?= IMX8QM

kbuild:
	$(info Building m4ctrl_dev for $(SOC))
	make SOC=$(SOC) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) -C $(KDIR) M=`pwd`

clean:
	make ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) -C $(KDIR) M=`pwd` clean
	-rm -f *~ Module.symvers Module.markers modules.order
