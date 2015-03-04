PROG = rer
SRCS = fdlist.c rer.c replace_str.c main.c
CC = clang
CFLAGS += -std=gnu99 -O2 -DHOST_OS=${host_os}
MAN = rer.1
MANTARGET = man

DEBUG ?= 0
.if ${DEBUG} > 0
CFLAGS += -g -DDEBUG=1
.endif

.if ${:!pkg-config --exists libpcre && echo 1!}
CFLAGS += ${:!pkg-config --cflags libpcre!}
LDADD += ${:!pkg-config --libs libpcre!}
.else
.error "libpcre not found"
.endif

test: ${PROG}
	${MAKE} -C tests
	(cd tests; ./rer_tests)
	(cd tests; . ./test.sh)

.include "Makefile.inc"
.include <host-target.mk>
.include <bsd.prog.mk>
