#ifndef _Y86_64_MEMORY_H
#define _Y86_64_MEMORY_H

// 内存读写标志
typedef enum {
    MEM_READ = 0,
    MEM_WRITE = 1,
} MEMWR;

// 内存操作数据
struct mem_op {
    MEMWR wr;                       // 读写标志
    unsigned long addr;            // 操作内存地址
    unsigned long data;             // 内存写入数据
    unsigned long valM;             // 内存数据出口
    unsigned char dmem_error : 1;   // 内存地址是否错误
};

// 内存操作方法
int mem_operate(struct mem_op *op);

#endif