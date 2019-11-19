include ../Makefile.inc

all: break 

break: break.c
	$(CC) $^ $(CFLAGS) $(INC) -o $@
#modify_env: modify_env.c
#	$(CC) $^ ${PROG_LIB} $(CFLAGS) $(INC) -o $@

.PHONY:
	clean
clean:
	${RM} *.o break