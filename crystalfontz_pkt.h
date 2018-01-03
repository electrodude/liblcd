#ifndef LIBLCD_CRYSTALFONTZ_PKT_H
#define LIBLCD_CRYSTALFONTZ_PKT_H

#include <sys/types.h>

struct liblcd_stream stream;

// 4 byte header + 255 byte payload + unnecessary NULL terminator + 2 byte crc = 262
#define LIBLCD_CF_PKT_BUF_MAX 262

struct liblcd_cfz_pkt
{
	struct liblcd_stream *stream;

	size_t n;

	unsigned char type;
	unsigned char len;
	unsigned short crc;

	char buf[LIBLCD_CF_PKT_BUF_MAX];
};

int liblcd_cfz_pkt_tx_begin(struct liblcd_cfz_pkt *pkt, unsigned char type, unsigned char len);
int liblcd_cfz_pkt_tx_putc(struct liblcd_cfz_pkt *pkt, unsigned char c);
int liblcd_cfz_pkt_tx_end(struct liblcd_cfz_pkt *pkt);

int liblcd_cfz_pkt_rx_begin(struct liblcd_cfz_pkt *pkt);
int liblcd_cfz_pkt_rx_getc(struct liblcd_cfz_pkt *pkt);
int liblcd_cfz_pkt_rx_end(struct liblcd_cfz_pkt *pkt);

#endif /* LIBLCD_CRYSTALFONTZ_PKT_H */
