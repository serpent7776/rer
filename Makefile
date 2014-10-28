PROG = rer
SRCS = fdlist.c rer.c replace_str.c main.c
CC = clang
CFLAGS += -std=gnu99 -O2
NO_MAN = 

DEBUG ?= 0
.if ${DEBUG} > 0
CFLAGS += -DDEBUG=1
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

.include <bsd.prog.mk>
