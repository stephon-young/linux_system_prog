// 用O_APPEND和可写标志打开一个已经存在的文件，并将文件的偏移量置于起始位置，在写入数据，数据会显示在哪里呢？
//  答: 新写入的数据将会在文件的尾部。
//  试验的结果，确实如同回答：
//  可以这样理解:
//    当写入文件的时候文件按照 O_APPEND 打开，则
//      无论怎么使用 lseek，对于写入，都是在文件的尾部并且这个写入是带有 O_EXCL 性质的，就是独占性质的，其他的写，需要等待。
//      读取则不受该规则影响。

#include <unistd.h>
#include <fcntl.h>

#include <header.h>

int main(int argc, char *argv[]) {
  if (argc != 2)
    usageErr("usage: %s <file>", argv[0]);

  char *file = argv[1];

  int flags = O_RDWR | O_APPEND;
  int fd;
  if ((fd = open(file, flags)) == -1)
    errExit("open %s", file);

  if (lseek(fd, 0, SEEK_SET) == -1)
    errExit("lseek to 0");

  char buff[4096];
  ssize_t rlen;
  if ((rlen = read(fd, buff, sizeof(buff) - 1)) == -1)
    errExit("read %s", file);

  buff[rlen] = '\0';
  printf("read 0: %s\n", buff);

  char data[] = "hello, world!\n";
  ssize_t wlen;
  if ((wlen = write(fd, data, sizeof(data))) == -1)
    errExit("write %s", file);

  printf("write %s successful\n", file);
  close(fd);

  return 0;
}