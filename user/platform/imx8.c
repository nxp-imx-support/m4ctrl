/*
 * Copyright 2017 NXP
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#include "../../include/m4ctrl.h"

#define DEVICE_PATH	"/dev/m4ctrl"

static int fd;

/* This variable is initialised in m4ctrl.c */
extern int core_id;

m4_data m4[M4_CORES_NUM] = {
	{
		.areas = {
			{ TCML_ADDR_M0, 0, TCML_RESERVED_SIZE_M0 },
		},
		.start_cmd = M4CTRL_START_CORE_M0,
		.stop_cmd = M4CTRL_STOP_CORE_M0,
		.pwron_cmd = M4CTRL_PWRON_CORE_M0,
		.pwroff_cmd = M4CTRL_PWROFF_CORE_M0
        },
#if defined(IMX8QM)
        {
	    .areas = {
		{ TCML_ADDR_M1, 0, TCML_RESERVED_SIZE_M1 },
	    },
		.start_cmd = M4CTRL_START_CORE_M1,
		.stop_cmd = M4CTRL_STOP_CORE_M1,
		.pwron_cmd = M4CTRL_PWRON_CORE_M1,
		.pwroff_cmd = M4CTRL_PWROFF_CORE_M1
        },
#endif /* defined(IMX8QM) */
};

void platform_setup()
{

	fd = open(DEVICE_PATH, O_RDONLY);
	if (fd < 0) {
		perror("Error during opening the m4ctrl kernel device \n");
		exit(EXIT_FAILURE);
	}

	alignment_check(&m4[core_id]);
	map_memory(&m4[core_id]);

}

void cleanup()
{
	/* Close the file descriptor for m4ctrl device driver */
	if (fd > 0)
		close(fd);

	unmap_memory(&m4[core_id]);
}

void  m4_start()
{
    int cmd = m4[core_id].start_cmd;
    if (ioctl(fd, cmd, 0) < 0) {
        perror("ioctl");
        exit(EXIT_FAILURE);
        printf("Start cortex M4, core %d\n", core_id);
    }
}
void m4_stop()
{
    int cmd = m4[core_id].stop_cmd;
    if (ioctl(fd, cmd, 0) < 0) {
        perror("ioctl");
        exit(EXIT_FAILURE);
    }

    printf("Stop cortex M4, core %d\n", core_id);
}
void m4_reset()
{
    m4_stop();
    m4_start();
}

static void  m4_pwroff()
{
    int cmd = m4[core_id].pwroff_cmd;
    if (ioctl(fd, cmd, 0) < 0) {
        perror("ioctl");
        exit(EXIT_FAILURE);
        printf("Poweroff cortex M4, core %d\n", core_id);
    }
}
static void m4_pwron()
{
    int cmd = m4[core_id].pwron_cmd;
    if (ioctl(fd, cmd, 0) < 0) {
        perror("ioctl");
        exit(EXIT_FAILURE);
    }

    printf("Poweron cortex M4, core %d\n", core_id);
}

static void m4_image_transfer(uint32_t *dst, const char * binary_name)
{
	int fw_fd;
	uint32_t block;

	if ((fw_fd = open(binary_name, O_RDONLY)) < 0) {
		perror("failed to open firmware image\n");
		exit(EXIT_FAILURE);
	}

	sync();

	while (read(fw_fd, &block, sizeof(block)) == sizeof(block))
	{
		*dst = block;
		dst = dst + 1;
	}

	sync();

	close(fw_fd);
}

void  m4_deploy(char *filename)
{
	m4_stop();
	m4_pwroff();
	m4_pwron();
	m4_image_transfer(m4[core_id].areas[TCML_IDX].vaddr, filename);
	m4_start();

}

