//---------------------------------------------------------
//	Catlive
//
//		©2017 Yuichiro Nakada
//---------------------------------------------------------

#include "jo_mpeg.h"
#include "v4l2.h"

static char* outFilename = NULL;

void mainLoop()
{
	while (1) {
		v4l2_frameRead();

		static int count = 0;
		printf("%d\n", count++);
		FILE *fp = fopen(outFilename, "ab");
		jo_write_mpeg(fp, v4l2.rgb, v4l2.width, v4l2.height, 10);  // frame 0
		fclose(fp);

		nanosleep((const struct timespec[]){{ 0, 100000000L }}, NULL);	// sleeps for half a sec
	}
}

void usage(FILE* fp, int argc, char** argv)
{
	fprintf(fp,
		"Usage: %s [options]\n\n"
		"Options:\n"
		"-d | --device name   Video device name [/dev/video0]\n"
		"-h | --help          Print this message\n"
		"-o | --output        Output filename\n"
		"-m | --mmap          Use memory mapped buffers\n"
		"-r | --read          Use read() calls\n"
		"-u | --userptr       Use application allocated buffers\n"
		"-W | --width         width\n"
		"-H | --height        height\n"
		"",
		argv[0]);
}

static const char short_options[] = "d:ho:mruW:H:";

static const struct option
	long_options[] = {
	{ "device",     required_argument,      NULL,           'd' },
	{ "help",       no_argument,            NULL,           'h' },
	{ "output",     required_argument,      NULL,           'o' },
	{ "mmap",       no_argument,            NULL,           'm' },
	{ "read",       no_argument,            NULL,           'r' },
	{ "userptr",    no_argument,            NULL,           'u' },
	{ "width",      required_argument,      NULL,           'W' },
	{ "height",     required_argument,      NULL,           'H' },
	{ 0, 0, 0, 0 }
};

int main(int argc, char *argv[])
{
	for (;;) {
		int index, c = 0;

		c = getopt_long(argc, argv, short_options, long_options, &index);

		if (-1 == c) {
			break;
		}

		switch (c) {
		case 0: /* getopt_long() flag */
			break;

		case 'd':
			v4l2.deviceName = optarg;
			break;

		case 'h':
			// print help
			usage(stdout, argc, argv);
			exit(EXIT_SUCCESS);

		case 'o':
			// set jpeg filename
			outFilename = optarg;
			break;

		case 'm':
#ifdef IO_MMAP
			v4l2.io = IO_METHOD_MMAP;
#else
			fprintf(stderr, "You didn't compile for mmap support.\n");
			exit(EXIT_FAILURE);
#endif
			break;

		case 'r':
#ifdef IO_READ
			v4l2.io = IO_METHOD_READ;
#else
			fprintf(stderr, "You didn't compile for read support.\n");
			exit(EXIT_FAILURE);
#endif
			break;

		case 'u':
#ifdef IO_USERPTR
			v4l2.io = IO_METHOD_USERPTR;
#else
			fprintf(stderr, "You didn't compile for userptr support.\n");
			exit(EXIT_FAILURE);
#endif
			break;

		case 'W':
			// set width
			v4l2.width = atoi(optarg);
			break;

		case 'H':
			// set height
			v4l2.height = atoi(optarg);
			break;

		default:
			usage(stderr, argc, argv);
			exit(EXIT_FAILURE);
		}
	}

	// check for need parameters
	if (!outFilename) {
		fprintf(stderr, "You have to specify JPEG output filename!\n\n");
		usage(stdout, argc, argv);
		exit(EXIT_FAILURE);
	}

	v4l2_deviceOpen();
	v4l2_captureStart();

	mainLoop();

	v4l2_captureStop();
	v4l2_deviceClose();

	return 0;
}

