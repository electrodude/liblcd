#include <stdlib.h>

#include <aem/log.h>

#include "driver.h"

#include "display.h"

int liblcd_display_init(struct liblcd_display *disp, const struct liblcd_driver *driver, int param1, int param2, int param3, void *argv)
{
	aem_assert(disp != NULL);
	aem_assert(driver != NULL);

	if (!liblcd_stream_ok(&disp->stream))
	{
		aem_logf_ctx(AEM_LOG_ERROR, "Not initializing display: no stream open.  Call liblcd_stream_open_*(&disp.stream, ...) first.\n");
		return 1;
	}

	aem_logf_ctx(AEM_LOG_NOTICE, "open display with driver %s\n", driver->name);

	disp->driver = driver;

	return disp->driver->init(disp, param1, param2, param3, argv);
}

int liblcd_display_close(struct liblcd_display *disp)
{
	if (disp == NULL) return 0;

	if (disp->driver != NULL)
	{
		aem_logf_ctx(AEM_LOG_NOTICE, "close display with driver %s\n", disp->driver->name);
		disp->driver->close(disp);
	}
	else
	{
		aem_logf_ctx(AEM_LOG_WARN, "close display with no driver open\n");
	}

	liblcd_stream_close(&disp->stream);

	return 0;
}

int liblcd_display_customchar(struct liblcd_display *disp, int i, const char def[8])
{
	if (disp == NULL || disp->driver == NULL) return 1;

	aem_logf_ctx(AEM_LOG_DEBUG, "define %d\n", i);

	return disp->driver->customchar(disp, i, def);
}

int liblcd_display_mvstr(struct liblcd_display *disp, int y, int x, struct aem_stringslice s)
{
	if (disp == NULL || disp->driver == NULL) return 1;

	aem_logf_ctx(AEM_LOG_DEBUG, "%d, %d, %zd chars\n", y, x, aem_stringslice_len(&s));

	return disp->driver->mvstr(disp, y, x, s);
}

int liblcd_display_backlight(struct liblcd_display *disp, int level)
{
	if (disp == NULL || disp->driver == NULL) return 1;

	aem_logf_ctx(AEM_LOG_DEBUG, "%d\n", level);

	return disp->driver->backlight(disp, level);
}

int liblcd_display_contrast(struct liblcd_display *disp, int level)
{
	if (disp == NULL || disp->driver == NULL) return 1;

	aem_logf_ctx(AEM_LOG_DEBUG, "%d\n", level);

	return disp->driver->contrast(disp, level);
}

int liblcd_display_cursor(struct liblcd_display *disp, enum liblcd_cursor_state state)
{
	if (disp == NULL || disp->driver == NULL) return 1;

	aem_logf_ctx(AEM_LOG_DEBUG, "%d\n", state);

	return disp->driver->cursor(disp, state);
}
