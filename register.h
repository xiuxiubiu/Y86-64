#ifndef _Y86_64_REGISTER_H  
#define _Y86_64_REGISTER_H  

// 寄存器ID
typedef enum {
    RRAX  = 0x0,    // %rax寄存器  
    RRCX  = 0x1,    // %rcx寄存器  
    RRDX  = 0x2,    // %rdx寄存器  
    RRBX  = 0x3,    // %rbx寄存器  
    RRSP  = 0x4,    // %rsp寄存器  
    RRBP  = 0x5,    // %rbp寄存器  
    RRSI  = 0x6,    // %rsi寄存器  
    RRDI  = 0x7,    // %rdi寄存器  
    RR8   = 0x8,    // %r8寄存器  
    RR9   = 0x9,    // %r9寄存器  
    RR10  = 0xA,    // %r10寄存器  
    RR11  = 0xB,    // %r11寄存器  
    RR12  = 0xC,    // %r12寄存器  
    RR13  = 0xD,    // %r13寄存器  
    RR14  = 0xE,    // %r14寄存器  
    RNONE = 0xF,    // 表明没有任何寄存器文件访问
    PC    = 0x10    // PC寄存器
} REGID;

#define MAXREGID PC  // PC寄存器ID是最大的寄存器ID

// 寄存器文件操作
struct rfop {
    REGID srcA;
    unsigned long valA;
    REGID srcB;
    unsigned long valB;
    REGID dstE;
    unsigned long valE;
    REGID dstM;
    unsigned long valM;
};

// 寄存器文件操作
void regfile_operate(struct rfop *op);

#endif  