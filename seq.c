#include "seq.h"
#include "register.h"

#include <stdio.h>

// 取指阶段
int fetch(struct instr_data *idptr) {

    // 获取PC寄存器内的指令地址
    unsigned char *pc;
    struct rfop readpc = {
        .srcA = PC,
        .srcB = RNONE,
        .dstE = RNONE,
        .dstM = RNONE,
    };
    regfile_operate(&readpc);
    pc = (unsigned char *)readpc.valA;

    // PC作为指令的第一个字节的地址
    unsigned char split = *pc;

    // 指令代码
    idptr->icode = split >> 4 & 0x0F;

    // 指令合法
    idptr->instr_valid = 1;

    // 是否需要寄存器
    if (idptr->icode == IRRMOVQ || idptr->icode == IOPQ || idptr->icode == IPUSHQ || idptr->icode == IPOPQ || 
    idptr->icode == IIRMOVQ || idptr->icode == IRMMOVQ || idptr->icode == IMRMOVQ)
        idptr->need_regids = 1;

    // 是否有常数
    if (idptr->icode ==  IIRMOVQ || idptr->icode == IRMMOVQ || idptr->icode == IMRMOVQ || 
    idptr->icode == IJXX || idptr->icode ==ICALL)
        idptr->need_valC = 1;

    // 指令方法
    idptr->ifun =  split & 0x0F;

    // 当计算出need_regids为1时，第二个字节分开分别放入rA和rB
    if (idptr->need_regids) {
        
        // 读取指令第二个字节
        unsigned char regbyte = *((unsigned char *)(pc + 1));

        // 分别读取rA和rB寄存器标识
        idptr->rA = (regbyte & 0xF0) >> 4;
        idptr->rB = regbyte & 0x0F;
    }

    // 读取常数valC
    if (idptr->need_valC) {

        // 有寄存器则读取第三个字节
        if (idptr->need_regids) {
            idptr->valC = *((unsigned long *)(pc + 2));
        } else {
            
            // 没有寄存器则读取第二个字节
            idptr->valC = *((unsigned long *)(pc + 1));
        }
    }

    // 下一条指令的地址
    // 对于PC值p、need_regids值r、need_calC值i、常数长度为w字节
    // 产生的valP信号为：p + 1 + r + w*i
    idptr->valP = pc + 1 + idptr->need_regids + idptr->need_valC * sizeof(unsigned long);

    return 0;
}

// 译码阶段
int decode(struct instr_data *idptr) {
    return 0;
}