#include "seq.h"

// 取指阶段
int fetch(struct seq_data *sdptr) {

    // 指令数据
    struct instr_data *idptr = sdptr->instr;

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
int decode(struct seq_data *sdptr) {
    
    // 指令信息
    struct instr_data *idptr = sdptr->instr;

    // 指令信息
    struct rfop *op = sdptr->op; 
    op->srcA = RNONE;
    op->srcB = RNONE;
    op->dstE = RNONE;
    op->dstM = RNONE;

    // 根据命令判断读端口A的源寄存器
    switch (idptr->icode) {

    // 需要读取命令中rA所表示的寄存器
    case IRRMOVQ:
    case IRMMOVQ: 
    case IOPQ:
    case IPUSHQ:
        op->srcA = idptr->rA;
        break;

    // 需要读取RRSP寄存器
    case IPOPQ:
    case IRET:
        op->srcA = RRSP;
        break;

    // 其余命令不需要寄存器
    default:
        op->srcA = RNONE;
    }

    // 根据命令判断读端口B的源寄存器
    switch (idptr->icode) {
    
    // 需要读取命令中rB所表示的寄存器
    case IRMMOVQ:
    case IMRMOVQ:
    case IOPQ:
        op->srcB = idptr->rB;
        break;

    // 需要读取RRSP寄存器
    case ICALL:
    case IRET:
    case IPUSHQ:
    case IPOPQ:
        op->srcB = RRSP;
        break;

    // 其余命令不需要寄存器    
    default:
        op->srcB = RNONE;    
    }

    // 从寄存器读数据
    regfile_operate(op);

    // 根据命令判断写端口E的目的寄存器
    switch (idptr->icode) {

    // 需要写rB寄存器
    case IRRMOVQ:
    case IIRMOVQ:
    case IOPQ:
        op->dstE = idptr->rB;

    // 需要写RRSP寄存器
    case ICALL:
    case IPUSHQ:
    case IPOPQ:
        op->dstE = RRSP;

    // 不需要写寄存器    
    default:
        op->dstE = RNONE;
    }

    // 根据命令判断写端口E的目的寄存器
    switch (idptr->icode) {

    // 需要写rB寄存器
    case IRRMOVQ:
    case IIRMOVQ:
    case IOPQ:
        op->dstE = idptr->rB;

    // 需要写RRSP寄存器
    case ICALL:
    case IPUSHQ:
    case IPOPQ:
        op->dstE = RRSP;

    // 不需要写寄存器    
    default:
        op->dstE = RNONE;
    }

    // 根据命令判断写端口M的目的寄存器
    switch(idptr->icode) {

    // 内存数据写rA寄存器
    case IMRMOVQ:
    case IPOPQ:
        op->dstM = idptr->rA;
    
    // 不需要写寄存器
    default:
        op->dstM = RNONE;
    }

    return 0;
}

// 执行阶段
int execute(struct seq_data *sdptr){

    IFUN ifun;
    long aluA, aluB;

    // 根据指令判断aluA的值
    switch(sdptr->instr->icode) {

    // aluA为valA的指令
    case IRRMOVQ:
    case IOPQ:
        aluA = sdptr->op->valA;

    // aluA为valC的指令
    case IIRMOVQ:
    case IRMMOVQ:
    case IMRMOVQ:
        aluA = sdptr->instr->valC;

    // aluA为-8的指令
    case ICALL:
    case IPUSHQ:
        aluA = -8;
    
    // aluA为+8的指令
    case IRET:
    case IPOPQ:
        aluA = 8;

    // 其余指令不需要ALU计算
    default:
        return 0;
    }

    // 根据指令判断aluB的值
    switch (sdptr->instr->icode) {

    // aluB为valB的指令 
    case IRMMOVQ:
    case IMRMOVQ:
    case IOPQ:
    case ICALL:
    case IRET:
    case IPUSHQ:
    case IPOPQ:
        aluB = sdptr->op->valB;

    // aluB为0的指令
    case IRRMOVQ:
    case IIRMOVQ:
        aluB = 0;
    
    // 其余指令不需要ALU
    default:
        return 0;
    }

    // 计算valE
    calculate(sdptr->alu);

    // 整数操作设置条件码
    if (sdptr->instr->icode == IOPQ)
        sdptr->cc = sdptr->alu->cc;

    return 0;
}