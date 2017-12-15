/*
 * Copyright 2017 NXP
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 */

#ifndef __IMX_V6_V7_H__
#define __IMX_V6_V7_H__		1

/* Number of M4 cores */
#define M4_CORES_NUM    (1)

/* Number of mapped memory area:
 * - SRC
 * - OCRAM
 * - TCML
 */
#define MEM_AREAS	(3)

#define SRC_IDX		(0)
#define OCRAM_IDX	(1)
#define TCML_IDX	(2)

#if defined(IMX6SX)

#define SRC_ADDR    0x20D8000U
#define OCRAM_ADDR  0x8f8000U
/* offset from the start address of the memory reserved for M4 cortex */
#define M4RCR_OFFS  0x00U
/* position of the platform reset bit on SRC_SCR register*/
#define M4PR_BIT    (12)
/* position of the core reset bit on SRC_SCR register*/
#define M4CR_BIT    (3)

#elif  defined(IMX7D) || defined(IMX7S)

#define SRC_ADDR    0x30390000U
#define OCRAM_ADDR  0x180000U
/* offset from the start address of the memory reserved for M4 cortex */
#define M4RCR_OFFS  0x0cU
/* position of the platform reset bit on SRC_SCR register */
#define M4PR_BIT    (2)
/* position of the core reset bit on SRC_SCR register */
#define M4CR_BIT    (1)

#else

#error platform unknown

#endif

#define TCML_ADDR   0x7f8000U

#define SRC_MAP_SIZE    (M4RCR_OFFS + 4U)
#define TCML_MAP_SIZE   (32U * 1024U)
#define OCRAM_MAP_SIZE  8U

#if !defined(__KERNEL__)
typedef struct {
    /* physical start address of the mapped memory */
    uint32_t paddr;
    /* virtual start address of the mapped memory */
    uint32_t * vaddr;
    /* size of the mapped memory */
    uint32_t size;
} mem;

typedef struct {
    mem areas[MEM_AREAS];
} m4_data;

#endif /* !defined(__KERNEL__) */


#endif
