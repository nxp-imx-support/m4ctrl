M4 Control Tool for i.MX platforms

1. Introduction

	This application is an example of controlling M4 core from A core: stop, start,
deploy a new firmware for M4 into OCRAM. Depending on the used platform, the application can
be only user space or to have light part in kernel space and one in user space.
	
Supported platforms: i.mx8qm and i.mx8qxp

2. Build Instructions

For i.mx8, it is needed to build the kernel module and user space application.

2.1 Setup the environment:
# Path to kernel building directory. It recommended to point directly to the built kernel from Yocto.
export KDIR=/work/platforms/imx/yocto/imx8/bld-x11/tmp/work/imx8qmlpddr4arm2-poky-linux/linux-imx/4.9.11-r0/build
# Architecture
export ARCH=arm64
# Path to the used cross-compiler
export CROSS_COMPILE=/home/disk2b/toolchain/gcc-linaro-aarch64-linux-gnu-4.9-2014.09_linux/aarch64-linux-gnu-
export SOC=<IMX8QXP/IMX8QM>

# Compile the kernel module

# change directory to kernel
cd kernel
# compile the module m4ctrl_dev.ko
make

# Compile the user space application
cd ../user
make

3. Run instructions

Note: 
1. This application assumes that the M4 apps are compiled for OCRAM in order to be in sync with MCU SDK release (SDK_2.2_VAL-MIMX8QM_M4).
If other memories are needed for M4 app, please update these macros from include/m4ctrl.h. Please note that some memories will likely need to
be reserved from Linux dtb in order to be excluded from the memory used by linux.

2. hello_world app for M4 was used for testing.

1. Insert the kernel module

insmod m4ctrl_dev.ko

2. Run the app:
Examples - i.mx8QM

root@imx8qmlpddr4arm2:~# ./m4ctrl --deploy=./m4_image_c0.bin --core=0

root@imx8qmlpddr4arm2:~# ./m4ctrl --stop --core=0

root@imx8qmlpddr4arm2:~# ./m4ctrl --start --core=0

root@imx8qmlpddr4arm2:~# ./m4ctrl --deploy=./m4_image_c1.bin --core=1

root@imx8qmlpddr4arm2:~# ./m4ctrl --stop --core=1

root@imx8qmlpddr4arm2:~# ./m4ctrl --start --core=1
