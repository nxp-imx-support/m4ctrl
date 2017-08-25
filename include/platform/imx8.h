#ifndef __IMX8_H__
#define __IMX8_H__		1

#define TCML_ADDR_M0 0x34FE0000
#define TCML_RESERVED_SIZE_M0 0x00040000

#if defined(IMX8QM)
#define TCML_ADDR_M1 0x38FE0000
#define TCML_RESERVED_SIZE_M1 0x00040000
#endif /* defined(IMX8QM) */

#if !defined(__KERNEL__)
typedef struct {
	/* start address of the memory reserved for M4 cortex */
	uint64_t addr;
	/* size of the memory reserved for M4 cortex */
	uint32_t size;
	/* ioctl commands */
	int start_cmd;
	int stop_cmd;
	int pwron_cmd;
	int pwroff_cmd;
} m4_data;
#endif /* !defined(__KERNEL__) */

#if defined(IMX8QM)

#define M4_CORES_NUM    (2)

#else

#define M4_CORES_NUM    (1)

#endif /* defined(IMX8QM) */

#define M4CTRL_START_CORE_M0	_IOC(_IOC_NONE,  'k', 1, 0)
#define M4CTRL_STOP_CORE_M0	_IOC(_IOC_NONE,  'k', 2, 0)
#define M4CTRL_PWRON_CORE_M0	_IOC(_IOC_NONE,  'k', 3, 0)
#define M4CTRL_PWROFF_CORE_M0	_IOC(_IOC_NONE,  'k', 4, 0)

#if defined(IMX8QM)

#define M4CTRL_PWRON_CORE_M1	_IOC(_IOC_NONE,  'k', 5, 0)
#define M4CTRL_START_CORE_M1	_IOC(_IOC_NONE,  'k', 6, 0)
#define M4CTRL_STOP_CORE_M1	_IOC(_IOC_NONE,  'k', 7, 0)
#define M4CTRL_PWROFF_CORE_M1	_IOC(_IOC_NONE,  'k', 8, 0)

#endif /* defined(IMX8QM) */

#endif
