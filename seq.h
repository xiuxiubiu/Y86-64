#ifndef _Y86_64_SEQ_H
#define _Y86_64_SEQ_H

#include "register.h"

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

    // 指令代码
    unsigned char icode;  

    // 指令方法          
    unsigned char ifun; 

    // 指令合法标志位            
    unsigned char instr_valid : 1;  

    // 指令包含常数标志位
    unsigned char need_regids : 1; 

    // 指令包含寄存器标志位
    unsigned char need_valC :1; 

    // 操作寄存器rA    
    unsigned char rA;   

    // 指令执行产生的valA值            
    long valA; 

    // 操作寄存器rB
    unsigned char rB;   

    // 指令执行产生的valA值             
    long valB;

    // 指令包含的常数
    long valC;            

    // 下一条指令的地址（即将放进PC寄存器的指令地址）
    unsigned char *valP;            
};

// SEQ执行参数
struct seq_data {

    // 指令数据
    struct instr_data *instr;

    // 寄存器操作数据
    struct rfop *op;

    // ALU产生的数据
    struct alu_data *alu;
};

// 取指阶段
int fetch(struct seq_data *sdptr);

// 译码阶段
int decode(struct seq_data *sdptr);

#endif