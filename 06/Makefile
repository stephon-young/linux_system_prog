include ../Makefile.inc

all: segment argv0

segment: segment.c
	$(CC) $^ $(CFLAGS) $(INC) -o $@
argv0: argv0.c
	$(CC) $^ $(CFLAGS) $(INC) -o $@
#large2: large2.c
#	$(CC) $^ ${PROG_LIB} $(CFLAGS) $(INC) -o $@

.PHONY:
	clean
clean:
	${RM} *.o segment argv0