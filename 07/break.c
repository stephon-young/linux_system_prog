// brk()/sbrk()函数的使用和测试。
//  1. 测试使用情况。
//  2. 测试边界情况。

#include <unistd.h>
#include <stdio.h>

int main(int argc, char *argv[]) {

  char *now = sbrk(0);
  printf("sbrk(0)  current break address: %10p\n", now);

  int one_mb_bytes = 1 << 20;
  char *addr = now + one_mb_bytes;
  int ret;
  if ((ret = brk(addr)) == -1) {
    printf("brk() add 1mb bytes address, but failed\n");
    return -1;
  }

  char *p = now + one_mb_bytes;
  printf("brk() add 1mb,    want address: %10p\n", p);

  now = sbrk(0);
  printf("brk() add 1mb,   after address: %10p\n", now);

  int ten_kb_bytes = 10240;
  intptr_t increment = ten_kb_bytes;
  char *inc_addr;
  if ((inc_addr = sbrk(increment)) == (char *)-1) {
    printf("sbrk() add 10kb address, but failed\n");
    return -1;
  }

  p = now + ten_kb_bytes;
  printf("sbrk() add 10kb,  want address: %10p\n", p);
  now = sbrk(0);
  printf("sbrk() add 10kb, after address: %10p\n", now);

  intptr_t decrease = -10240;
  char *dec_addr;
  if ((dec_addr = sbrk(decrease)) == (char *)-1) {
    printf("sbrk() dec 10kb, but failed\n"); 
    return -1;
  }

  p = now - ten_kb_bytes;
  printf("sbrk() dec 10kb,  want address: %10p\n", p);
  now = sbrk(0);
  printf("sbrk() dec 10kb, after address: %10p\n", now);

  return 0;
}