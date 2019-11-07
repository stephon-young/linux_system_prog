# 深入探究文件I/O

## 1. 原子操作和竞争条件
- 创建文件和写入数据，多线程、多进程操作都存在竞争条件，通常有如下方法解决。
### 独占的方式创建文件
- 检测文件存在和创建文件，两个操作需要保证原子性，可以采用如下方式:
  - open的flag参数: O_CREAT | O_EXCL | ...
  - 这样，能够保证，检测到文件不存在则创建，如果文件存在返回errno为EEXIST错误。
  - 该操作是原子性的，不会出现竞态问题。
### 向文件尾部追加数据
- 通常如果多进程、多线程，持有对同一文件(这里指的是同一内核打开文件句柄，下文将会解析该结构)句柄的尾部追加数据，通常会共享文件偏移量。
  - 如下操作:
  ```c
  lseek(fd, 0, SEEK_END);
  char buf[] = "...";
  size_t len = sizeof(buf);
  write(fd, buf, len);
  ```
  - 如果有两个进程/线程同时执行这个操作，将出现竞态条件，先写的数据有可能被后写的数据覆盖。
  - 可以采用如下的方式来保证不出现竞态条件:
  ```c
  open(file, ... | O_APPEND, mode);
  ```
  - 即，在打开的时候添加`O_APPEND`标志。`O_APPEND`标志能保证上述的操作(仅write操作)是原子的。不会出现竞态条件。

## 2. 文件控制操作: fcntl()
- fcntl函数对一个打开的文件描述符执行一系列的控制操作。
- 原型: `int fcntl(int fd, int cmd, ...); // <fcntl.h>`
  - 参数: 
    - fd: 调用open返回的文件描述符，或者STDIN_FILENO/STDOUT_FILENO/STDERR_FILENO。
    - cmd: 所支持的控制操作命令。
    - ...: 有些命令需要参数，通常和cmd配合使用。
  - 返回值: 失败返回-1,成功返回的值和cmd相关。

## 3. 打开文件的状态标志
- 通过调用fcntl，并传入`F_GETFL`即可获取其访问模式和状态标志。
  - 例子:
  ```c
  int flags;
  flags = fcntl(fd, F_GETFL);
  if (flags == -1) {
    // handle error.
  }

  if (flags & O_SYNC) {
    // 存在 O_SYNC标志。
  }

  // 文件的访问模式标志位比较特殊：需要使用 返回值 & O_ACCMODE 获取。
  int accessMode;
  accessMode = flags & O_ACCMODE;
  if (accessMode == O_RDONLY) {
    // 只读
  }
  if (accessMode == O_WRONLY) {
    // 可写
  }
  if (accessMode == O_RDWR) {
    // 可读可写
  }
  ```
  - **注意**: 文件的访问模式标志位比较特殊：需要使用`返回值 & O_ACCMODE`获取
- 可以通过fcntl来修改打开文件的某些状态标志: O_APPEND/O_NONBLOCK/O_NOATIME/O_ASYNC/O_DIRECT
  - 例子:
  ```c
  int flags;
  flags = fcntl(fd, F_GETFL);
  if (flags == -1) {
    //handle error.
  }

  flags |= O_APPEND;
  if (fcntl(fd, F_SETFL, flags) == -1) {
    //handle error.
  }
  ```
  - **注意** 使用`F_SETFL`来设置flag，flag的设置需要先获取，在添加。

## 4. 文件描述符和打开文件之间的关系
- 文件描述符和打开文件通常都是一一对应的关系，但是也存在多个描述符关联一个打开的文件，要理清其中的关系，就需要了解三个内核的数据结构：
  - **进程级**的文件描述符表
  - **系统级**的打开文件表
  - **文件系统**的i-node表
- 进程级的文件描述符表包括如下信息:
  - 控制文件描述符操作的一组标志，目前只定义了一个: close-on-exec
  - 对打开文件句柄的引用。
- 系统级的打开文件表包含如下信息:
  - 当前文件偏移量
  - 打开文件所使用的状态标志，即flags参数。
  - 文件的访问模式，如调用open所设置的只读、可写或可读可写模式。
  - 与信号驱动IO相关的设置。
  - 对该文件i-node对象的引用
- 文件系统的i-node表包含如下信息:
  - 文件类型，指示是什么类型的文件: 常规文件，套接字或者FIFO，和访问权限。
  - 文件锁，通常是一个指向文件锁的指针。
  - 文件的各种属性: 文件大小，时间戳等。
- **例子**
  - 文件描述符同时指向同一个系统的打开文件句柄。
    - 进程内:
    ```c
    char *file = "abc.txt";
    int flags = O_RDWR;
    int fd;
    if ((fd = open(file, flags)) == -1) {
      //handle error.
    }

    int fd2;
    fd2 = dup(fd);
    // 这时fd2和fd指向了系统文件打开表的同一个句柄。
    ...
    ```
    - 进程间:
    ```c
    char *file = "abc.txt";
    int flags = O_RDWR;
    int fd;
    if ((fd = open(file, flags)) == -1) {
      //handle error.
    }

    pid_t pid;
    if ((pid = fork()) == -1) {
      //handle error.
    }

    if (pid == 0) {
      // 子进程的 fd
    } else {
      // 父进程的 fd
    }
    // 这时父子进程的fd指向了系统文件打开表的同一个句柄。
    ...
    ```
  - 系统文件打开表的两行指向同一个文件系统的i-node对象。无论是进程内同时打开两次同一个文件还是进程间分别打开同一个文件，都会出现这样的结果。
  ```c
    char *file = "abc.txt";
    int flags = O_RDWR;
    int fd;
    if ((fd = open(file, flags)) == -1) {
      //handle error.
    }

    int fd2;
    if ((fd2 = open(file, flags)) == -1) {
      // handle error.
    }
    // 这时fd2和fd分别对应系统打开文件表的两行，这两行最终指向文件系统的同一个i-node对象。
    ...
    ```
## 5. 复制文件描述符
- sh的重定向语法`2>&1`，里面的标准错误就和标准输出关联在一起了，就是将标准错误重定向向标准输出了。这时如何做到的呢？
- 其实很简单: 将标准错误的指向标准输出即可，说起来容易，做起来难，怎么做呢？
- 基于如下原理:
  - 文件描述符对于一个进程来说，总是从小到大分配。如果某个描述符被关闭了，将会确保新分配的一定是最小的未使用的。
  - 复制文件描述符。
### 复制文件描述符: dup()
- dup函数复制参数里面的文件描述符，并返回进程最小的未使用文件描述符。
  - 原型: `int dup(int oldfd); // <unistd.h>`
  - 参数: oldfd: 待复制的文件描述符。
  - 返回: 成功返回新的复制的文件描述符，失败返回-1.
  - 要完成上面的`2>&1`操作，可以写如下代码:
  ```c
  close(2); // 释放2，即标准错误文件描述符。
  dup(1);   // 复制1，即标准输出文件描述符，这时返回值将会是2，因为进程当前最小未使用的文件描述符是刚释放的2.
  ```
### 复制文件描述符: dup2()
- dup函数的返回值不易控制，所以才有dup2函数。
  - 原型: `int dup2(int oldfd, int newfd); // <unistd.h>`
  - 参数: 
    - oldfd: 待复制的文件描述符。
    - newfd: 复制后的最小的文件描述符。复制成功，能保证，复制的描述符不小于该值。
  - 返回值: 成功返回不小于newfd的最小的文件描述符，失败返回-1，当oldfd无效的时候，errno的值为EBADF。
  - **注意**: 传入的newfd，将会执行close操作，close失败，并不会处理，即默认不处理。如果`oldfd == newfd`，则不作任何操作，直接返回oldfd。
  - 要完成上面的`2>&1`操作，可以写如下代码:
  ```c
  dup2(1, 2); // 等价于先 close(2), 然后dup(1)
  ```
### 复制文件描述符: dup3()
- 与dup2不同的是，dup3带了一个flags参数，允许复制新的描述符后，设置新描述符的状态。
  - 原型: `int dup3(int oldfd, int newfd, int flags); // <unistd.h>`
  - 参数:
    - oldfd/newfd: 同上面的dup2的相关参数。
    - flags: 可以设置新的文件描述符的状态，当前只有一个状态可以设置: close-on-exec.
  - 返回值: 同dup2。
  - **注意**: 上面两个函数，新复制的fd的状态并不设置FD_CLOEXEC。
  - **注意**: 该函数只存在于Linux系统中。使用需要定义`_GNU_SOURCE`
  - 要完成上面的`2>&1`操作，可以写如下代码:
  ```c
  dup2(1, 2, O_CLOEXEC); // 等价于先 close(2), 然后dup(1)
  ```
### 复制文件描述符: fcntl()
- 可以使用`F_DUPFD`或者`F_DUPFD_CLOEXEC`命令来复制文件描述符。
  - 要完成上面的`2>&1`操作，可以写如下代码:
  ```c
  close(2);
  fcntl(1, F_DUPFD); // 还可以使用 fcntl(1, F_DUPFD_CLOEXEC) 等价于dup3上面的例子的功能。
  ```
## 6. 在文件特定偏移处的IO: pread()和pwrite()
- pread/pwrite函数，仅在read/write函数参数里面多了一个偏移量参数，所以，这两个函数的读取和写入**不受文件偏移指针影响**。
  - 原型: `<unistd.h>`
    - `ssize_t pread(int fd, void *buf, size_t count, off_t offset);`
    - `ssize_t pwrite(int fd, const void *buf, size_t count, off_t offset);`
  - 通常在多线程，存在竞态条件的时候，该函数能避免竞态。

## 7. 分散输入和集中输出: readv()和writev()
- readv/writev函数，用于批量将分块的数据原子性的读取、写入文件。
  - 原型: `<sys/uio.h>`
    - `ssize_t readv(int fd, const struct iovec *iov, int iovcnt);`
    - `ssize_t writev(int fd, const struct iovec *iov, int iovcnt);`
  - 两个函数都有一个相同类型的参数`iov`，其类型定义如下:
  ```c
  struct iovec {
    void *iov_base; // 数据起始地址
    size_t iov_len; // 数据长度
  }
  ```
  - 而两个函数，都是传入了一个`struct iovec`的数组。
  - 个人认为: 这两个函数是专门为序列化和反序列化使用的。
  - **注意**: 这两个函数是原子的。
  - **注意**: 通常要检查返回值，防止部分写入。
- (linux-special) 在特定偏移处分散输入，集中输出:
  - 原型: `<sys/uio.h> _BSD_SOURCE`
    - `ssize_t preadv(int fd, const struct iovec *iov, int iovcnt, off_t offset);`
    - `ssize_t pwritev(int fd, const struct iovec *iov, int iovcnt, off_t offset);`

## 8. 截断文件: truncate()和ftruncate()
- truncate/ftruncate函数可以截断文件到指定长度。
  - 原型: `<unistd.h>`
    - `int truncate(const char *pathname, off_t length);`
    - `int ftruncate(int fd, off_t length);`
  - 参数:
    - pathname: 文件路径，通常需要如下权限: 可访问该文件(对文件的目录有x权限); 对文件有写权限。
    - fd: open函数打开的合法的文件描述符。
    - length: 截断后的文件长度
    - **注意** length超出文件长度，两者的标准处理方式不同:
      - ftruncate函数，SUSv3允许两种行为: **要么扩展，要么返回错误**
      - truncate函数，SUSv3要求**总是能扩展文件**
      - 扩展的方式:可以添加空字节，或者一个文件空洞。
    
## 9. 非阻塞IO
- 打开文件的时候使用`O_NONBLOCK`标志，有两种目的:
  - 如果调用open()函数未能立即打开文件，则返回错误。(调用open()操作FIFO的时候可能会陷入阻塞，这种情况例外)。
  - 调用open()成功后，**后续的IO操作也是非阻塞的**。
    - 若后续的IO系统调用未能立即完成，则可能会只传输部分数据，或者系统调用失败，这时返回`EAGAIN`或者`EWOULDBLOCK`，linux和许多unix实现一样，将两个错误常量视为同义。
  - 管道、FIFO、套接字和设备都支持非阻塞模式。
  - pipe、套接字的创建并不是使用open创建的，所以设置其`O_NONBLOCK`只能用fcntl函数进行设置。
    - **注意** socket在linux2.6.27版本之后可以在创建的时候添加该标志:
      - `int socket(int domain, int type, int protocol); //<sys/socket.h>`
      - 在第二个参数中可以使用`type |= SOCK_NONBLOCK;`来添加`O_NONBLOCK`标志，后续可以通过fcntl检测，其确实存在了该标志位。
  - 普通文件通常都有内核缓冲区，可以保证IO操作的不阻塞，所以对于普通文件一般会忽略`O_NONBLOCK`标志。但是**在使用强制文件锁的时候，该标志对普通文件还是起作用的**。

  
