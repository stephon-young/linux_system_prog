include ../Makefile.inc

all: segment argv0 env modify_env longjmp example2 example3

segment: segment.c
	$(CC) $^ $(CFLAGS) $(INC) -o $@
argv0: argv0.c
	$(CC) $^ $(CFLAGS) $(INC) -o $@
env: env.c
	$(CC) $^ $(CFLAGS) $(INC) -o $@
modify_env: modify_env.c
	$(CC) $^ ${PROG_LIB} $(CFLAGS) $(INC) -o $@
longjmp: longjmp.c
	$(CC) $^ $(CFLAGS) $(INC) -o $@
example2: example2.c
	$(CC) $^ $(CFLAGS) $(INC) -o $@
example3: example3.c
	$(CC) $^ $(CFLAGS) $(INC) -o $@

.PHONY:
	clean
clean:
	${RM} *.o segment argv0 env modify_env longjmp example2 example3