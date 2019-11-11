#include <stdio.h>
#include <stdlib.h>

// 显式的声明文本段、初始化数据段、未初始化数据段的结尾下一字节地址。
extern char etext, edata, end;

char globalBuf[65536];
int primes[] = {2, 3, 5, 7};

static int square(int x) {
  int result;
  result = x * x;
  return result;
}

static void doCalc(int val) {
  printf("The square of %d is %d\n", val, square(val));

  if (val < 1000) {
    int t;
    t = val * val *val;
    printf("The cube of %d is %d\n", val, t);
  }
}

int main(int argc, char *argv[]) {
  static int key = 9973;
  //虽然有未初始化的空间，但是文件大小，不会有这么大，也就是未初始化段，并不会在程序文件中占有空间
  // 而是只记录位置和大小，在运行时由程序加载器分配这一空间。
  // 注意，初始化为0和初始为非零，文件大小不一样，初始化为0，和不初始化一样，编译后文件大小很小
  // 但是初始化为非零，则文件大小，至少是10240000字节。

  //static char mbuf[10240000] = {0};   // -rwxrwxrwx 1 aux aux 11584 Nov 11 12:25 mem_segment*
  //static char mbuf[10240000] = {1};   // -rwxrwxrwx 1 aux aux 10251608 Nov 11 12:24 mem_segment*
  static char mbuf[10240000];           // -rwxrwxrwx 1 aux aux 11584 Nov 11 12:25 mem_segment*
  char *p;

  printf(".text segment end address: %10p\n", &etext);
  printf(".data segment end address: %10p\n", &edata);
  printf(".bss  segment end address: %10p\n", &end);

  p = malloc(1024);

  doCalc(key);

  return 0;
}