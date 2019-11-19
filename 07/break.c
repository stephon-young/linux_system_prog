// brk()/sbrk()函数的使用和测试。
//  1. 测试使用情况。
//  2. 测试边界情况。

#include <unistd.h>
#include <stdio.h>

int main(int argc, char *argv[]) {

  //获取当前的program break的位置
  char *now = sbrk(0);
  printf("sbrk(0)  current break address: %10p\n", now);

  int one_mb_bytes = 1 << 20;
  //在当前位置基础上加1MB，
  char *addr = now + one_mb_bytes;
  int ret;
  //使用brk直接设置program break位置。
  if ((ret = brk(addr)) == -1) {
    printf("brk() add 1mb bytes address, but failed\n");
    return -1;
  }

  char *p = now + one_mb_bytes;
  printf("brk() add 1mb,    want address: %10p\n", p);

  //验证是否提升成功。
  now = sbrk(0);
  printf("brk() add 1mb,   after address: %10p\n", now);

  int ten_kb_bytes = 10240;
  intptr_t increment = ten_kb_bytes;
  char *inc_addr;
  //在原地址上提升10kb的program break位置。
  if ((inc_addr = sbrk(increment)) == (char *)-1) {
    printf("sbrk() add 10kb address, but failed\n");
    return -1;
  }

  p = now + ten_kb_bytes;
  printf("sbrk() add 10kb,  want address: %10p\n", p);
  //验证是否成功
  now = sbrk(0);
  printf("sbrk() add 10kb, after address: %10p\n", now);

  intptr_t decrease = -10240;
  char *dec_addr;
  //在原地址上降低10kb的program break位置。
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