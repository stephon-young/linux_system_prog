// longjmp 示例

#include <setjmp.h>
#include <stdio.h>

jmp_buf env;

static void f2() {
  longjmp(env, 2);
}

static void f1(int argc) {
  if (argc == 1)
    longjmp(env, 1);

  f2();
}

int main(int argc, char *argv[]) {
  switch (setjmp(env)) {
  case 0:
    printf("call setjmp for initialize...\n");
    f1(argc);
    break;
  case 1:
    printf("we jump back from f1()\n");
    break;
  case 2:
    printf("we jump back from f2()\n");
    break;
  }

  return 0;
}