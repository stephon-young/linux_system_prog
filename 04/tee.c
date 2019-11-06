// for command tee.

#include <unistd.h>
#include <fcntl.h>
#include <signal.h>

#include <header.h>

int fd = -1;

void interrupt(int signo) {
  //printf("interrupted\n");
  if (fd != -1) {
    if (close(fd) == -1)
      errExit("interrupt close");
  }
  exit(0);
}

void exited(void) {
  printf("exited...\n");
}

int main(int argc, char *argv[]) {
  //atexit(exited);

  char *optstring = "a";
  char ch;
  int append = 0;
  while ((ch = getopt(argc, argv, optstring)) != -1) {
    switch (ch) {
    case 'a':
      append = 1;
      break;
    case '?':
      usageErr("unknown option...");
    }
  }
  char *file = argv[optind];
  //printf("append=%d,file=%s\n", append, file);

  signal(SIGINT, interrupt);
  signal(SIGTSTP, interrupt);

  int flag = O_CREAT | O_WRONLY;
  if (append)
    flag |= O_APPEND;
  else
    flag |= O_TRUNC;
  mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;

  if ((fd = open(file, flag, mode)) == -1)
    errExit("open %s", file);

  int in = STDIN_FILENO;
  int out = STDOUT_FILENO;

  size_t buflen = (1 << 20) * 16; // 16M
  //printf("buf len: %lu\n", buflen);
  char *buf;
  if ((buf = malloc(buflen)) == NULL)
    errExit("malloc buffer");
  
  ssize_t rlen, wlen;
  for (;;) {
    //printf("read...\n");
    rlen = read(in, buf, buflen - 1);
    if (rlen == -1)
      errExit("read stdin");
    if (rlen == 0) {// EOF.
      //printf("Ctrl-D press...\n");
      break;
    }
    
    //printf("write...\n");
    wlen = write(out, buf, rlen);
    if (wlen == -1)
      errExit("write stdout");

    wlen = write(fd, buf, rlen);
    if (wlen == -1)
      errExit("write %s", file);
  }

  if (close(fd) == -1)
    errExit("close %s", file);

  //printf("normall exit...\n");
  return 0;
}