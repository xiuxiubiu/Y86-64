#include "memory.h"
#include <stdio.h>

// 内存操作方法
int mem_operate(struct mem_op *op) {

    if (op->addr == 0) {
        op->dmem_error = 1;
        return 1;
    }

    if (op->wr == MEM_WRITE)
        *(long *)op->addr = op->data;

    if (op->wr == MEM_READ)
        op->valM = *(long *)op->addr;
    
    return 0;
}