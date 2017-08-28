#include "../../include/m4ctrl.h"

static int fd;
extern int fd_mem;
static uint32_t *src;
static uint32_t *m4rcr;
static uint32_t *tcml;
static uint32_t *ocram;

/* This variable is initialised in m4ctrl.c */
extern int core_id;

m4_data m4 = {
	SRC_ADDR, SRC_MAP_SIZE,
	TCML_ADDR, OCRAM_ADDR,
	TCML_MAP_SIZE, OCRAM_MAP_SIZE
};

static void assert_alignment(void)
{
	long page_size;

	if ((page_size = sysconf(_SC_PAGESIZE)) == -1) {
		perror("failed to get page size");
		exit(EXIT_FAILURE);
	}

	if ((SRC_ADDR % page_size) != 0)
	{
		perror("internal error: SRC is not page aligned\n");
		exit(EXIT_FAILURE);
	}

	if ((TCML_ADDR % page_size) != 0)
	{
		perror("internal error: TCML is not page aligned\n");
		exit(EXIT_FAILURE);
	}

	if ((OCRAM_ADDR % page_size) != 0)
	{
		perror("internal error: OCRAM is not page aligned\n");
		exit(EXIT_FAILURE);
	}
}

static void map_memory(void)
{
	if ((fd_mem = open("/dev/mem", O_RDWR | O_SYNC)) == -1)
	{
		perror("failed to open /dev/mem");
		exit(EXIT_FAILURE);
	}

	src = (uint32_t *) mmap(NULL, SRC_MAP_SIZE, PROT_READ | PROT_WRITE,
			MAP_SHARED, fd_mem, SRC_ADDR);
	if ((void *) src == MAP_FAILED)
	{
		perror("failed to mmap src");
		exit(EXIT_FAILURE);
	}

	m4rcr = src + M4RCR_OFFS / sizeof(*m4rcr);

	tcml = (uint32_t *) mmap(NULL, TCML_MAP_SIZE, PROT_READ | PROT_WRITE,
			MAP_SHARED, fd_mem, TCML_ADDR);
	if ((void *) tcml == MAP_FAILED)
	{
		perror("failed to mmap tcml");
		exit(EXIT_FAILURE);
	}

	ocram = (uint32_t *) mmap(NULL, OCRAM_MAP_SIZE, PROT_READ | PROT_WRITE,
			MAP_SHARED, fd_mem, OCRAM_ADDR);
	if ((void *) ocram == MAP_FAILED)
	{
		perror("failed to mmap ocram");
		exit(EXIT_FAILURE);
	}
}

void platform_setup()
{
	assert_alignment();
	map_memory();
}
void cleanup()
{
	if (munmap(src, SRC_MAP_SIZE) == -1) {
		perror("failed to munmap src");
		exit(EXIT_FAILURE);
	}

	if (munmap(tcml, TCML_MAP_SIZE) == -1) {
		perror("failed to munmap tcml");
		exit(EXIT_FAILURE);
	}
	if (munmap(ocram, OCRAM_MAP_SIZE) == -1) {
		perror("failed to munmap ocram");
		exit(EXIT_FAILURE);
	}
	if (close(fd_mem) == -1) {
		perror("failed to close /dev/mem");
		exit(EXIT_FAILURE);
	}
}

void m4_start()
{}
void m4_stop()
{}

static void m4_platform_reset(void)
{
	*m4rcr |= 1 << M4PR_BIT;
}

static void m4_core_reset(void)
{
	*m4rcr |= 1 << M4CR_BIT;
}

static void m4_image_transfer(uint32_t *dst, const char * binary_name)
{
	int fd;
	uint32_t v;
	uint32_t *p;

	if ((fd = open(binary_name, O_RDONLY)) == -1)
	{
		perror("failed to open firmware image");
		exit(EXIT_FAILURE);
	}

	sync();

	p = dst;
	while (read(fd, &v, sizeof(v)) == sizeof(v))
	{
		*p++ = v;
	}

	ocram[0] = tcml[0]; // copy initial stack pointer into ocram
	ocram[1] = tcml[1]; // copy reset vector

	sync();

	close(fd);
}

void  m4_deploy(char *filename)
{
	m4_platform_reset();
	m4_image_transfer(tcml, filename);
	m4_core_reset();
}
