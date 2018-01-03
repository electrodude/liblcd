#ifndef LIBLCD_DRIVER_H
#define LIBLCD_DRIVER_H

#include <aem/stringslice.h>

#include "display.h"

struct liblcd_driver
{
	const char *name;

	int (*init)(struct liblcd_display *disp, int param1, int param2, int param3, void *argv);
	int (*close)(struct liblcd_display *disp);

	int (*customchar)(struct liblcd_display *disy, int i, const char def[8]);
	int (*mvstr)(struct liblcd_display *disp, int y, int x, struct aem_stringslice s);

	int (*backlight)(struct liblcd_display *disp, int level);
	int (*contrast)(struct liblcd_display *disp, int level);
	int (*cursor)(struct liblcd_display *disp, enum liblcd_cursor_state state);
};

const struct liblcd_driver *liblcd_driver_get(const char *name);


#endif /* LIBLCD_DRIVER_H */
