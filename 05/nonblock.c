// nonblock for file descriptor.

// socket/pipe can't use open to create fd, so you should set O_NONBLOCK by fcntl.
//

#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>

#include <header.h>

void pipe_nonblock() {
  int fds[2];

  if (pipe(fds) == -1)
    errExit("pipe");
  
  int flags;
  if ((flags = fcntl(fds[0], F_GETFL)) == -1)
    errExit("pipe fcntl");

  flags |= O_NONBLOCK;
  if (fcntl(fds[0], F_SETFL, flags) == -1)
    errExit("pipe fcntl");

  printf("pipe set read-fd O_NONBLOCK success\n");
  close(fds[0]);
  close(fds[1]);
}

void sock_nonblock() {
  int fd;
  int type = SOCK_STREAM;
  if ((fd = socket(AF_INET, type, 0)) == -1)
    errExit("socket");

  int flags;
  if ((flags = fcntl(fd, F_GETFL)) == -1)
    errExit("socket fcntl");

  flags |= O_NONBLOCK;
  if (fcntl(fd, F_SETFL, flags) == -1)
    errExit("socket fcntl");

  printf("socket O_NONBLOCK success\n");
  close(fd);
}

void sock_nonblock_ex() {
  int fd;
  int type = SOCK_STREAM | SOCK_NONBLOCK; // linux 2.6.27 允许直接在type中添加 SOCK_NONBLOCK选项。
  if ((fd = socket(AF_INET, type, 0)) == -1)
    errExit("socket");

  int flags;
  if ((flags = fcntl(fd, F_GETFL)) == -1)
    errExit("socket fcntl");

  if (flags & O_NONBLOCK)
    printf("socket O_NONBLOCK get...\n");

  close(fd);
}

int main() {
  pipe_nonblock();
  sock_nonblock();
  sock_nonblock_ex();
  return 0;
}
