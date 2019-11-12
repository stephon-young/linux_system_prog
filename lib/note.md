## 代码说明
- 结构:
  文件| 描述
  - | :-: |
  error.c  | 错误处理
  parser.c | 解析输入数字

### 错误处理
- 文件`error.c`，查看头文件定义:
  ```c
  void errMsg(const char *format, ...);
  void errExit(const char *format, ...) NORETURN;
  void err_exit(const char *format, ...) NORETURN;
  void errExitEN(int errnum, const char *format, ...) NORETURN;
  void fatal(const char *format, ...) NORETURN;
  void usageErr(const char *format, ...) NORETURN;
  void cmdlineErr(const char *format, ...) NORETURN;
  ```
- 注意区别`errExit/err_exit/errExitEN`三个函数
  - 三个函数都检查环境变量`EF_DUMPCORE`，如果存在且不为空，则执行`abort()`去转储输出? // todo:: 需要编程验证，是否会转储输出。
  - `errExit` 打印全局变量errno，并调用`exit()`退出。
  - `err_exit` 打印全局变量errno，并调用`_exit()`退出。
  - `errExitEN` 打印参数`errnum`，并调用`exit()`退出。
- 系统调用`abort/exit/_exit`的区别(可以用man `xxx`来查看帮助)。
  - `abort()`，调用后:
    - 将触发SIGABRT信号，
      - 如果有对应的信号处理器(handler)，则进行如下处理:
        - 进入信号处理器处理，直到处理完成，如果完成则终止程序。
      - 否则，即无信号处理器(被忽略)，直接终止程序。
  - `exit()`，调用后:
    - 首先: 所有注册的atexit()/on_exit()被调用。按照他们注册顺序的倒序。如果注册函数并不返回（如调用了_exit()函数）则，后续的调用链全部取消，同时，下面的步骤也不再执行。
    - 然后: 所有打开的stdio流，将被刷新，并关闭
  - `_exit()`，调用后:
    - 所有注册的atexit()/on_exit()***不被调用***。
		- 所有打开的stdio流，***不被刷新，也不关闭***。
		- 通常，在所有的子进程退出的时候，会用到_exit()来防止调用exit退出的处理器，所有的stdio流不被刷新，也不关闭。
- `fatal`函数，不打印errno，直接调用`exit()`退出。
- `usageErr`函数，打印usage，然后刷新缓冲，并调用`exit()`退出。
- `cmdlineErr`函数，打印cmdline，然后刷新缓冲，并调用`exit()`退出。

### 解析数字
- 文件`parser.c`，查看头文件定义:
  ```c
  long getLong(const char *arg, int flags, const char *name);
  int getInt(const char *arg, int flags, const char *name);
  ```
- 两者解析参数，并返回数据，如果解析出错，则打印错误，并调用`exit()`终止程序。

  