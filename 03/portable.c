// for portable:
// _POSIX_SOURCE/_POSIX_C_SOURCE
// _XOPEN_SOURCE
// _BSD_SOURCE
// _SVID_SOURCE
// _GNU_SOURCE

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#if defined(_POSIX_SOURCE)
long posix = _POSIX_SOURCE;
#else
long posix = -1;
#endif

#if defined(_POSIX_C_SOURCE)
long posix_c = _POSIX_C_SOURCE;
#else
long posix_c = -1;
#endif

#if defined(_XOPEN_SOURCE)
long xopen = _XOPEN_SOURCE;
#else
long xopen = -1;
#endif

#if defined(_BSD_SOURCE)
long bsd = _BSD_SOURCE;
#else
long bsd = -1;
#endif

#if defined(_SVID_SOURCE)
long svid = _SVID_SOURCE;
#else
long svid = -1;
#endif

#if defined(_GNU_SOURCE)
long gnu = _GNU_SOURCE;
#else
long gnu = -1;
#endif

void portable_type_print();
void portable_struct_initialize();

int main() {
  if (posix != -1)
    printf("_POSIX_SOURCE defined = %ld\n", posix);

  if (posix_c != -1)
    printf("_POSIX_C_SOURCE defined = %ld\n", posix_c);

  if (xopen != -1)
    printf("_XOPEN_SOURCE defined = %ld\n", xopen);

  if (bsd != -1)
    printf("_BSD_SOURCE defined = %ld\n", bsd);

  if (svid != -1)
    printf("_SVID_SOURCE defined = %ld\n", svid);

  if (gnu != -1)
    printf("_GNU_SOURCE defined = %ld\n", gnu);

  // _XOPEN_SOURCE value:
  // 500 include _POSIX_C_SOURCE=199506L
  // 600 include _POSIX_C_SOURCE=200112L
  // 700 include _POSIX_C_SOURCE=200809L

  portable_type_print();
  portable_struct_initialize();

  return 0;
}

void portable_type_print() {
  pid_t pid = getpid();
  pid_t ppid = getppid();

  printf("my self pid_t is %ld\n", (long)pid);
  printf("my self ppid_t is %ld\n", (long)ppid);

  // off_t is long long type.
  off_t x = 12345;
  printf("off_t type is long long: %lld\n", (long long)x);

  // can use c99 z for size_t/ssize_t type. but unix is not supported.
  size_t y = 10000;
  ssize_t z = 20000;
  printf("size_t type can be use printf with %%zd at c99, but not be supported by UNIX! value=%zd\n", y);
  printf("ssize_t type can be use printf with %%zd at c99, but not be supported by UNIX! value=%zd\n", z);

  // can use c99 j for all integer type. match all integer type. but unix is not supported. 
  printf("off_t type can be use printf with %%jd at c99, but not be supported by UNIX! value=%jd\n", x);
  printf("pid_t type can be use printf with %%jd at c99, but not be supported by UNIX! value=%jd\n", (long)pid);
}

void portable_struct_initialize() {
  struct sembuf sbuf;
  // define by SUSv3 but not define the order of sembuf's element.
  // so initialize DONT use the following.
  struct sembuf buf = {3, -1, SEM_UNDO};

  // you should use the following for portable.
  struct sembuf b;
  b.sem_num = 3;
  b.sem_op = -1;
  b.sem_flg = SEM_UNDO;

  printf("struct sembuf initialize should call s.<element> format.\n");
}