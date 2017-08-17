#ifndef __M4CTRL_H__
#define __M4CTRL_H__		1

#include <asm/ioctl.h>
#define M4CTRL_START_CORE_M0	_IOC(_IOC_NONE,  'k', 1, 0)
#define M4CTRL_START_CORE_M1	_IOC(_IOC_NONE,  'k', 2, 0)
#define M4CTRL_STOP_CORE_M0	_IOC(_IOC_NONE,  'k', 3, 0)
#define M4CTRL_STOP_CORE_M1	_IOC(_IOC_NONE,  'k', 4, 0)
#define M4CTRL_PWRON_CORE_M0	_IOC(_IOC_NONE,  'k', 5, 0)
#define M4CTRL_PWRON_CORE_M1	_IOC(_IOC_NONE,  'k', 6, 0)
#define M4CTRL_PWROFF_CORE_M0	_IOC(_IOC_NONE,  'k', 7, 0)
#define M4CTRL_PWROFF_CORE_M1	_IOC(_IOC_NONE,  'k', 8, 0)

#endif
