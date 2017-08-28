#ifndef __IMX_V6_V7_H__
#define __IMX_V6_V7_H__		1

#if defined(IMX6SX)

#define SRC_ADDR    0x20D8000U
#define OCRAM_ADDR  0x8f8000U
/* offset from the start address of the memory reserved for M4 cortex */
#define M4RCR_OFFS  0x00U
/*position of the platform reset bit on SRC_SCR register*/
#define M4PR_BIT    12
/*position of the core reset bit on SRC_SCR register*/
#define M4CR_BIT    3

#elif  defined(IMX7D) || defined(IMX7S)

#define SRC_ADDR    0x30390000U
#define OCRAM_ADDR  0x180000U
/* offset from the start address of the memory reserved for M4 cortex */
#define M4RCR_OFFS  0x0cU
/*position of the platform reset bit on SRC_SCR register*/
#define M4PR_BIT    2
/*position of the core reset bit on SRC_SCR register*/
#define M4CR_BIT    1

#elif

#error platform unknown

#endif

#define TCML_ADDR   0x7f8000U

#define SRC_MAP_SIZE    (M4RCR_OFFS + 4U)
#define TCML_MAP_SIZE   (32U * 1024U)
#define OCRAM_MAP_SIZE  8U

#if !defined(__KERNEL__)
typedef struct {
	/* start address of the reset control register for M4 cortex */
	uint32_t src_addr;
	/* size of the reset control register for M4 cortex */
	uint32_t src_size;
	/* start address of the TCML memory */
	uint32_t tcml_addr;
	/* start address of the OCRAMS memory */
	uint32_t ocram_addr;
	/* size of TCML memory */
	uint32_t tcml_size;
	/* size of OCRAMS memory */
	uint32_t ocram_size;
} m4_data;
#endif /* !defined(__KERNEL__) */


#endif
