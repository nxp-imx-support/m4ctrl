#ifndef __PLATFORM_H__
#define __PLATFORM_H__		1

#if defined(IMX8QM) || defined(IMX8QXP)

#include "platform/imx8.h"

#else

#error "Platform undefined!"

#endif

#endif
