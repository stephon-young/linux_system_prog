// GNU C中获取进程的文件名。
//  program_invocation_name/procgram_invocation_short_name
//  需要引用<errno.h>并定义 _GNU_SOURCE
//
//  man pages ---------------
// 
//SYNOPSIS
//       #define _GNU_SOURCE         /* See feature_test_macros(7) */
//       #include <errno.h>
//
//       extern char *program_invocation_name; 
//       extern char *program_invocation_short_name;
//
// /proc/<pid>/cmdline文件中也可以获取任一进程的命令行参数。
//

#define _GNU_SOURCE

#include <errno.h>
#include <stdio.h>

int main(int argc, char *argv[]) {
  // program_invocation_name等价于argv[0]
  printf("GNU C get the program invocation name: %s\n", program_invocation_name);
  printf("GNU C get the program invocation short name: %s\n", program_invocation_short_name);

  return 0;
}