// usage for file hole(文件空洞)

#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <header.h>

int main(int argc, char *argv[]) {
  const char *file = "./hole";
  int flag = O_RDWR | O_CREAT | O_TRUNC;
  mode_t mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;
  int fd;

  if ((fd = open(file, flag, mode)) == -1)
    errExit("open %s", file);

  // move pointer to over end.
  off_t hole_space = 1000 * 1000;
  off_t curr;
  curr = lseek(fd, hole_space, SEEK_END);

  // write.
  char data[] = "hello, world!";
  ssize_t wlen;
  wlen = write(fd, data, sizeof(data));
  if (wlen == -1)
    errExit("write hole end");

  if (close(fd) == -1)
    errExit("close");

  return 0;
}