# 内存分配
- 内存分配是一个非常重要的主题，通常可以衍生出来很多有意思的东西。诸如: 
  - glibc的内存申请库使用的是ptmalloc
  - google推出的tcmalloc
  - facebook推出的jemalloc
- 这些开源的库，都可以深入研究一下，写一些文章。
## 1. 在堆上分配内存
- 进程的堆区域从bss(未初始化数据区域)的末尾开始，结束的区域称为`program break`。
  - 结束区域会随着申请内存的增多**向上增长**。通常这个过程是由C库的malloc自动管理的。
- 进程中分配堆区域内存可以采用C库的malloc函数簇。

### 1.1 调整堆的上界(program break): brk()和sbrk()
- brk()函数，直接改变`program break`位置，这个函数需要谨慎使用，当传入的指针地址不在进程地址空间或者低于`end`的位置时，将会出现不可预知的问题。
  - 原型: `int brk(void *addr) // <unistd.h>`
  - 参数: addr: 设定新的`program break`的地址。
    - **注意**: 这个地址的设定有一些限制，在bss的结尾`end`之后，在栈地址之下。如果设定了在`end`之下的位置，访问内存将造成无法预知的行为。通常可能导致内存访问异常。
    - addr的值设定后，通常会根据页面大小对齐到最近的那个页面大小。
    - 可以调整的addr的上限受这些因素影响: RLIMIT_DATA, 内存映射，共享内存段，共享库的影响。
  - 返回值: 成功返回0，失败返回-1，可以通过errno查询错误值，通常是ENOMEM，即内存不足。
- sbrk()函数，增加或减少`program break`的值。
  - 原型: `void* sbrk(intptr_t increment); // <unistd.h>`
  - 参数: increment: 增加或减少的字节数。
    - **注意**: 通常传入0可以返回当前的`program break`的位置。  
    - 该参数可正可负，代表增加和减少。
  - 返回值，成功返回上次的`program break`位置，失败返回`(void *)-1`
  