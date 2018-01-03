#include <stdlib.h>

#include <aem/stringbuf.h>
#include <aem/stringslice.h>
#include <aem/log.h>

#include "stream.h"
#include "display.h"

#include "crystalfontz_pkt.h"

#include "crystalfontz.h"

int liblcd_cfz_init(struct liblcd_display *disp, int param1, int param2, int param3, void *argv);
int liblcd_cfz_close(struct liblcd_display *disp);

int liblcd_cfz_customchar(struct liblcd_display *disp, int i, const char def[8]);
int liblcd_cfz_mvstr(struct liblcd_display *disp, int y, int x, struct aem_stringslice s);

int liblcd_cfz_backlight(struct liblcd_display *disp, int level);
int liblcd_cfz_contrast(struct liblcd_display *disp, int level);
int liblcd_cfz_cursor(struct liblcd_display *disp, enum liblcd_cursor_state state);

static const struct liblcd_driver liblcd_driver_crystalfontz_s =
{
	.name       = "crystalfontz",

	.init       = liblcd_cfz_init,
	.close      = liblcd_cfz_close,

	.customchar = liblcd_cfz_customchar,
	.mvstr      = liblcd_cfz_mvstr,

	.backlight  = liblcd_cfz_backlight,
	.contrast   = liblcd_cfz_contrast,
	.cursor     = liblcd_cfz_cursor,
};

const struct liblcd_driver *const liblcd_driver_crystalfontz = &liblcd_driver_crystalfontz_s;

struct cfz_data
{
	// TODO: LEDs, buttons, etc.
	void (*on_key)(char key, int state);
};


int liblcd_cfz_init(struct liblcd_display *disp, int param1, int param2, int param3, void *argv)
{
	struct cfz_data *data = malloc(sizeof(*data));
	disp->driver_data = data;

	disp->n_rows = param1;
	disp->n_cols = param2;
	disp->n_cchars = 8;

	aem_logf_ctx(AEM_LOG_BUG, "NYI: construct (struct cfz_data*)disp->driver_data\n");

	return 1;
}

int liblcd_cfz_close(struct liblcd_display *disp)
{
	struct cfz_data *data = disp->driver_data;

	aem_logf_ctx(AEM_LOG_BUG, "NYI: destruct (struct cfz_data*)disp->driver_data\n");

	free(disp->driver_data);
	disp->driver_data = NULL;

	return 0;
}


int liblcd_cfz_customchar(struct liblcd_display *disp, int i, const char def[8])
{
	struct cfz_data *data = disp->driver_data;

	struct liblcd_cfz_pkt pkt = {.stream = &disp->stream};

	liblcd_cfz_pkt_tx_begin(&pkt, 0x09, 9);
	liblcd_cfz_pkt_tx_putc(&pkt, i);
	for (size_t i = 0; i < 8; i++)
	{
		liblcd_cfz_pkt_tx_putc(&pkt, def[i]);
	}
	return liblcd_cfz_pkt_tx_end(&pkt);
}

int liblcd_cfz_mvstr(struct liblcd_display *disp, int y, int x, struct aem_stringslice stream)
{
	struct cfz_data *data = disp->driver_data;

	struct liblcd_cfz_pkt pkt = {.stream = &disp->stream};

	liblcd_cfz_pkt_tx_begin(&pkt, 0x1F, 2 + aem_stringslice_len(&stream));
	liblcd_cfz_pkt_tx_putc(&pkt, x);
	liblcd_cfz_pkt_tx_putc(&pkt, y);
	for (const char *p = stream.start; p != stream.end; p++)
	{
		liblcd_cfz_pkt_tx_putc(&pkt, *p);
	}
	return liblcd_cfz_pkt_tx_end(&pkt);
}

int liblcd_cfz_backlight(struct liblcd_display *disp, int level)
{
	struct cfz_data *data = disp->driver_data;

	struct liblcd_cfz_pkt pkt = {.stream = &disp->stream};

	liblcd_cfz_pkt_tx_begin(&pkt, 0x0E, 1);
	liblcd_cfz_pkt_tx_putc(&pkt, level);
	return liblcd_cfz_pkt_tx_end(&pkt);
}

int liblcd_cfz_contrast(struct liblcd_display *disp, int level)
{
	struct cfz_data *data = disp->driver_data;

	struct liblcd_cfz_pkt pkt = {.stream = &disp->stream};

	liblcd_cfz_pkt_tx_begin(&pkt, 0x0D, 1);
	liblcd_cfz_pkt_tx_putc(&pkt, level);
	return liblcd_cfz_pkt_tx_end(&pkt);
}

int liblcd_cfz_cursor(struct liblcd_display *disp, enum liblcd_cursor_state state)
{
	struct cfz_data *data = disp->driver_data;

	struct liblcd_cfz_pkt pkt = {.stream = &disp->stream};

	liblcd_cfz_pkt_tx_begin(&pkt, 0x0C, 1);
	liblcd_cfz_pkt_tx_putc(&pkt, state);
	return liblcd_cfz_pkt_tx_end(&pkt);
}

void liblcd_cfz_rx(struct liblcd_display *disp)
{
	aem_assert(disp != NULL);

	struct cfz_data *data = disp->driver_data;

	struct liblcd_cfz_pkt pkt = {.stream = &disp->stream};
again:;
	if (liblcd_cfz_pkt_rx_begin(&pkt) < 0) goto again;

	switch (pkt.type)
	{
		case 0x80:
		{
			int act = liblcd_cfz_pkt_rx_getc(&pkt);
			if (act < 0) goto fail;
			unsigned char key = '\0';
			int state = -1;
			switch (act)
			{
				case  1: key = 'u'; state = 1; break;
				case  2: key = 'd'; state = 1; break;
				case  3: key = 'l'; state = 1; break;
				case  4: key = 'r'; state = 1; break;
				case  5: key = 'e'; state = 1; break;
				case  6: key = 'x'; state = 1; break;
				case  7: key = 'u'; state = 0; break;
				case  8: key = 'd'; state = 0; break;
				case  9: key = 'l'; state = 0; break;
				case 10: key = 'r'; state = 0; break;
				case 11: key = 'e'; state = 0; break;
				case 12: key = 'x'; state = 0; break;
			}
			if (data->on_key != NULL)
			{
				data->on_key(act, state);
			}
			aem_logf_ctx(AEM_LOG_INFO, "key %c state %d\n", key, state);
		} break;

		case 0x40:
		{
			aem_logf_ctx(AEM_LOG_INFO, "ack ping\n");

			// TODO: show data
		} break;

		case 0x41:
		{
			aem_logf_ctx(AEM_LOG_INFO, "ack version\n");

			// TODO: deal with version number
		} break;

		case 0x42:
		{
			aem_logf_ctx(AEM_LOG_INFO, "ack write user flash\n");
		} break;

		case 0x43:
		{
			aem_logf_ctx(AEM_LOG_INFO, "ack read user flash\n");

			// TODO: show 16 bytes of data
		} break;

		case 0x44:
		{
			aem_logf_ctx(AEM_LOG_INFO, "ack store boot state\n");
		} break;

		case 0x45:
		{
			aem_logf_ctx(AEM_LOG_INFO, "ack reboot\n");
		} break;

		case 0x46:
		{
			aem_logf_ctx(AEM_LOG_INFO, "ack clear\n");
		} break;

		case 0x49:
		{
			aem_logf_ctx(AEM_LOG_INFO, "ack special char\n");
		} break;

		case 0x4a:
		{
			aem_logf_ctx(AEM_LOG_INFO, "read lcd\n");

			// TODO: deal with data
		} break;

		case 0x4b:
		{
			aem_logf_ctx(AEM_LOG_INFO, "ack cursor position\n");
		} break;

		case 0x4c:
		{
			aem_logf_ctx(AEM_LOG_INFO, "ack cursor style\n");
		} break;

		case 0x4d:
		{
			aem_logf_ctx(AEM_LOG_INFO, "ack contrast\n");
		} break;

		case 0x4e:
		{
			aem_logf_ctx(AEM_LOG_INFO, "ack backlight\n");
		} break;

		case 0x57:
		{
			aem_logf_ctx(AEM_LOG_INFO, "ack key mode\n");
		} break;

		case 0x58:
		{
			int curr = liblcd_cfz_pkt_rx_getc(&pkt);
			int pressed = liblcd_cfz_pkt_rx_getc(&pkt);
			int released = liblcd_cfz_pkt_rx_getc(&pkt);
			aem_logf_ctx(AEM_LOG_INFO, "read keys curr %02x pressed %02x released %02x\n", curr, pressed, released);
		} break;

		case 0x5e:
		{
			AEM_STRINGBUF_ON_STACK(str, 128);
			for (size_t i = 0; i < 15; i++)
			{
				int c = liblcd_cfz_pkt_rx_getc(&pkt);

				aem_stringbuf_putc(&str, ' ');
				if (c > 0)
				{
					aem_stringbuf_puthex(&str, c);
				}
				else
				{
					aem_stringbuf_puts(&str, "??");
				}
			}
			aem_logf_ctx(AEM_LOG_INFO, "read status%s\n", aem_stringbuf_get(&str));
			aem_stringbuf_dtor(&str);
		} break;

		case 0x5f:
		{
			aem_logf_ctx(AEM_LOG_INFO, "ack write\n");
		} break;

		case 0x61: // ?
		{
			aem_logf_ctx(AEM_LOG_INFO, "ack set baud rate\n");
		} break;

		case 0x62:
		{
			aem_logf_ctx(AEM_LOG_INFO, "ack write gpo\n");
		} break;

		default:
			aem_logf_ctx(AEM_LOG_INFO, "unknown type\n");
			while (pkt.len)
			{
				int c = liblcd_cfz_pkt_rx_getc(&pkt);
				if (c < 0) goto fail;
				aem_logf_ctx(AEM_LOG_INFO, "pkt getc: %02x\n", c);
			}
			break;
	}

fail:
	liblcd_cfz_pkt_rx_end(&pkt);
}
