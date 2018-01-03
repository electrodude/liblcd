#define _XOPEN_SOURCE
#include <stdlib.h>
#include <aem/log.h>

#include "driver.h"

#include "display.h"

void usage(void)
{
	fprintf(stderr, "Usage: liblcd_test -d <driver> [-b <baud>] <device>\n");
}

int main(int argc, char **argv)
{
	aem_log_fp = stderr;

	aem_log_level_curr = AEM_LOG_DEBUG;

	const char *device = NULL;
	int baud = -1;
	const char *driver_name = NULL;

	int opt;
	while ((opt = getopt(argc, argv, "hd:b:")) != -1)
	{
		switch(opt)
		{
			case 'd':
				driver_name = optarg;
				break;

			case 'b':
				baud = atoi(optarg);
				break;

			case 'h':
			default:
				usage();
				return 1;
		}
	}

	argc -= optind;
	argv += optind;

	if (argc)
	{
		device = *argv;
		argv++; argc--;
	}
	else
	{
		usage();
		return 1;
	}

	if (driver_name == NULL)
	{
		usage();
		return 1;
	}

	const struct liblcd_driver *driver = liblcd_driver_get(driver_name);

	if (driver == NULL)
	{
		aem_logf_ctx(AEM_LOG_FATAL, "Couldn't find driver \"%s\"!\n", driver_name);
		return 1;
	}

	struct liblcd_display disp = {0};

	liblcd_stream_open_tty(&disp.stream, device, baud);
	if (liblcd_display_init(&disp, driver, 4, 20, 0, NULL))
	{
		//return 1;
	}

	if (!strcmp(driver_name, "crystalfontz"))
	{
		liblcd_display_backlight(&disp, 100);
		liblcd_display_contrast(&disp, 110);
		liblcd_display_cursor(&disp, LIBLCD_CURSOR_OFF);

		liblcd_display_mvstr(&disp, 0, 0, aem_stringslice_new_cstr("Test 1 2 3 A B C"));

		void liblcd_cfz_rx(struct liblcd_display *disp);

		while (1) liblcd_cfz_rx(&disp);
	}
	else
	{
		liblcd_display_backlight(&disp, 1);
		liblcd_display_cursor(&disp, LIBLCD_CURSOR_UNDERLINE_BLOCK_BLINK);
		liblcd_display_mvstr(&disp, 0, 0, aem_stringslice_new_cstr("Chicken Soup"));
	}

	liblcd_display_close(&disp);

	return 0;
}
