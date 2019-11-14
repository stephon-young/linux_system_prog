// 用getenv/putenv或者修改environ来实现setenv/unsetenv函数。
// 注意unsetenv函数将清除多个同名的环境变量。

#define _GNU_SOURCE

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

extern char **environ;

char *make_env(const char *name, const char *value) {
  char *env;
  size_t nlen = strlen(name);
  size_t vlen = strlen(value);
  if ((env = malloc(nlen + vlen + 2)) == NULL)
    return NULL;

  strcpy(env, name);
  strcpy(env + nlen, "=");
  strcpy(env + nlen + 1, value);

  return env;
}

int my_setenv(const char *name, const char *value, int overwrite) {
  if (!overwrite) {
    char *val;
    if ((val = getenv(name)) != NULL)
      return 0;
  }

  char *env;
  if ((env = make_env(name, value)) == NULL)
    return -1;

  if (putenv(env) != 0)
    return -1;

  return 0;
}

int env_remove(const char *name, const char *value) {
  char **last;
  char **find;

  char *env;
  if ((env = make_env(name, value)) == NULL)
    return -1;

  //找出结尾的那个，填充删除的那个。
  // glibc对putenv进行了扩展，可以使用putenv("NAME")来删除某个环境变量。
  for (char **ep = environ; *ep != NULL; ep++) {
    if (*ep != NULL)
      last = ep;
    if (strcmp(*ep, env) == 0)
      find = ep;
  }

  free(env);

  //printf("find: %s\n", *find);
  //printf("last: %s\n", *last);

  if (find == NULL || last == NULL)
    return -1;
  
  *find = *last;
  *last = NULL;

  return 0;
}

int my_unsetenv(const char *name) {
  int times = 0;
  char *value;
  for (;;) {
    if ((value = getenv(name)) == NULL) 
      break;

    env_remove(name, value);
    times++;
  }
  if (times > 0)
    return 0;

  return -1;
}

int my_unsetenv2(const char *name) {
  // for glibc extesion.
  if (putenv((char *)name) != 0)
    return -1;
  
  return 0;
}

void printenv() {
  for (char **ep = environ; *ep != NULL; ep++)
    printf("%s\n", *ep);
}

int main(int argc, char *argv[]) {
  clearenv();

  //unsetenv("LS_COLORS");
  //unsetenv("PATH");

  setenv("A", "aaaaa", 0);
  setenv("B", "bbbbb", 0);
  setenv("C", "ccccc", 0);
  setenv("D", "ddddd", 0);
  setenv("E", "eeeee", 0);
  setenv("F", "fffff", 0);

  printf("setenv over...\n");
  printenv();

  printf("test my function\n");

  printf("setenv(A, AAAAA, 0): ");
  if (my_setenv("A", "AAAAA", 0) == 0)
    printf("ok\n");
  else
    printf("failed\n");
    
  //printf("%d\n", setenv("A", "AAAAA", 0));
  //return 0;

  printf("setenv(A, AAAAA, 1): ");
  if (my_setenv("A", "AAAAA", 1) == 0)
    printf("ok\n");
  else
    printf("failed\n");

  printf("unsetenv(B): ");
  if (my_unsetenv("B") == 0)
    printf("ok\n");
  else
    printf("failed\n");

  //printenv();
  //return 0;

  printf("unsetenv(G): ");
  if (my_unsetenv("G") != 0)
    printf("ok\n");
  else
    printf("failed\n");

  printf("unsetenv2(C): ");
  if (my_unsetenv2("C") == 0)
    printf("ok\n");
  else
    printf("failed\n");

  printf("unsetenv2(H): ");
  if (my_unsetenv("H") != 0)
    printf("ok\n");
  else
    printf("failed\n");

  printf("test over, env:\n");
  printenv();

  return 0;
}

