// copy file descriptor.
//  1. int dup(int oldfd);
//  2. int dup2(int oldfd, int newfd);
//  3. int dup3(int oldfd, int newfd, int flags); // linux-special.
//  4. int fcntl(int fd, F_DUPFD/F_DUPFD_CLOEXEC);

// dup3 must define _GNU_SOURCE.

#define _GNU_SOURCE

#include <unistd.h>
#include <fcntl.h>

#include <header.h>

// implement shell `xxx 2>&1`
void copy_fd1() {
  if (close(STDERR_FILENO) == -1)
    errExit("copy fd 1: close");

  int fd;
  if ((fd = dup(STDOUT_FILENO)) == -1)
    errExit("copy fd 1: dup");

  printf("1. copy fd over, fd=%d\n", fd);
}

void copy_fd2() {
  int fd;
  if ((fd = dup2(STDOUT_FILENO, STDERR_FILENO)) == -1)
    errExit("copy fd 2: dup2");

  printf("2. copy fd over, fd=%d\n", fd);
}

void copy_fd3() {
  int fd;
  int flags = 0;
  if ((fd = dup3(STDOUT_FILENO, STDERR_FILENO, flags)) == -1)
    errExit("copy fd 3: dup3");

  printf("3. copy fd over, fd=%d\n", fd);
}

void copy_fd3_ex() {
  int fd;
  int flags = O_CLOEXEC;
  if ((fd = dup3(STDOUT_FILENO, STDERR_FILENO, flags)) == -1)
    errExit("copy fd 3 ex: dup3");

  printf("3.x copy fd over, fd=%d\n", fd);
}

void copy_fd4() {
  if (close(STDERR_FILENO) == -1)
    errExit("copy fd 4: close");

  int fd;
  if ((fd = fcntl(STDOUT_FILENO, F_DUPFD)) == -1)
    errExit("copy fd 4: fcntl");

  printf("4. copy fd over, fd=%d\n", fd);
}

void copy_fd4_ex() {
  if (close(STDERR_FILENO) == -1)
    errExit("copy fd 4 ex: close");

  int fd;
  if ((fd = fcntl(STDOUT_FILENO, F_DUPFD_CLOEXEC)) == -1)
    errExit("copy fd 4 ex: fcntl");

  printf("4.x copy fd over, fd=%d\n", fd);
}

int main(int argc, char *argv[]) {
  int no = 1;
  if (argc > 1)
    no = getInt(argv[1], PSR_ANY_BASE | PSR_GT_0, NULL);
  
  if (no > 4)
    usageErr("usage: %s [select no]. \n  select no can be 1,2,3,4 default is 1\n", argv[0]);

  switch (no) {
  case 1: copy_fd1(); break;
  case 2: copy_fd2(); break;
  case 3: copy_fd3(); break;
  case 4: copy_fd4(); break;
  }

  return 0;
}