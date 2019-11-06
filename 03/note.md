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

#### 特性测试宏
- 特性测试宏，包含在`#include <features.h>`，可以用`man test_feature_macros`查看帮助
- 在glibc的2.27版本上的表现结果:
  ```
  Default definitions, implicit definitions, and combining definitions
    If  no  feature  test  macros  are  explicitly  defined, then the following feature test macros are defined by default: _BSD_SOURCE (in glibc 2.19 and earlier), _SVID_SOURCE (in glibc 2.19  and  earlier),  _DEFAULT_SOURCE(since glibc 2.19), _POSIX_SOURCE, and _POSIX_C_SOURCE=200809L (200112L in glibc versions before 2.10; 199506L in glibc versions before 2.4; 199309L in glibc versions before 2.1).                                                                                                                                  
    
    If any of __STRICT_ANSI__, _ISOC99_SOURCE, _POSIX_SOURCE, _POSIX_C_SOURCE, _XOPEN_SOURCE,_XOPEN_SOURCE_EXTENDED,  _BSD_SOURCE  (in glibc 2.19 and earlier), or _SVID_SOURCE (in glibc 2.19 and earlier) is explicitly defined, then _BSD_SOURCE, _SVID_SOURCE, and _DEFAULT_SOURCE are not defined by default.                                                                                            
    
    If _POSIX_SOURCE and _POSIX_C_SOURCE are not explicitly defined, and either __STRICT_ANSI__ is not defined  or _XOPEN_SOURCE is defined with a value of 500 or more, then                                                                                                *  _POSIX_SOURCE is defined with the value 1; and                                                                                      *  _POSIX_C_SOURCE is defined with one of the following values:                                                                          ·  2, if _XOPEN_SOURCE is defined with a value less than 500;                                                                          ·  199506L, if _XOPEN_SOURCE is defined with a value greater than or equal to 500 and less than 600; or                                ·  (since  glibc  2.4)  200112L,  if _XOPEN_SOURCE is defined with a value greater than or equal to 600 and less than 700.             ·  (Since glibc 2.10) 200809L, if _XOPEN_SOURCE is defined with a value greater than or equal to 700.                                  ·  Older versions of glibc do not know about the values 200112L and 200809L for  _POSIX_C_SOURCE,  and  the setting of this macro will depend on the glibc version.                                                                                                          ·  If  _XOPEN_SOURCE  is  undefined,  then  the  setting  of  _POSIX_C_SOURCE depends on the glibc version: 199506L, in glibc versions before 2.4; 200112L, in glibc 2.4 to 2.9; and 200809L, since glibc 2.10.   
    
    Multiple macros can be defined; the results are additive. 
  ```
- 通过上面可以看到:默认定义的只有两个宏: `_POSIX_SOURCE=1`和`_POSIX_C_SOURCE=200809L`
- `_BSD_SOURCE`和`_SVID_SOURCE`在2.19之后就被废弃了。
- `_POSIX_SOURCE`和`_POSIX_C_SOURCE`在不同的glibc版本有不同的值。
- `_XOPEN_SOURCE`的值定义为500、600、700时分别对应SUSv2、SUSv3、SUSv4扩展的支持，定义了三者的同时，将自动包含`_POSIX_C_SOURCE`的定义:
  - 500: `_POSIX_C_SOURCE=199506L`
  - 600: `_POSIX_C_SOURCE=200112L`
  - 700: `_POSIX_C_SOURCE=200809L`
  - `_XOPEN_SOURCE`定义表明了不仅对posix支持，还对SUSvx的支持。
  - 通常可以这样编译程序: `gcc -std=c99 -D_XOPEN_SOURCE=500/600/700 xxx.c` 分别对应500、600、700。

#### 系统数据类型
- 系统数据类型，***不同于***C语言的基本类型，考虑到在不同系统之间的可移植性，通常都是采用`typedef`来重新定义的类型，如: `pid_t`表示进程id。该类型在不同的平台上可能是不同的类型。通常如果我们需要考虑程序的可移植性，则需要用***标准的系统数据类型***,这些类型大多定义在`<sys/types.h>`中，也有少量定义在其他文件中。
- 定义了这些可移植性的标准是在SUSv3中规范的。
  数据类型 | SUSv3类型需求 | 描述
  |  :-:  |     :-:     | - |
  blkcnt_t | 有符号类型 | 文件块数量
  blksize_t | 有符号类型 | 文件块大小
  cc_t | 无符号类型 | 终端特殊字符
  clock_t | 整型或浮点型实数 | 以时钟周期计量的系统时间
  clockid_t | 运算类型之一 | 针对POSIX.1b时钟和定时器函数的时钟标志符
  comp_t | 未做规范 | 经由压缩处理的时钟周期
  dev_t | 运算类型之一 | 设备号，包含主、次设备号
  DIR | 无类型要求 | 目录流
  fd_set | 结构类型 | select()中的文件描述符集合
  fsblkcnt_t | 无符号类型 | 文件系统块数量
  fsfilcnt_t | 无符号类型 | 文件数量
  gid_t | 整型 | 数值型组标识符
  id_t | 整型 | 用以存放标识的通用类型，其大小至少可以放置pid_t、uid_t和gid_t类型
  in_addr_t | 32位无符号整型 | IPv4地址
  in_port_t | 16位无符号整型 | IP端口号
  ino_t | 无符号整型 | 文件i-node号
  key_t | 运算类型之一 | System V IPC键
  mode_t | 整型 | 文件权限及类型
  mqd_t | 无类型要求，但不能为数组类型 | POSIX消息队列描述符
  msglen_t | 无符号整型 | System V 消息队列所允许的字节数
  msgqnum_t | 无符号整型 | System V 消息队列中的消息数量
  nfds_t | 无符号整型 | poll()中的文件描述符数量
  nlink_t | 整型 | 文件的(硬)链接数量
  off_t | 有符号整型 | 文件偏移或大小
  pid_t | 有符号整型 | 进程ID、进程组ID或会话ID
  ptrdiff_t | 有符号整型 | 两指针差值，为有符号整型
  rlim_t | 无符号整型 | 资源限制
  sa_family_t | 无符号整型 | 套接字地址簇
  shmatt_t | 无符号整型 | 与System V 共享内存段相连的进程数量
  sig_atomic_t | 整型 | 可进行原子访问的数据类型
  siginfo_t | 结构类型 | 信号起源的相关信息
  sigset_t | 整型或结构类型 | 信号集合
  size_t | 无符号整型 | 对象大小
  socklen_t | 至少32位整型 | 套接字地址结构大小
  speed_t | 无符号整型 | 终端线速度
  ssize_t | 有符号整型 | 字节数或标识错误
  stack_t | 结构类型 | 对备选信号栈的描述
  suseconds_t | 有符号整型，范围为[-1, 1000000] | 微秒级的时间间隔
  tcflag_t | 无符号整型 | 终端模式标志位的位掩码
  time_t | 整型或浮点型实数 | 自所谓纪元(Epoch)开始，以秒计的日历时间
  timer_t | 运算类型之一 | POSIX.1b间隔定时器函数的定时器标识符
  uid_t | 整型 | 数值型用户标识符

- 打印系统数据类型值
  - 通常通过类型转换，即可打印，如pid_t转换为long类型， off_t转换为long long类型。
  - 注意，c99中，可以使用%zd用printf格式化输出size_t/ssize_t类型，***但是UNIX并不支持***。
  - 注意，c99中，可以使用%jd用printf格式化输出所有的整型，包括128位的整型，能够自适应，***但是UNIX也不支持***。

#### 其他可移植性问题
- 结构体初始化
  - 结构体定义后，但是其内部包含的字段的顺序并没有规定顺序，规范定义了结构包含了什么，但是没有定义其顺序(SUSv3).
    - 例子
    ```c
    struct sembuf {
      unsigned short sem_num;
      short sem_op;
      short sem_flg;
    };
    ```
    - 可以进行如下方式进行初始化:
    ```c
    struct sembuf s = {3, -1, SEM_UNDO};
    ```
    - 但是这样会存在问题，在不同的系统上`struct sembuf`内部的字段的顺序并不一致，可能会存在问题。
    - 推荐使用下面的初始化方式:
    ```c
    struct sembuf s;
    s.sem_num = 3;
    s.sem_op = -1;
    s.sem_flg = SEM_UNDO;
    ```
    - 或者使用c99语法:
    ```c
    struct sembuf s = {.sem_num = 3, .sem_op = -1, .sem_flg = SEM_UNDO};
    ```
  - 标准结构转储到文件时，需要定义每个字段的严格顺序。
- 有些宏并未定义
  - WCOREDUMP宏在很多地方都有定义，但是SUSv3并未对其规范，所以要妥善处理此类潜在性的可移植性问题。推荐做法如下：
  ```c
  #ifdef WCOREDUMP
    /* use WCOREDUMP macro code */
  #endif
  ```
- 不同实现间所需头文件的变化。

