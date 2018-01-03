CC=gcc
LD=gcc
AR=ar rcu
RANLIB=ranlib

CFLAGS+=-std=c99 -fPIC -Wall -Wextra -Wno-unused-parameter
LDFLAGS+=

CFLAGS+=-I.

CFLAGS+=-O3
LDFLAGS+=-O3

SOURCES_LIBLCD=parallax.c crystalfontz.c crystalfontz_pkt.c display.c driver.c stream.c
OBJECTS_LIBLCD=$(patsubst %.c,%.o,${SOURCES_LIBLCD})

SOURCES_LIBLCD_TEST=test.c
OBJECTS_LIBLCD_TEST=$(patsubst %.c,%.o,${SOURCES_LIBLCD_TEST})

LIBAEM_DIR=aem

LIBS=${LIBAEM_DIR}/libaem.a

DEPDIR=.deps
DEPFLAGS=-MD -MP -MF ${DEPDIR}/$*.d

$(shell mkdir -p ${DEPDIR})

all:	liblcd_test

clean:
	rm -vf ${OBJECTS_LIBLCD} ${OBJECTS_LIBLCD_TEST} ${DEPDIR}/*.d liblcd_test liblcd.a
	${MAKE} -C ${LIBAEM_DIR} -w clean

liblcd_test:	${OBJECTS_LIBLCD_TEST} liblcd.a ${LIBS}
	${LD} $^ ${LDFLAGS} -o $@

liblcd.a:	${OBJECTS_LIBLCD}
	${AR} $@ $^
	${RANLIB} $@

%.o:	%.c
	${CC} ${CFLAGS} ${DEPFLAGS} -c $< -o $@

${LIBAEM_DIR}/libaem.a:
	${MAKE} -C ${LIBAEM_DIR} -w libaem.a

.PHONY:	${LIBAEM_DIR}/libaem.a

.PHONY:	all clean

include $(wildcard ${DEPDIR}/*.d)
#
# vim: set ts=13 :
