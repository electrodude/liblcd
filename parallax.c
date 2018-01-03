#include <stdlib.h>

#include <aem/stringbuf.h>
#include <aem/stringslice.h>
#include <aem/log.h>

#include "stream.h"
#include "display.h"

#include "parallax.h"

int liblcd_parallax_init(struct liblcd_display *disp, int param1, int param2, int param3, void *argv);
int liblcd_parallax_close(struct liblcd_display *disp);

int liblcd_parallax_customchar(struct liblcd_display *disp, int i, const char def[8]);
int liblcd_parallax_mvstr(struct liblcd_display *disp, int y, int x, struct aem_stringslice s);

int liblcd_parallax_backlight(struct liblcd_display *disp, int level);
int liblcd_parallax_contrast(struct liblcd_display *disp, int level);
int liblcd_parallax_cursor(struct liblcd_display *disp, enum liblcd_cursor_state state);

const struct liblcd_driver liblcd_driver_parallax_s =
{
	.name       = "parallax",

	.init       = liblcd_parallax_init,
	.close      = liblcd_parallax_close,

	.customchar = liblcd_parallax_customchar,
	.mvstr      = liblcd_parallax_mvstr,

	.backlight  = liblcd_parallax_backlight,
	.contrast   = liblcd_parallax_contrast,
	.cursor     = liblcd_parallax_cursor,
};

const struct liblcd_driver *const liblcd_driver_parallax = &liblcd_driver_parallax_s;

struct plx_data
{
	char sound_length;
	char sound_octave;
};


int liblcd_parallax_init(struct liblcd_display *disp, int param1, int param2, int param3, void *argv)
{
	struct plx_data *data = malloc(sizeof(*data));
	disp->driver_data = data;

	disp->n_rows = param1;
	disp->n_cols = param2;
	disp->n_cchars = 8;

	aem_logf_ctx(AEM_LOG_BUG, "NYI: construct (struct plx_data*)disp->driver_data\n");

	return 1;
}

int liblcd_parallax_close(struct liblcd_display *disp)
{
	struct plx_data *data = disp->driver_data;

	aem_logf_ctx(AEM_LOG_BUG, "NYI: destruct (struct plx_data*)disp->driver_data\n");

	free(disp->driver_data);
	disp->driver_data = NULL;

	return 0;
}


int liblcd_parallax_customchar(struct liblcd_display *disp, int i, const char def[8])
{
	struct plx_data *data = disp->driver_data;

	aem_logf_ctx(AEM_LOG_WARN, "untested\n");

	AEM_STRINGBUF_ON_STACK(pkt, 9);

	aem_stringbuf_putc(&pkt, 0xF0 + i);
	aem_stringbuf_putn(&pkt, 8, def);

	size_t n = liblcd_stream_write(&disp->stream, aem_stringslice_new_str(&pkt));

	aem_stringbuf_dtor(&pkt);

	return pkt.n != n;
}

int liblcd_parallax_mvstr(struct liblcd_display *disp, int y, int x, struct aem_stringslice s)
{
	struct plx_data *data = disp->driver_data;

	aem_logf_ctx(AEM_LOG_WARN, "untested\n");

	AEM_STRINGBUF_ON_STACK(pkt, 1+aem_stringslice_len(&s));

	aem_stringbuf_putc(&pkt, 0x80 + y*20 + x);
	aem_stringbuf_append_stringslice(&pkt, s); // TODO: sanitize, check custom characters, etc.

	size_t n = liblcd_stream_write(&disp->stream, aem_stringslice_new_str(&pkt));

	aem_stringbuf_dtor(&pkt);

	return pkt.n != n;
}


int liblcd_parallax_backlight(struct liblcd_display *disp, int level)
{
	struct plx_data *data = disp->driver_data;

	aem_logf_ctx(AEM_LOG_WARN, "untested\n");

	char c = level ? 0x11 : 0x12;

	size_t n = liblcd_stream_write(&disp->stream, aem_stringslice_new_len(&c, 1));

	return n != 1;
}

int liblcd_parallax_contrast(struct liblcd_display *disp, int level)
{
	aem_logf_ctx(AEM_LOG_INFO, "not available on this hardware\n");

	return 1;
}

int liblcd_parallax_cursor(struct liblcd_display *disp, enum liblcd_cursor_state state)
{
	struct plx_data *data = disp->driver_data;

	aem_logf_ctx(AEM_LOG_WARN, "untested\n");

	char c = '\0';

	switch (state)
	{
		case LIBLCD_CURSOR_OFF                   : c = 0x16; break;
		case LIBLCD_CURSOR_BLOCK_BLINK           : c = 0x17; break;
		case LIBLCD_CURSOR_UNDERLINE             : c = 0x18; break;
		case LIBLCD_CURSOR_UNDERLINE_BLOCK_BLINK : c = 0x19; break;

		default: return 1;
	}

	size_t n = liblcd_stream_write(&disp->stream, aem_stringslice_new_len(&c, 1));

	return n != 1;
}
