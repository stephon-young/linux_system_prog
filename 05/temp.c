// 创建临时文件:
//  推荐的两种方法:
//    1. mkstemp() 创建临时文件。
//    2. tmpfile() 创建临时文件。

// mkstemp() 函数用法说明:
//  原型: int mkstemp(char *template); //<stdlib.h>
//  参数: template[in,out], 输入参数模板，要求必须以 XXXXXX 为结尾，创建临时文件成功后，会返回真实文件名
//  返回值: 成功返回文件描述符，失败返回-1.
//  注意: 
//    1. <stdlib.h> 而不是系统调用。
//    2. 创建过程中会使用 O_EXCL 标志，保证是原子的，同时创建成功后，文件可读可写。
//    3. 通常创建成功后，会使用 unlink() 函数删除文件，然后再通过文件描述符进行IO操作，最后通过close最终删除。

// tmpfile() 函数用法说明:
//  原型: FILE* tmpfile(); //<stdio.h>
//  返回值: 返回文件句柄(c语言库层面的句柄)，可以用fread/fwrite调用执行相应的IO操作。
//  注意: 
//    1. 函数在调用创建时，就使用unlink删除了临时文件，所以当fclose关闭文件句柄的时候，文件就被彻底删除了，同上。

#include <unistd.h>
#include <stdlib.h>

#include <header.h>

int main() {

  char template[] = "/tmp/prefix-XXXXXX";
  int fd;
  if ((fd = mkstemp(template)) == -1)
    errExit("mkstemp");

  unlink(template);

  char data[] = "hello, world! newbee...\n";
  ssize_t wlen;
  if ((wlen = write(fd, data, sizeof(data))) == -1)
    errExit("write");


  lseek(fd, 0, SEEK_SET);

  char buff[4096];
  ssize_t rlen;
  if ((rlen = read(fd, buff, sizeof(buff) - 1)) == -1)
    errExit("read");

  // 注意上面的write写入的时候，已经写入了字符串终止符了。
  //buff[rlen] = '\0';

  printf("read from temp file: %s \ncontent: %s", template, buff);

  close(fd);
  return 0;
}