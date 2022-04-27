#include "register.h"

// 寄存器存储
static unsigned long regstores[MAXREGID + 1];

// 寄存器文件读端口A
int regfile_read_A(unsigned char regid, unsigned long *valA) {
    if (regid > MAXREGID)
        return -1;
    *valA = regstores[regid];
    return 0;
}

// 寄存器文件读端口B
int regfile_read_B(unsigned char regid, unsigned long *valB) {
    if (regid > MAXREGID)
        return -1;
    *valB = regstores[regid];
    return 0;
}

// 寄存器文件写端口E
int regfile_write_E(unsigned char regid, unsigned long valE) {
    if (regid > MAXREGID)
        return -1;
    regstores[regid] = valE;
    return 0;
}

// 寄存器文件写端口M
void regfile_write_M(unsigned char);

