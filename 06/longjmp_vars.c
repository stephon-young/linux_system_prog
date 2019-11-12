// longjmp和编译器优化的问题。
// longjmp将在编译器开启优化时，影响优化的结果。
//  下面的程序在开启优化选项和不开启的时候结果不同:
//    1. 某些二进制接口(ABI)实现的语义要求longjmp()函数恢复先前setjmp()调用所保存的CPU寄存器副本。
//  下面的结果在添加优化编译选项时，输出结果为:
//    inside: 777 888 999
//    after:  777 222 999
//  并未达到预期？ 这是为什么呢？ 
//    难道222变量是保存在了CPU寄存器，而我们的ABI实现正好要求保存setjmp调用的CPU寄存器副本吗？
//  优化后的结果:
//    inside: 777 888 999
//    after:  111 222 999
//  这个倒是正常。
//  
//  未优化编译
//   gcc -o longjmp_vars longjmp_vars.c
//  优化编译
//   gcc -O -o longjmp_vars longjmp_vars.c

#include <setjmp.h>
#include <stdio.h>

static jmp_buf env;
static void doJump(int nvar, int rvar, int vvar) {
  printf("Inside dojump(): nvar=%d, rvar=%d, vvar=%d\n", nvar, rvar, vvar);
  longjmp(env, 1);
}

int main(int argc, char *argv[]) {
  int nvar;
  register int rvar;  // 优先在寄存器中分配，如果可能的话。
  volatile int vvar;

  nvar = 111;
  rvar = 222;
  vvar = 333;
  if (setjmp(env) == 0) {
    nvar = 777;
    rvar = 888;
    vvar = 999;
    doJump(nvar, rvar, vvar);
  } else {
    printf("After longjmp(): nvar=%d, rvar=%d, vvar=%d\n", nvar, rvar, vvar);
  }

  return 0;
}