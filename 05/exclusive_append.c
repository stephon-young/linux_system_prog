// exclusive append test.

#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>

#include <header.h>


int main(int argc, char *argv[]) {
  char *file = "append.txt";

  int flag = O_CREAT | O_TRUNC | O_APPEND | O_WRONLY;
  mode_t mode = S_IRUSR | S_IWUSR;

  int fd;
  if ((fd = open(file, flag, mode)) == -1)
    errExit("open %s", file);

  pid_t id;
  if ((id = fork()) == -1)
    errExit("fork");

  if (id != 0) { // parent process
    lseek(fd, 0, SEEK_END);
    printf("[parent] lseek over...\n");
    sleep(1);
    printf("[parent] write begin...\n");

    char buf[] = "i am parent process.\n";
    size_t len = sizeof(buf);
    ssize_t wlen;
    if ((wlen = write(fd, buf, len)) == -1)
      errExit("write");

    printf("[parent] write over...\n");
    wait(NULL);
  } else { // child process
    lseek(fd, 0, SEEK_END);
    printf("[child] lseek over...\n");
    sleep(1);
    printf("[child] write begin...\n");

    char buf[] = "i am child process.\n";
    size_t len = sizeof(buf);
    ssize_t wlen;
    if ((wlen = write(fd, buf, len)) == -1)
      errExit("write");
    printf("[child] write over...\n");
  }

  close(fd);
  return 0;
}