// longjmp时setjmp的调用函数已经退出的验证。

// 这时可能的输出: (未添加data数组)
//  initialize for setjmp()...
//  setjmp caller is exit...
//  longjmp to there...
//  setjmp caller is exit...
//  Segmentation fault (core dumped)
// 最终程序crash了。

// 这是可能的输出: (添加了data数组)
// initialize for setjmp()...
// setjmp caller is exit...
// longjmp to there...
// longjmp data:
// setjmp caller is exit...
// *** stack smashing detected ***: <unknown> terminated
// Aborted (core dumped)

#include <setjmp.h>
#include <unistd.h> // for sleep()
#include <stdio.h>

static jmp_buf env;

static void set_jump() {
  char data[100] = {0};
  switch (setjmp(env)) {
  case 0:
    printf("initialize for setjmp()...\n");
    break;
  case 1:
    data[99] = '\0';
    printf("longjmp to there...\n");
    printf("longjmp data: %s\n", data);
    break;
  }

  printf("setjmp caller is exit...\n");
}

int main(int argc, char *argv[]) {
  set_jump();

  sleep(2);

  longjmp(env, 1);

  return 0;
}