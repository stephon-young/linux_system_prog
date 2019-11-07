// exclusive open.

#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

#include <header.h>

int main(int argc, char *argv[]) {
  if (argc <= 1)
    usageErr("usage: %s <file>", argv[0]);

  char *file = argv[1];
  // O_CREAT和O_EXCL组合能保证，创建文件是本进程；如果文件存在，则返回错误，EEXIST。
  //  即，检查文件存在和创建文件是原子的，不可中断。
  // 同样的作用，还有当多个进程都写系统日志文件时，打开文件的时候有O_APPEND, 能够保证
  //  文件偏移量的移动和数据写入是原子的。
  int flag = O_CREAT | O_WRONLY | O_EXCL; 
  mode_t mode = S_IRUSR | S_IWUSR;
  int fd;
  if ((fd = open(file, flag, mode)) == -1) {
    if (errno != EEXIST)
      errExit("open %s", file);

    printf("[PID %ld] File \"%s\" already exist\n", (long)getpid(), file);
    return 0;
  }

  printf("[PID %ld] Create file \"%s\" exclusively\n", (long)getpid(), file);
  close(fd);
  return 0;

}