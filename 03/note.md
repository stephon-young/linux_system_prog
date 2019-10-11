## 系统调用/库函数返回值以及错误处理

### 系统调用返回值
- 通常系统调用失败返回-1，同时设置errno值。调用成功，并不设置errno，所以编程时需要注意：
  - 先检查返回值，当出现错误后，在检查errno.
- 极少数系统调用成功也会返回-1，这时就需要先把errno设置为0，再进行系统调用，再判断errno是否为零来判断是否出错。
### 库函数返回值
- 通常库函数调用失败返回-1，同时也会设置errno的值。
- 有些库函数调用失败返回其他值，如fopen等，同时也会设置errno的值。
- 有些库函数根本不使用errno。
### 错误处理
- perror函数能够打印错误的友好提示。
- strerror函数能够转换输入参数错误号的对应字符串。注意这个函数不是线程安全的，也可以理解为不是可重入的。如果要求线程安全，则可以调用strerror_r函数。
## glibc版本
- 在shell中获取glibc的版本:
  - 执行`/lib/libc.so.6`，通常这个路径是针对x86平台的，如果是其他平台，可以使用如下方式查找其路径:
    - `ldd <binary file> | grep libc`
- 在编程时获取glibc的版本
  - 宏`__GLIBC__`和`__GLIBC_MINOR__`可以分别获取主次版本。这样获取会存在局限:编译好的程序移植到其他机器，就不起作用了。
  - 函数`gnu_get_libc_version()`函数可以返回glibc的版本字符串。
  - 函数`confstr()`函数，传入`_CS_GNU_LIBC_VERSION`参数，将返回类似于`glibc x.y`的形式的字符串。注意这个是`gnu`特有的。

### 可移植问题
- 特征测试宏`#include <features.h>` `man test_feature_macros`
- 在glibc的2.27版本上的表现结果:
  ```
  Default definitions, implicit definitions, and combining definitions
    If  no  feature  test  macros  are  explicitly  defined, then the following feature test macros are defined by default: _BSD_SOURCE (in glibc 2.19 and earlier), _SVID_SOURCE (in glibc 2.19  and  earlier),  _DEFAULT_SOURCE(since glibc 2.19), _POSIX_SOURCE, and _POSIX_C_SOURCE=200809L (200112L in glibc versions before 2.10; 199506L in glibc versions before 2.4; 199309L in glibc versions before 2.1).                                                                                                                                  
    
    If any of __STRICT_ANSI__, _ISOC99_SOURCE, _POSIX_SOURCE, _POSIX_C_SOURCE, _XOPEN_SOURCE,_XOPEN_SOURCE_EXTENDED,  _BSD_SOURCE  (in glibc 2.19 and earlier), or _SVID_SOURCE (in glibc 2.19 and earlier) is explicitly defined, then _BSD_SOURCE, _SVID_SOURCE, and _DEFAULT_SOURCE are not defined by default.                                                                                            
    
    If _POSIX_SOURCE and _POSIX_C_SOURCE are not explicitly defined, and either __STRICT_ANSI__ is not defined  or _XOPEN_SOURCE is defined with a value of 500 or more, then                                                                                                *  _POSIX_SOURCE is defined with the value 1; and                                                                                      *  _POSIX_C_SOURCE is defined with one of the following values:                                                                          ·  2, if _XOPEN_SOURCE is defined with a value less than 500;                                                                          ·  199506L, if _XOPEN_SOURCE is defined with a value greater than or equal to 500 and less than 600; or                                ·  (since  glibc  2.4)  200112L,  if _XOPEN_SOURCE is defined with a value greater than or equal to 600 and less than 700.             ·  (Since glibc 2.10) 200809L, if _XOPEN_SOURCE is defined with a value greater than or equal to 700.                                  ·  Older versions of glibc do not know about the values 200112L and 200809L for  _POSIX_C_SOURCE,  and  the setting of this macro will depend on the glibc version.                                                                                                          ·  If  _XOPEN_SOURCE  is  undefined,  then  the  setting  of  _POSIX_C_SOURCE depends on the glibc version: 199506L, in glibc versions before 2.4; 200112L, in glibc 2.4 to 2.9; and 200809L, since glibc 2.10.   
    
    Multiple macros can be defined; the results are additive. 
  ```
- 通过上面可以看到:默认定义的只有两个宏: `_POSIX_SOURCE=1`和`_POSIX_C_SOURCE=200809L`
