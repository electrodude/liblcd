#include <aem/serial.h>

#include "stream.h"

int liblcd_stream_open_tty(struct liblcd_stream *s, const char *device, int baud)
{
	return aem_serial_open(&s->ser, device, baud);
}

int liblcd_stream_close(struct liblcd_stream *s)
{
	return aem_serial_close(&s->ser);
}

int liblcd_stream_ok(struct liblcd_stream *s)
{
	return aem_serial_ok(&s->ser);
}

size_t liblcd_stream_write(struct liblcd_stream *s, struct aem_stringslice out)
{
	return aem_serial_write(&s->ser, out);
}

size_t liblcd_stream_read(struct liblcd_stream *s, struct aem_stringbuf *in)
{
	return aem_serial_read(&s->ser, in);
}

int liblcd_stream_getc(struct liblcd_stream *s)
{
	int c = aem_serial_getc(&s->ser);
	return c;
}
