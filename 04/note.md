# 文件IO:通用的IO模型
- 本章关注磁盘文件IO操作。
- 通用IO模型的操作: 
  - 打开文件
  - 关闭文件
  - 从文件中读取数据
  - 向文件中写数据
## 1. 概述
- 所有执行IO操作的系统调用都是以***文件描述符***，一个非负整数（通常是小整数）来指代打开的文件，包括:
  - 管道(pipe)
  - FIFO
  - socket
  - 终端
  - 设备
  - 普通文件
- ***注意*** 每个进程，描述符都自成一套。
- shell中运行命令则由shell打开0/1/2三个文件描述符，确切的说，是命令继承了shell的文件描述符副本
  - 0: stdin:  标准输入: STDIN_FILENO
  - 1: stdout: 标准输出: STDOUT_FILENO
  - 2: stderr: 标准错误: STDERR_FILENO、
  - 通常可以使用0/1/2的方式，但是更为推荐的是采用<unistd.h>中定义的: STDIN_FILENO/STDOUT_FILENO/STDERR_FILENO
  - ***注意*** freopen在重新打开流的时候更换隐匿在其后的文件描述符
    - 原型: `FILE* freopen(const char *pathname, const char *mode, FILE *stream);`
    - 如: 针对stdout调用freopen后，并不保证stdout的值仍然为1.

### 1.1 通用函数:
- open:
  - 原型: `int open(const char *pathname, int flag, ... /* mode_t mode */); // <sys/stat.h>/<fcntl.h>`
  - 描述: 打开pathname所标识的文件，并返回文件描述符，当文件不存在的时候，可以使用flag标志位进行创建，同时可以用mode来指定创建时文件的权限，该参数是可选的，当不指定创建标志位的时候，并不需要mode参数。
- read:
  - 原型: `ssize_t read(int fd, void *buffer, size_t count); // <unistd.h>`
  - 描述: 从open调用返回的文件读取至多count个字节，存储到buffer中，返回读取的字节数。buffer需要用户自行申请，空间至少位buffer个字节。
- write:
  - 原型: `ssize_t write(int fd, const void *buffer, size_t count); // <unistd.h>`
  - 描述: 向open调用分会的文件写入至多count个字节的缓冲区buffer中的数据，返回写入的字节数。
- close:
  - 原型: `int close(int fd); // <unistd.h>`
  - 描述: 当所有的输入、输出操作完成后，调用close关闭文件，释放文件描述符以及内核相关资源。

## 2. 通用I/O
- 上面描述的通用函数，通常可以作用于所有类型的文件，包括终端之类的设备。一切皆文件是UNIX系统的精髓。对于特定的文件，如设备等，可以使用ioctl接口来实现特定的功能。
## 3. 打开一个文件: open
- 原型: `int open(const char *pathname, int flag, ... /* mode_t mode */); // <sys/stat.h>/<fcntl.h>`
- 参数: 
  - pathname: 需要打开的文件名，如果是一个符号链接，则会被解引用。
  - flag: 如下描述
### 3.1 flag参数:
- 1. 打开文件时的访问模式:
  - O_RDONLY: 只读
  - O_WRONLY: 只写
  - O_RDWR: 可读可写
  - ***注意*** 上面的标志位并不能用`|`进行组合。
- 2. 文件创建标志:
  - O_CLOEXEC: 设置close_on_exec标志
  - O_CREAT: 文件不存在则创建，这时mode参数指定创建文件的权限，通常文件创建的权限还受***进程的umask值***和***父目录的默认访问控制列表***影响。
  - O_DIRECT: 无缓冲的输入/输出
  - O_DIRECTORY: 如果文件不是目录，则失败。
  - O_EXCL: 和O_CREAT结合使用，来判断文件是否为本进程创建，如果文件存在则失败，如果不存在则创建。
  - O_LARGEFILE: 32位系统中使用此标志打开大文件。
  - O_NOATIME: 这个比较复杂:
    - 特权进程: 当文件的所有者和进程的有效用户id相匹配，或者进程拥有特权CAP_FOWNER，才能使用此标志
    - 非特权进程: 文件用户id必须匹配进程的文件系统用户id，而不是进程有效用户id。
    - ***注意*** 此标志是linux的特有扩展，必须使用`_GNU_SOURCE`宏，用于索引和备份程序服务，减少磁盘活动量。
  - O_NOCTTY: 如果打开的是终端设备，则此标志防止称为控制终端。如果不是终端设备，此标志无效。
  - O_NOFOLLOW: 对符号链接不予解引用。
  - O_TRUNC: 截断已有文件，使其长度为零，***在linux下无论以读、写方式打开文件都可清空内容***，但是要求有写权限。
- 3. 已打开文件的状态标志:
  - O_APPEND: 总是在文件尾部追加数据
  - O_ASYNC: linux中调用open指定该标志位，无任何效果，如果要使用需要使用fcntl来设置。该标志位的作用在于，当文件描述符可以实施I/O时，系统会参数一个信号通知进程，即信号驱动I/O，仅对特定类型的文件有效: 终端、FIFO和socket。
  - O_DSYNC: 根据同步I/O数据完整性的完成要求来执行文件的写操作。***难以理解???***
  - O_NONBLOCK: 以非阻塞方式打开
  - O_SYNC: 以同步方式写入文件。
- 4. `/proc/PID/fdinfo`
  - 该目录下面有进程<PID>打开的文件描述符的相关信息，里面的每个文件都按照fd-文件描述符命名，内容为:
  ```
  pos:    0
  flags:  02
  mnt_id: 24
  ```
  - 其中pos为文件的偏移量，flags为文件的打开标志位，mnt_id是文件挂载点id

### 3.2 open函数的错误
- 当open调用失败的时候，函数返回-1，在全局变量errno中设置错误原因，可能的错误如下:
  - EACCESS: 文件权限不允许。
  - EISDIR: 对目录打开进行写操作，不被允许。
  - EMFILE: 进程打开的文件描述符已经达到了进程资源限制的上限，可以参考RLIMIT_NOFILE参数。
  - ENFILE: 文件打开数量已经达到了系统允许的上限。
  - ENOENT: 文件不存在且未指定O_CREAT标志位，或者指定了O_CREAT，但是pathname的目录之一不存在，或者pathname为符号链接，但是为空链接。
  - EROFS: 只读文件系统，却以写方式打开。
  - ETXTBSY: 所指定的文件为可执行文件，并且正在运行。

### 3.3 creat函数
- creat函数可以用于创建一个新文件，但是不再推荐，推荐使用open函数来创建文件。
  - 原型: `int creat(const char *pathname, mode_t mode); // <fcntl.h>`
  - 等价于: `open(pathname, O_CREAT | O_WRONLY | OTRUNC, mode)`
  - ***注意***: creat的使用已不多见，所以推荐使用open来创建文件。
## 4. 读取文件内容: read()
- read函数，从文件中读取数据。
  - 原型: `ssize_t read(int fd, void *buffer, size_t count); // <unistd.h>`
  - 参数:
    - fd: open()函数返回的合法的文件描述符，或者: STDIN_FILENO/STDOUT_FILENO/STDERR_FILENO;
    - buffer: 读取数据写入的缓冲区，***注意*** 需要用户自行申请，且至少是count字节。
    - count: 缓冲区的长度
  - 返回值: 
    - 失败: 返回-1, 可以通过errno查看错误。
    - 成功: 返回读取字节数，通常小于等于count。当遇到文件结尾EOF返回0。
  - ***注意*** 从终端读取字符的时候，遇见换行符(`\n`)，read调用结束，读取的字符包含换行符，但是没有字符结束符，通常需要用户自行添加字符结束符。
    - 例子:
    ```c
    ssize_t rlen;
    char buf[256] = {0};
    rlen = read(STDIN_FILENO, buf, sizeof(buf) - 1);
    if (rlen == -1) {
      // handler error by errno.
      return;
    }
    buf[rlen] = '\0'; // you must add '\0' to the end.
    //...
    ```
## 5. 数据写入文件: write()
- write函数，写入文件数据。
  - 原型: `ssize_t write(int fd, const void *buffer, size_t count); // <unistd.h>`
  - 参数:
    - fd: open()函数返回的合法的文件描述符，或者: STDIN_FILENO/STDOUT_FILENO/STDERR_FILENO;
    - buffer: 向文件写入数据的缓冲区，且至少是count字节。
    - count: 缓冲区的长度
  - 返回值: 
    - 失败: 返回-1, 可以通过errno查看错误。
    - 成功: 返回写入字节数，通常小于等于count。
      - 部分写的原因: 
        - 对于磁盘文件，通常是磁盘空间不足，或者进程资源对文件大小的限制(RLIMIT_FSIZE)。
      - 对于磁盘文件的写入，***并不能保证正在的写入了磁盘，通常磁盘都会缓存***。
## 6. 关闭文件: close()
- close函数，用于关闭打开的文件描述符。
  - 原型: `int close(int fd); // <unistd.h>`
  - 参数: fd: open()函数返回的合法的文件描述符，或者: STDIN_FILENO/STDOUT_FILENO/STDERR_FILENO;
  - 返回值:
    - 失败: 返回-1，通常关闭一个未打开的文件描述符，或者，关闭两次打开的文件描述符，会返回-1.
    - 成功: 返回0.
  - ***好的建议***: 通常打开的文件操作完毕后关闭，是一个好的编程习惯；否则会消耗掉进程的文件描述符资源。
## 7. 改变文件偏移量: lseek()
- lseek函数，用于修改读或者写的指针。
  - 原型: `off_t lseek(int fd, off_t offset, int whence); // <unistd.h>`
  - 参数: 
    - fd: open()函数返回的合法的文件描述符;
    - offset: 偏移值，相对于参数`whence`的偏移值。
    - whence: 参照点:
      - SEEK_SET: 文件的开头。
      - SEEK_CUR: 当前的位置。
      - SEEK_END: 文件的结尾。
      - ***注意*** 当取值为SEEK_SET的时候，offset值为非负值，其他两者可以取正负值。
  - 返回值: 返回真正文件的偏移量(相对于文件开头的位置的偏移量)。
  - 常用法: 可以通过调用`lseek(fd, 0, SEEK_END)`获取文件的长度。返回值即为文件长度。
- 文件空洞:
  - 当向文件结尾后面的某个位置写入数据时，文件结尾和新写入的位置之间的这段空间称为文件空洞。
  - 读取文件空洞的值将会填充为0，
    - 文件空洞不占用磁盘空间***注意*** 这句话并不准确。测试过在ubuntu18.04上创建空洞文件，但是其文件大小都包含了空洞的大小
    ```sh
    ls -al hole
    du -h ./*
    ```
    - 上述两者相同。***具体原因不明***

## 8. 通用I/O模型以外的操作: ioctl()
- ioctl为执行文件和设备操作提供了一种多用途机制，即可以更加确切的操作文件或设备，或者是通用I/O操作不具备的能力。
  - 原型: `int ioctl(int fd, int request, .../* argp */); // <sys/ioctl.h>`
  - 参数: request是特定的操作命令，后面会根据命令来判断是否存在更多的参数。
    

