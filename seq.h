#ifndef _Y86_64_SEQ_H
#define _Y86_64_SEQ_H

#define IHALT   0x0     // halt指令的代码 
#define INOP    0x1     // nop指令的代码
#define IRRMOVQ 0x2     // rrmovq指令的代码
#define IIRMOVQ 0x3     // irmovq指令的代码
#define IRMMOVQ 0x4     // rmovq指令的代码
#define IMRMOVQ 0x5     // mrmovq指令的代码
#define IOPQ    0x6     // 整数运算指令的代码
#define IJXX    0x7     // 跳转指令的代码
#define ICALL   0x8     // call指令的代码
#define IRET    0x9     // ret指令的代码
#define IPUSHQ  0xA     // pushq指令的代码
#define IPOPQ   0xB     // popq指令的代码

#define FNONE   0x0     // 默认功能码

// 指令相关数据
struct instr_data {
    unsigned char icode;            // 指令代码
    unsigned char ifun;             // 指令方法
    unsigned char instr_valid : 1;  // 指令合法标志位
    unsigned char need_regids : 1;  // 指令包含常数标志位
    unsigned char need_valC :1;     // 指令包含寄存器标志位s
    unsigned char rA;               // 操作寄存器rA
    unsigned char rB;               // 操作寄存器rB
    unsigned long valC;             // 指令包含的常数
    unsigned char *valP;            // 下一条指令的地址（即将放进PC寄存器的指令地址）
};

// 取指阶段
int fetch(struct instr_data *idptr);

#endif