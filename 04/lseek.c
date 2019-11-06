// usage for lseek() syscall.
#include <unistd.h>
// prototype: off_t lseek(int fd, off_t offset, int whence);
//#include <sys/stat.h>
#include <fcntl.h>
#include <header.h>

int main(int argc, char *argv[]) {

  // whence: 
  //  1. SEEK_SET: begin.
  //  2. SEEK_CUR: current.
  //  3. SEEK_END: end.

  char *file = argv[0];
  if (argc > 1)
    file = argv[1];

  int flag = O_RDONLY;
  int fd;
  if ((fd = open(file, flag)) == -1)
    errExit("open %s", file);

  // get the current pointer pos.
  off_t curr;
  curr = lseek(fd, 0, SEEK_CUR);
  printf("current pos: %ld\n", curr);

  char buff[256] = {0};
  ssize_t rlen;
  rlen = read(fd, buff, sizeof(buff));
  if (rlen == -1)
    errExit("read %s", file);

  curr = lseek(fd, 0, SEEK_CUR);
  printf("read after, current pos: %ld\n", curr);

  // set to begin.
  off_t pos;
  pos = lseek(fd, 0, SEEK_SET);
  printf("set pos to begin: %ld\n", pos);

  // set to end, will return the file length.
  pos = lseek(fd, 0, SEEK_END);
  printf("set pos to end: %ld\n", pos);


  if (close(fd) == -1)
    errExit("close");
  
  return 0;
}
