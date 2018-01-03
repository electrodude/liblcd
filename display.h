#ifndef LIBLCD_DISPLAY_H
#define LIBLCD_DISPLAY_H

#include <stddef.h>

#include <aem/stringslice.h>

#include "stream.h"

enum liblcd_cursor_state
{
	LIBLCD_CURSOR_UNKNOWN = -1,
	LIBLCD_CURSOR_OFF = 0,
	LIBLCD_CURSOR_BLOCK_BLINK,
	LIBLCD_CURSOR_UNDERLINE,
	LIBLCD_CURSOR_UNDERLINE_BLOCK_BLINK,
};

struct liblcd_display
{
	struct liblcd_stream stream;

	const struct liblcd_driver *driver;

	void *driver_data;

	size_t n_rows;
	size_t n_cols;
	size_t n_cchars;

	// TODO: curr, next screen state
};

int liblcd_display_init(struct liblcd_display *disp, const struct liblcd_driver *driver, int param1, int param2, int param3, void *argv);
int liblcd_display_close(struct liblcd_display *disp);

int liblcd_display_customchar(struct liblcd_display *disp, int i, const char def[8]);
int liblcd_display_mvstr(struct liblcd_display *disp, int y, int x, struct aem_stringslice s);

int liblcd_display_backlight(struct liblcd_display *disp, int level);
int liblcd_display_contrast(struct liblcd_display *disp, int level);
int liblcd_display_cursor(struct liblcd_display *disp, enum liblcd_cursor_state state);

int liblcd_display_button_callback(struct liblcd_display *disp, void (*on_button)(char button, int state));

#endif /* LIBLCD_DISPLAY_H */
