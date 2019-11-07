include ../Makefile.inc

all: bad_open exclusive_open exclusive_append copy_fd vector_rw nonblock

bad_open: bad_open.c
	$(CC) $^ ${PROG_LIB} $(CFLAGS) $(INC) -o $@
exclusive_open: exclusive_open.c
	$(CC) $^ ${PROG_LIB} $(CFLAGS) $(INC) -o $@
exclusive_append: exclusive_append.c
	$(CC) $^ ${PROG_LIB} $(CFLAGS) $(INC) -o $@
copy_fd: copy_fd.c
	$(CC) $^ ${PROG_LIB} $(CFLAGS) $(INC) -o $@
vector_rw: vector_rw.c
	$(CC) $^ ${PROG_LIB} $(CFLAGS) $(INC) -o $@
nonblock: nonblock.c
	$(CC) $^ ${PROG_LIB} $(CFLAGS) $(INC) -o $@

.PHONY:
	clean
clean:
	${RM} *.o bad_open exclusive_open exclusive_append copy_fd vector_rw nonblock