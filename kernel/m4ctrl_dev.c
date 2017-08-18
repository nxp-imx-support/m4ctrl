/*
 * Copyright 2017 NXP
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/sched.h>
#include <linux/wait.h>
#include <soc/imx8/sc/sci.h>

#include "../include/m4ctrl.h"

#define LOG_LEVEL	KERN_DEBUG

#define MY_MAJOR		42
#define MY_MINOR		0
#define NUM_MINORS		1
#define MODULE_NAME		"m4ctrl"

struct m4ctrl_device_data {
	struct cdev cdev;
	atomic_t access;
};

sc_ipc_t ipc_handle;
sc_rm_pt_t pt_m4_1;
uint32_t mu_id;
struct m4ctrl_device_data devs[NUM_MINORS];

static int m4ctrl_open(struct inode *inode, struct file *file)
{
	struct m4ctrl_device_data *data =
		container_of(inode->i_cdev, struct m4ctrl_device_data, cdev);

	file->private_data = data;

	if (atomic_cmpxchg(&data->access, 1, 0) != 1)
		return -EBUSY;

	return 0;
}

static int
m4ctrl_release(struct inode *inode, struct file *file)
{
	struct m4ctrl_device_data *data =
		(struct m4ctrl_device_data *) file->private_data;

	atomic_inc(&data->access);

	return 0;
}

static long
m4ctrl_ioctl(struct file *file, unsigned int cmd, unsigned long arg)
{
	int ret = 0;

	switch (cmd) {
	case M4CTRL_PWRON_CORE_M0:
		printk(LOG_LEVEL "%s\n", "pwron core 0\n");
	        if (sc_pm_set_resource_power_mode(ipc_handle, SC_R_M4_0_PID0, SC_PM_PW_MODE_ON) != SC_ERR_NONE)
			return -EIO;

		if (sc_pm_set_resource_power_mode(ipc_handle, SC_R_M4_0_MU_1A, SC_PM_PW_MODE_ON) != SC_ERR_NONE)
	            return -EIO;
		break;
	case M4CTRL_PWRON_CORE_M1:
		printk(LOG_LEVEL "%s\n", "pwron core 1\n");
		if (sc_pm_set_resource_power_mode(ipc_handle, SC_R_M4_1_PID0, SC_PM_PW_MODE_ON) != SC_ERR_NONE)
			return -EIO;

		if (sc_pm_set_resource_power_mode(ipc_handle, SC_R_M4_1_MU_1A, SC_PM_PW_MODE_ON) != SC_ERR_NONE)
	            return -EIO;
		break;
	case M4CTRL_PWROFF_CORE_M0:

		printk(LOG_LEVEL "pwroff the core M0\n");
		if (sc_pm_set_resource_power_mode(ipc_handle, SC_R_M4_0_PID0, SC_PM_PW_MODE_OFF) != SC_ERR_NONE)
                return -EIO;

		if (sc_pm_set_resource_power_mode(ipc_handle, SC_R_M4_0_MU_1A, SC_PM_PW_MODE_OFF) != SC_ERR_NONE)
                return -EIO;
		break;
	case M4CTRL_PWROFF_CORE_M1:
		printk(LOG_LEVEL "%s\n", "pwroff core 1\n");
		if (sc_pm_set_resource_power_mode(ipc_handle, SC_R_M4_1_PID0, SC_PM_PW_MODE_OFF) != SC_ERR_NONE)
			return -EIO;

		if (sc_pm_set_resource_power_mode(ipc_handle, SC_R_M4_1_MU_1A, SC_PM_PW_MODE_OFF) != SC_ERR_NONE)
			return -EIO;
	case M4CTRL_START_CORE_M0:
		printk(LOG_LEVEL "%s\n", "start core 0\n");
		printk(LOG_LEVEL "starting the core\n");
		sc_pm_cpu_start(ipc_handle, SC_R_M4_0_PID0, true,  TCML_ADDR_M0);
		printk(LOG_LEVEL "%s\n", "finish the operation\n");
		break;
	case M4CTRL_START_CORE_M1:
		printk(LOG_LEVEL "%s\n", "start core 1\n");
		sc_pm_cpu_start(ipc_handle, SC_R_M4_1_PID0, true,  TCML_ADDR_M1);
		printk(LOG_LEVEL "%s\n", "finish the operation\n");
		printk(LOG_LEVEL "starting the core\n");
		break;
	case M4CTRL_STOP_CORE_M0:

		printk(LOG_LEVEL "stooping the core M0\n");
		sc_pm_cpu_start(ipc_handle, SC_R_M4_0_PID0, false, TCML_ADDR_M0);
		break;
	case M4CTRL_STOP_CORE_M1:
		printk(LOG_LEVEL "%s\n", "stop core 1\n");
		sc_pm_cpu_start(ipc_handle, SC_R_M4_1_PID0, false, TCML_ADDR_M1);
		break;
	default:
		ret = -EINVAL;
	}

	return ret;
}

static const struct file_operations m4ctrl_fops = {
	.owner = THIS_MODULE,
	.open = m4ctrl_open,
	.release = m4ctrl_release,
	.unlocked_ioctl = m4ctrl_ioctl,
};

static int m4ctrl_init(void)
{
	int err;
	int i;
	sc_err_t sciErr;

	err = register_chrdev_region(MKDEV(MY_MAJOR, MY_MINOR),
			NUM_MINORS, MODULE_NAME);
	if (err != 0) {
		printk(KERN_ERR "register_chrdev_region");
		return err;
	}

	for (i = 0; i < NUM_MINORS; i++) {
		atomic_set(&devs[i].access, 1);
		cdev_init(&devs[i].cdev, &m4ctrl_fops);
		cdev_add(&devs[i].cdev, MKDEV(MY_MAJOR, i), 1);
	}

	sciErr = sc_ipc_getMuID(&mu_id);
        if (sciErr != SC_ERR_NONE) {
                pr_info("Cannot obtain MU ID\n");
                return -EPROBE_DEFER;
        }

        sciErr = sc_ipc_open(&ipc_handle, mu_id);

        if (sciErr != SC_ERR_NONE) {
                pr_info("Cannot open MU channel to SCU\n");
                return -EPROBE_DEFER;
        }

	return 0;
}

static void m4ctrl_exit(void)
{
	int i;

	for (i = 0; i < NUM_MINORS; i++)
		cdev_del(&devs[i].cdev);

	unregister_chrdev_region(MKDEV(MY_MAJOR, MY_MINOR), NUM_MINORS);
}

module_init(m4ctrl_init);
module_exit(m4ctrl_exit);

MODULE_DESCRIPTION("Driver to control M4 cortex via SCFW");
MODULE_AUTHOR("Stoica Cosmin-Stefan cosmin.stoica@nxp.com");
MODULE_LICENSE("GPL");


