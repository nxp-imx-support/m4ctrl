#ifndef __PLATFORM_H__
#define __PLATFORM_H__		1

#if defined(IMX8QM) || defined(IMX8QXP)

#include "platform/imx8.h"

#elif defined(IMX6SX) || defined(IMX7D) || defined(IMX7S)

#include "platform/imx_v6_v7.h"

#else

#error "Platform undefined!"

#endif

#if !defined(__KERNEL__)
void platform_setup();
void cleanup();
void map_memory(m4_data * m4);
void unmap_memory(m4_data * m4);
void alignment_check(m4_data * m4);
#endif

#endif
