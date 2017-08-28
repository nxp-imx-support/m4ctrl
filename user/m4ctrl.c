/*
 * Copyright 2017 NXP
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * o Redistributions of source code must retain the above copyright notice, this list
 *   of conditions and the following disclaimer.
 *
 * o Redistributions in binary form must reproduce the above copyright notice, this
 *   list of conditions and the following disclaimer in the documentation and/or
 *   other materials provided with the distribution.
 *
 * o Neither the name of NXP Semiconductor, Inc. nor the names of its
 *   contributors may be used to endorse or promote products derived from this
 *   software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */
/*
 * User-space tool for M4 control
 */

#include "../include/m4ctrl.h"

static char * filename = NULL;
/*
 * getopt structures
 */

/* The tool's command line options */
static struct option long_options[] =
{
    {"help", no_argument,       0, 'h'},
#if M4_CORES_NUM > 1
    {"core", required_argument, 0, 'c'},
#endif
    {"start",  no_argument,       0, 's'},
    {"stop",   no_argument, 0, 'x'},
    {"deploy", required_argument, 0, 'd'},
    {0, 0, 0, 0}
};

/*
 * use case
 */
static void usage(const char *argv0)
{
	fprintf(stdout, "M4 Control Tool\n"
			"Usage: %s <options> "
#if M4_CORES_NUM > 1
			"[--core=<n>]"
#endif
	"\n options:\n"
		"\t --help - display the list of supported commands\n"
		"\t --start - start the specified M4 core\n"
		"\t --stop - stop the specified M4 core\n"
		"\t --deploy=<firmware_file> - deploy firmware_file on the specified M4 core\n",
	       argv0);
	exit(EXIT_FAILURE);
}

static int start = 0, stop = 0, deploy = 0;
int core_id, fd_mem;

static m4ctrl_setup()
{
	fd_mem = open("/dev/mem", O_RDWR | O_SYNC);
	if (fd_mem < 0) {
		perror("failed to open /dev/mem \n");
		exit(EXIT_FAILURE);
	}

}

static void parse_cmds(int argc, char ** argv)
{
	int c;
	struct stat sb;

	if (argc < 2) {
		fprintf(stderr, "too few parameters\n");
		usage(argv[0]);
	}

	while(1)
	{
		/* getopt_long stores the option index here. */
		int option_index = 0;

		c = getopt_long(argc, argv, "c:h::sxd:",
				long_options, &option_index);

		/* Detect the end of the options. */
		if (c == -1)
		        break;

		switch (c)
		{
			case 'h':
				usage(argv[0]);
				exit(EXIT_SUCCESS);
#if M4_CORES_NUM > 1
			case 'c':
				core_id = atoi(optarg);
				if (core_id < 0 || core_id > (M4_CORES_NUM - 1)) {
					fprintf(stderr, "Invalid M4 core index.\n");
					if ((M4_CORES_NUM - 1) == 0) {
						fprintf(stderr, "It shall be 0.\n");
					}
					else {
						fprintf(stderr, "It shall be between 0 and %d.\n", M4_CORES_NUM - 1);
					}
					exit(EXIT_FAILURE);
				}
#endif

				break;

			case 's':
				if (stop) {
					fprintf(stderr, "It is not allowed to use simultaneously start and stop commands\n");
					exit(EXIT_FAILURE);
				}

				start = 1;

				break;

			case 'x':
				if (start) {
					fprintf(stderr, "It is not allowed to use simultaneously start and stop commands\n");
					exit(EXIT_FAILURE);
				}

				stop = 1;

				break;

			case 'd':
				if (start || stop) {
					fprintf(stderr, "It is not allowed to use simultaneously start or stop commands together with deploy\n");
					exit(EXIT_FAILURE);
				}

				if (stat(optarg, &sb) < 0) {
					perror("Impossible to access the firmware file\n");
					exit(EXIT_FAILURE);
				}

				deploy = 1;
				filename = strdup(optarg);

				break;

			case '?':
				/* getopt_long already printed an error message. */
				break;

			default:
				abort ();
		}
	};
}

static void execute_cmds()
{
	/* Start the specified M4 core */
	if (start)
	{
		m4_start();
	}

	/* Stop the specified M4 core */
	if (stop)
	{
		m4_stop();
	}

	if (deploy)
	{
		m4_deploy(filename);
	}
}

int main(int argc, char **argv)
{
	parse_cmds(argc, argv);
	m4ctrl_setup();
	platform_setup();
	execute_cmds();
	cleanup();
	return 0;
}
