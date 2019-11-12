// large file [32bits art only.]
//  注意: 这个仅仅在32bits架构的系统下面才有用，64bits架构的系统并不存在下面的问题。
//       因为off_t为64位，而32位系统下，off_t为32位，除去符号位，则只有31位，即最大值为2G.
//  大文件支持的方式: 
//  1. 过渡型LFS API
//      定义测试宏: _LARGEFILE64_SOURCE
//  2. 设置_FILE_OFFSET_BITS宏的值为64 - 推荐使用。
//
//  printf()打印off64_t的时候为 lld，如果原本就是64位系统，则为ld。

// 设置_FILE_OFFSET_BITS 宏的值为64
// 其他源码不变，但是要求代码不能将off_t 转为c的类型。

#define _FILE_OFFSET_BITS 64

#include <unistd.h>
#include <fcntl.h>

#include <header.h>

int main(int argc, char *argv[]) {
  if (argc != 3)
    usageErr("usage: %s <file> <length>", argv[0]);

  char *file = argv[1];
  off_t off = atoll(argv[2]);

  //printf("default offset bits: %d\n", _FILE_OFFSET_BITS);

  int fd;
  int flags = O_RDWR | O_CREAT | O_TRUNC;
  mode_t mode = S_IRUSR | S_IWUSR;
  if ((fd = open(file, flags, mode)) == -1)
    errExit("open %s", file);


  if (lseek(fd, off, SEEK_SET) == -1)
    errExit("lseek64 %s", file);

  printf("offset = %ld\n", off);

  char data[] = "test";
  ssize_t wlen;
  if ((wlen = write(fd, data, sizeof(data))) == -1)
    errExit("write %s", file);

  close(fd);
  printf("everything is ok...\n");

  return 0;
}