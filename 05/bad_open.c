// bad execusive open.

#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#include <header.h>

int main(int argc, char *argv[]) {

  if (argc <= 1)
    usageErr("usage: %s <file> [sleep]");

  char *file = argv[1];
  int fd;
  if ((fd = open(file, O_WRONLY)) == -1) {
    if (errno != ENOENT)
      errExit("open %s", file);
    
    printf("[PID %ld] File \"%s\" doesn't exist yet\n", (long)getpid(), file);
    if (argc > 2) {
      sleep(5);
      printf("[PID %ld] Done sleeping\n", (long)getpid());
    }
    int flag = O_WRONLY | O_CREAT;
    mode_t mode = S_IRUSR | S_IWUSR;
    if ((fd = open(file, flag, mode)) == -1)
      errExit("open %s", file);

    close(fd);
    printf("[PID %ld] Created file \"%s\" exclusively\n", (long)getpid(), file);
    return 0;
  }

  printf("[PID %ld] File \"%s\" already exists\n", (long)getpid(), file);
  close(fd);

  return 0;
}