PROG = rer_tests
SRCS = main.c fdlist_test.c replace_str_test.c rer_test.c ../fdlist.c ../replace_str.c ../rer.c
NO_MAN = 

CFLAGS += -std=gnu99
DEBUG ?= 0
.if ${DEBUG} > 0
CFLAGS += -DDEBUG=1
.endif

.if ${:!pkg-config --exists cunit && echo 1!}
CFLAGS += ${:!pkg-config --cflags cunit!}
LDADD += ${:!pkg-config --libs cunit!}
.else
.error "cunit not found"
.endif

.if ${:!pkg-config --exists libpcre && echo 1!}
CFLAGS += ${:!pkg-config --cflags libpcre!}
LDADD += ${:!pkg-config --libs libpcre!}
.else
.error "libpcre not found"
.endif

test: ${PROG}
	./${PROG}

.include <bsd.prog.mk>
