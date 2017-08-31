# M4 Control Tool for i.MX platforms

## 1. Introduction
This application is an example of controlling M4 core from A core: stop, start,
deploy a new firmware for M4 into TCM. Depending on the used platform, the application can
be only user space or to have a light component in kernel space and one in user space.

Supported platforms: i.mx8qm and i.mx8qxp

## 2. Build Instructions

For i.mx8, it is needed to build the kernel module and user space application.

### 2.1 Setup the environment:
#### # Path to kernel building directory. It recommended to point directly to the built kernel from Yocto.
export KDIR=/work/platforms/imx/yocto/imx8/bld-x11/tmp/work/imx8qmlpddr4arm2-poky-linux/linux-imx/4.9.11-r0/build
#### # Architecture
export ARCH=arm64
#### # Path to the used cross-compiler
export CROSS_COMPILE=/home/disk2b/toolchain/gcc-linaro-aarch64-linux-gnu-4.9-2014.09_linux/aarch64-linux-gnu-
export SOC=<IMX8QXP/IMX8QM>

#### # Build the application and kernel module
make

For i.mx6 and i.mx7, it is needed to build only the user space application.

### 2.1 Setup environment:
#### # Initialize the environment by sourcing an environment setup script from yocto toolchain generated for desired platform
. /opt/fsl-imx-x11/4.1.33-7ulp_beta/environment-setup-cortexa7hf-neon-poky-linux-gnueabi

#### # Export SOC variable for the desired platform
export SOC=<IMX6SX/IMX7D/IMX7S>

#### # Build the application
make

### 3. Run instructions

#### # Instructions for i.mx8 platform
Note:
1. This application assumes that the M4 apps are compiled for TCM in order to be in sync with MCU SDK release (SDK_2.2_VAL-MIMX8QM_M4).
If other memories are needed for M4 app, please update these macros from include/m4ctrl.h. Please note that some memories will likely need to
be reserved from Linux dtb in order to be excluded from the memory used by linux.
2. hello_world app for M4 was used for testing.

#### 3.1 Insert the kernel module

insmod m4ctrl_dev.ko

#### 3.2 Run the app:
Examples - i.mx8QM

root@imx8qmlpddr4arm2:~# ./m4ctrl --deploy=./m4_image_c0.bin --core=0

root@imx8qmlpddr4arm2:~# ./m4ctrl --stop --core=0

root@imx8qmlpddr4arm2:~# ./m4ctrl --start --core=0

root@imx8qmlpddr4arm2:~# ./m4ctrl --deploy=./m4_image_c1.bin --core=1

root@imx8qmlpddr4arm2:~# ./m4ctrl --stop --core=1

root@imx8qmlpddr4arm2:~# ./m4ctrl --start --core=1

#### # Instructions for i.mx6 and i.mx7 platforms
Note:
1. This application assumes that the M4 apps are compiled for TCM and the cortex M4 is started from uboot.
2. hello_world app for M4 was used for testing.

#### 3.1 Run the app:
Examples: - i.mx6sx

##### U-boot shell
=> run m4boot

##### Linux shell
root@imx6sxsabresd:~# ./m4ctrl --deploy=hello_world.bin

### 4. Known limitations:
1. Cortex M4 should be started from u-boot on imx6 and imx7 platforms.
2. Due to known hardware TLB issue, sometimes the application will block on IMX8QM.
