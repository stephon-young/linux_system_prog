/*
 chapter 03 example for reboot() function for liniux-special.
 */

#include <unistd.h>
#include <linux/reboot.h>

#include <stdio.h>
#include <string.h>
#include <errno.h>

// declare. if you don't declare, compiler will complain for warning: can't find the declare of reboot, so you 
// should declare, it's implement will be linked auto.
int reboot(int magic, int magic2, int cmd, void *arg);

// define: 
//  int reboot(int magic, int magic2, int cmd, void *arg); 
// desc:[man 2 reboot] output.
//  This system call fail (with the error EINVAL) unless magic equals LINUX_REBOOT_MAGIC1  (that  is,  0xfee1dead)
//  and  magic2  equals LINUX_REBOOT_MAGIC2 (that is, 672274793).  However, since 2.1.17 also LINUX_REBOOT_MAGIC2A
//  (that is, 85072278) and since 2.1.97 also LINUX_REBOOT_MAGIC2B (that is,  369367448)  and  since  2.5.71  also
//  LINUX_REBOOT_MAGIC2C  (that  is,  537993216)  are  permitted as values for magic2.  (The hexadecimal values of
//  these constants are meaningful.)   
//  当调用reboot时，magic的值为LINUX_REBOOT_MAGIC1，并且magic2的值为下面的值时：
//    LINUX_REBOOT_MAGIC2=672274793
//    LINUX_REBOOT_MAGIC2A=85072278
//    LINUX_REBOOT_MAGIC2B=368367448
//    LINUX_REBOOT_MAGIC2C=537993216
//  调用成功。

int main() {

  printf("uid=%ld\n", (long)getuid());

  int magic = LINUX_REBOOT_MAGIC1;
  int magic2 = LINUX_REBOOT_MAGIC2;

  int cmd = LINUX_REBOOT_CMD_RESTART;

  int x;
  x = reboot(magic, magic2, cmd, NULL);
  int err = errno;
  printf("reboot return: %d\n", x);
  if (x != 0)
    printf("reboot call failed reason: %s\n", strerror(err));
  return x;
}   