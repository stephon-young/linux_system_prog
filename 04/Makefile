include ../Makefile.inc

all: copy open lseek filehole tee copy_hole

copy: copy.c
	$(CC) $^ ${PROG_LIB} $(CFLAGS) $(INC) -o $@
#$(CC) $^ $(CFLAGS) $(INC) $(LINK) -lprog -static -o $@
open: open.c
	$(CC) $^ ${PROG_LIB} $(CFLAGS) $(INC) -o $@
lseek: lseek.c
	$(CC) $^ ${PROG_LIB} $(CFLAGS) $(INC) -o $@
filehole: filehole.c
	$(CC) $^ ${PROG_LIB} $(CFLAGS) $(INC) -o $@
tee: tee.c
	$(CC) $^ ${PROG_LIB} $(CFLAGS) $(INC) -o $@
copy_hole: copy_hole.c
	$(CC) $^ ${PROG_LIB} $(CFLAGS) $(INC) -o $@

.PHONY:
	clean
clean:
	${RM} *.o copy open lseek filehole tee copy_hole