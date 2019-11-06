#include <sys/stat.h>
#include <fcntl.h>

#include <header.h>

int main(int argc, char *argv[]) {
  int fd;
  const char *file = "./file";
  int flag = O_RDWR | O_CREAT | O_TRUNC;
  mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;
  if ((fd = open(file, flag, mode)) == -1) {
    errExit("open %s", file);
  }

  char data[] = "hello, world!";
  size_t len = sizeof(data);
  if (write(fd, data, len) != len) {
    errExit("write %s", file);
  }

  if (close(fd) == -1) {
    errExit("close %s", file);
  }

  printf("write some word for read: ");
  fflush(stdout);

  int in = STDIN_FILENO;
  int out = STDOUT_FILENO;
  int err = STDERR_FILENO;
  char rbuff[256] = {0};
  ssize_t rlen;
  rlen = read(in, rbuff, sizeof(rbuff) - 1);
  if (rlen == -1) {
    errExit("read stdin");
  }
  rbuff[rlen] = '\0';
  printf("read stdin: %s", rbuff);

  ssize_t wlen;
  char wbuff[] = "write stdout: hello, generic I/O...\n";
  wlen = write(out, wbuff, sizeof(wbuff));
  if (wlen == -1) {
    errExit("write stdout");
  }

  return 0;
}