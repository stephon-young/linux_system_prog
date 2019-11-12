// 修改环境变量，setenv/putenv/getenv/unsetenv/clearenv.
// 

#define _GNU_SOURCE // for clearenv: NO SUSv3 special.
#include <stdlib.h>

#include <header.h>

extern char **environ;

int main(int argc, char *argv[]) {
  char **ep;

  clearenv();
  if (environ == NULL)
    printf("environ is null\n");

  // 虽然 environ为空，但是setenv函数创建一个新的环境列表。
  if (setenv("NEW", "Hello world", 0) == -1)
    errExit("setenv");


  for (int i = 0; i < argc; i++) {
    // 虽然 environ为空，但是putenv函数创建一个新的环境列表。
    if (putenv(argv[i]) != 0)
      errExit("putenv %s", argv[i]);
  }

  if (setenv("GREET", "Hello world", 0) == -1)
    errExit("setenv");

  for (ep = environ; *ep != NULL; ep++)
    puts(*ep);

  return 0;
}


