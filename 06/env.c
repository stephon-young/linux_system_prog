// environ 全局变量的访问
// getenv() 获取单个环境变量的值。
//  注意, SUSv3有如下规范
//      1. getenv()函数返回值，应用程序不应修改，
//         因为实现可能是对应的环境变量的存储位置，修改了后，就直接修改了环境变量。
//         如果确实需要修改，建议使用setenv或者putenv函数。
//      2. 允许getenv的返回值使用静态分配的缓冲区来返回结果，后续对getenv、setenv、
//         putenv或者unsetenv的函数调用可以重写该缓冲区。虽然glibc的实现并未使用这
//         样的缓冲区，所以如果要编写具备可移植性的程序，则需要先将返回值复制到其他
//         位置，之后方可发起其他函数调用。

#include <stdlib.h> // for environ
#include <stdio.h>
#include <string.h>

extern char **environ;

int main(int argc, char *argv[])
{
    for (char **ep = environ; *ep != NULL; ep++)
        printf("%s\n", *ep);

    char name[] = "HOSTTYPE";
    char *value;
    if ((value = getenv(name)) == NULL)
        printf("env[%s]=NULL\n", name);
    else
        printf("env[%s]=%s\n", name, value);

    char *val = malloc(strlen(value) + 1);
    strcpy(val, value);

    // 添加环境变量
    char *env = malloc(20);
    strcpy(env, "SHELL=/bin/bash");

    if (putenv(env) != 0)
        printf("add env %s failed\n", env);
    else
        printf("add env %s succeed\n", env);

    char sh[] = "SHELL";
    char *shval = getenv(sh);
    printf("get env[%s]=%s\n", sh, shval);

    // getenv()

    return 0;
}