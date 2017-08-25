#ifndef __M4CTRL_H__
#define __M4CTRL_H__		1

#include <asm/ioctl.h>

#if !defined(__KERNEL__)
#include <fcntl.h>
#include <getopt.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#endif /* !defined(__KERNEL__) */

#include "platform.h"

#if !defined(__KERNEL__)
/* M4 Control API */
void m4_start();
void m4_stop();
void m4_deploy( char * filename );
#endif

#endif
