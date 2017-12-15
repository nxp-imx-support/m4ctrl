#
# Copyright 2017 NXP
#
# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or
# (at your option) any later version.
#
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
