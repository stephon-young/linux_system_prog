// copy with file hole...

#include <unistd.h>
#include <fcntl.h>

#include <header.h>

int main(int argc, char *argv[]) {
  if (argc != 2)
    usageErr("usage: %s old-file new-file", argv[0]);

  char *from = argv[1];
  char *to = argv[2];
  
  int from_fd;
  if ((from_fd = open(from, O_RDONLY)) == -1)
    errExit("open %s", from);

  int to_fd;
  int flag = O_CREAT | O_TRUNC | O_WRONLY;
  mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;
  if ((to_fd = open(to, flag, mode)) == -1)
    errExit("open %s", to);

  ssize_t rlen;
  ssize_t wlen;
  char data[1];
  off_t i = 0;
  while ((rlen = read(from_fd, data, sizeof(data))) > 0) {
    if (data[0] != '\0') {
      lseek(to_fd, i, SEEK_SET);
      if ((wlen = write(to_fd, data, sizeof(data))) == -1)
        errExit("write %s", to);
    }
    i++;
  }
  if (rlen == -1)
    errExit("read %s", from);

  if (close(from_fd) == -1)
    errExit("close %s", from);
  if (close(to_fd) == -1)
    errExit("close %s", to);

  return 0;
}