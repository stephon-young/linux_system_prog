
include ../Makefile.inc

all: ${PROG_LIB}

${PROG_LIB}: error.o parser.o
	ar rs $@ $^

error.o: error.c ename.c.inc
	$(CC) $(CFLAGS) -c error.c

parser.o: parser.c
	$(CC) $(CFLAGS) -c parser.c

ename.c.inc:
	sh Build_ename.sh > $@
	echo 1>&2 "ename.c.inc built"

.PHONY: clean
clean:
	${RM} *.o ename.c.inc ${PROG_LIB}