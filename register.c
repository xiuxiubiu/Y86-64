#include "register.h"

// 寄存器存储
static unsigned long regstores[MAXREGID + 1];

// 寄存器文件操作
void regfile_operate(struct rfop *op) {

    // 读端口A
    if (op->srcA != RNONE)
        op->valA = regstores[op->srcA];

    // 读端口B
    if (op->srcB != RNONE)
        op->valB = regstores[op->srcB];

    // 写端口M
    if (op->dstM != RNONE)
        regstores[op->dstM] = op->valM;

    // 写端口E
    if (op->dstE != RNONE)
        regstores[op->dstE] = op->valE;
}