// dev for /dev/fd/xxx

// /dev/fd/0 === stdin
// /dev/fd/1 === stdout
// /dev/fd/2 === stderr.
// open("/dev/fd/0", O_RDONLY);
//  和 dup(0)作用相同。
// 通常这种用法，在程序中很少用，在shell中经常使用，有些命令，如:
//    ls | diff - oldlslist 
//  其中 - 就代表 标准输入，但是这种写法支持需要diff命令进行特殊处理，
//  很多命令并不会这样处理，可以用下面的命令取而代之，而不用特殊处理。
//    ls | diff /dev/fd/0 oldlslist
//
// 系统还针对三者提供了符号链接: /dev/stdin; /dev/stdout; /dev/stderr;
//

#include <fcntl.h>

#include <header.h>

int main() {

  int in;
  if ((in = open("/dev/fd/0", O_RDONLY)) == -1)
    errExit("open /dev/fd/0");

  int out;
  if ((out = open("/dev/fd/1", O_WRONLY)) == -1)
    errExit("open /dev/fd/1");

  int err;
  if ((err = open("/dev/fd/2", O_WRONLY)) == -1)
    errExit("open /dev/fd/2");

  char buf[4096] = {0};
  ssize_t rlen;
  if ((rlen = read(in, buf, sizeof(buf) - 1)) == -1)
    errExit("read");

  buf[rlen] = '\0';
  ssize_t wlen;
  if ((wlen = write(out, buf, rlen)) == -1)
    errExit("write");

  printf("in=%d,out=%d,err=%d\n", in, out, err);

  close(in);
  close(out);
  close(err);

  return 0;
}