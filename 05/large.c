// large file [32bits art only.]
//  注意: 这个仅仅在32bits架构的系统下面才有用，64bits架构的系统并不存在下面的问题。
//       因为off_t为64位，而32位系统下，off_t为32位，除去符号位，则只有31位，即最大值为2G.
//  大文件支持的方式: 
//  1. 过渡型LFS API
//      定义测试宏: _LARGEFILE64_SOURCE
//  2. 设置_FILE_OFFSET_BITS宏的值为64 - 推荐使用。
//
//  printf()打印off64_t的时候为 lld，如果原本就是64位系统，则为ld。

// 过渡型LFS API
//  1. 定义宏: _LARGEFILE64_SOURCE
//  2. api由带后缀的64替换原来的api: open -> open64, lseek -> lseek64 ..., 
//     注意只是api带有off_t的api需要改变，其中open接口可以不做改变，而传入O_LARGEFILE 标志即可。
//     lseek -> lseek64, truncate -> truncate64, stat -> stat64, mmap -> mmap64, setrlimit -> setrlimit64.
//    
//  3. 相关的数据类型也变为64位的: off_t -> off64_t ...

#define _LARGEFILE64_SOURCE

#include <unistd.h>
#include <fcntl.h>

#include <header.h>

// open_64 和 open_with_large_file函数 的效果一样。
int open_64(const char *file) {
  int flags = O_RDWR | O_CREAT | O_TRUNC;
  mode_t mode = S_IRUSR | S_IWUSR;
  return open64(file, flags, mode);
}
int open_with_large_flag(const char *file) {
  int flags = O_RDWR | O_CREAT | O_TRUNC | O_LARGEFILE;
  mode_t mode = S_IRUSR | S_IWUSR;
  return open(file, flags, mode);
}

int main(int argc, char *argv[]) {
  if (argc != 3)
    usageErr("usage: %s <file> <length>", argv[0]);

  char *file = argv[1];
  off64_t off = atoll(argv[2]);

  int fd;
  //if ((fd = open_64(file)) == -1)
  if ((fd = open_with_large_flag(file)) == -1)
    errExit("open64 %s", file);


  if (lseek64(fd, off, SEEK_SET) == -1)
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