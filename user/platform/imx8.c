#include "../../include/m4ctrl.h"

#define DEVICE_PATH	"/dev/m4ctrl"

static int fd;
extern int fd_mem;
static char * tcml = NULL;

/* This variable is initialised in m4ctrl.c */
extern int core_id;

m4_data m4[M4_CORES_NUM] = {
        {
         TCML_ADDR_M0, TCML_RESERVED_SIZE_M0,
         M4CTRL_START_CORE_M0, M4CTRL_STOP_CORE_M0,
         M4CTRL_PWRON_CORE_M0, M4CTRL_PWROFF_CORE_M0
        },
#if defined(IMX8QM)
        {
          TCML_ADDR_M1, TCML_RESERVED_SIZE_M1,
          M4CTRL_START_CORE_M1, M4CTRL_STOP_CORE_M1,
          M4CTRL_PWRON_CORE_M1, M4CTRL_PWROFF_CORE_M1
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

	tcml = (void *) mmap(NULL,
				 m4[core_id].size,
				 PROT_READ | PROT_WRITE,
				 MAP_SHARED, fd_mem,
				 m4[core_id].addr
				);
	if ((void *) tcml == MAP_FAILED)
	{
	    perror("failed to mmap tcml");
	    exit(EXIT_FAILURE);
	}

}

void cleanup()
{
	/* Close the file descriptor for m4ctrl device driver */
	if (fd > 0)
		close(fd);

	/* Unmap the memory */
	if (munmap(tcml, m4[core_id].size) == -1) {
		perror("failed to munmap tcml");
		exit(EXIT_FAILURE);
	}

	if (close(fd_mem) == -1) {
		perror("failed to close /dev/mem");
		exit(EXIT_FAILURE);
	}

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

static void m4_image_transfer(char *dst, const char * binary_name)
{
    int fw_fd;
    char block;

    if ((fw_fd = open(binary_name, O_RDONLY)) < 0) {
        perror("failed to open firmware image\n");
        exit(EXIT_FAILURE);
    }

    sync();

    while (read(fw_fd, &block, sizeof(block)) ==sizeof(block))
    {
        *dst++ = block;
        /* workaround for caching bug from imx8qm*/
        *(dst-1);
    }

    sync();

    close(fw_fd);
}

void  m4_deploy(char *filename)
{
        m4_stop();
        m4_pwroff();
        m4_pwron();
        m4_image_transfer(tcml, filename);
        m4_start();
}

