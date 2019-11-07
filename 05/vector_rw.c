// vector r/w.

// readv/writev 's argument: struct iovec:
//  struct iovec {
//    void *iov_base;
//    size_t iov_len;
//  }

#include <sys/uio.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#include <math.h> // for fabs();

#include <header.h>

struct subObject {
  int a;
  double b;
  float c;
};

struct Object {
  struct subObject sub;
  int x;
  char str[100];
};

size_t serialize(int fd, struct Object *obj) {
  struct iovec iv[3];
  iv[0].iov_base = &(obj->sub);
  iv[0].iov_len = sizeof(obj->sub);

  iv[1].iov_base = &(obj->x);
  iv[1].iov_len = sizeof(obj->x);

  iv[2].iov_base = obj->str;
  iv[2].iov_len = sizeof(obj->str);

  size_t total = sizeof(struct Object);
  ssize_t wlen;
  wlen = writev(fd, iv, 3);
  if (wlen == -1) {
    int err = errno;
    printf("writev error: %d\n", err);
    return 0;
  }
  
  return (size_t)wlen;
}

size_t unserialize(int fd, struct Object *obj) {
  struct iovec iv[3];
  iv[0].iov_base = &(obj->sub);
  iv[0].iov_len = sizeof(obj->sub);

  iv[1].iov_base = &(obj->x);
  iv[1].iov_len = sizeof(obj->x);

  iv[2].iov_base = obj->str;
  iv[2].iov_len = sizeof(obj->str);

  size_t total = sizeof(struct Object);
  ssize_t rlen;
  rlen = readv(fd, iv, 3);
  if (rlen == -1) {
    int err = errno;
    printf("readv error: %d\n", err);
    return 0;
  }
  
  return (size_t)rlen;
}

int sub_equal(const struct subObject *sub1, const struct subObject *sub2) {
  if (sub1 == NULL && sub2 == NULL)
    return 1;
  
  if (sub1 == NULL)
    return 0;

  if (sub2 == NULL)
    return 0;

  if (sub1->a != sub2->a)
    return 0;

  if (fabs(sub1->b - sub2->b) > 0.0000001)
    return 0;

  if (fabs(sub1->c - sub2->c) > 0.0000001)
    return 0;

  return 1;
}

int equal(const struct Object *obj1, const struct Object *obj2) {
  if (obj1 == NULL && obj2 == NULL)
    return 1;
  
  if (obj1 == NULL)
    return 0;

  if (obj2 == NULL)
    return 0;

  if (obj1->x != obj2->x)
    return 0;

  if (strcmp(obj1->str, obj2->str) != 0)
    return 0;

  return sub_equal(&(obj1->sub), &(obj2->sub));
}

int main() {
  char *file = "object.bin";
  int flags = O_RDWR | O_CREAT | O_TRUNC;
  mode_t mode = S_IRUSR | S_IWUSR;
  int fd;
  if ((fd = open(file, flags, mode)) == -1)
    errExit("open %s", file);

  struct subObject sub = {.a = 100, .b = 3.1415926, .c = 3.14f};
  struct Object obj = {.sub = sub, .x = 1024};
  strcpy(obj.str, "hello, world");

  size_t total = sizeof(struct Object);

  size_t wlen;
  wlen = serialize(fd, &obj);
  if (wlen != total) {
    printf("serialize size=%lu is not equal total=%lu\n", wlen, total);
    return -1;
  }

  lseek(fd, 0, SEEK_SET);

  struct Object obj2; 
  size_t rlen;
  rlen = unserialize(fd, &obj2);
  if (rlen != total) {
    printf("unserialize size=%lu is not equal total=%lu\n", rlen, total);
    return -1;
  }
  close(fd);

  if (!equal(&obj, &obj2))
    printf("not equal...\n");

  return 0;
}
