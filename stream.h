#ifndef LIBLCD_STREAM_H
#define LIBLCD_STREAM_H

#include <aem/serial.h>
#include <aem/stringbuf.h>
#include <aem/stringslice.h>

struct liblcd_stream
{
	struct aem_serial ser;
};

int liblcd_stream_open_tty(struct liblcd_stream *s, const char *device, int baud);
int liblcd_stream_close(struct liblcd_stream *s);

int liblcd_stream_ok(struct liblcd_stream *s);

size_t liblcd_stream_write(struct liblcd_stream *s, struct aem_stringslice out);

size_t liblcd_stream_read(struct liblcd_stream *s, struct aem_stringbuf *in);
int liblcd_stream_getc(struct liblcd_stream *s);

#endif /* LIBLCD_STREAM_H */
