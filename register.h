#ifndef _Y86_64_REGISTER_H  
#define _Y86_64_REGISTER_H  

#define RRAX  0x0    // %rax寄存器  
#define RRCX  0x1    // %rcx寄存器  
#define RRDX  0x2    // %rdx寄存器  
#define RRBX  0x3    // %rbx寄存器  
#define RRSP  0x4    // %rsp寄存器  
#define RRBP  0x5    // %rbp寄存器  
#define RRSI  0x6    // %rsi寄存器  
#define RRDI  0x7    // %rdi寄存器  
#define RR8   0x8    // %r8寄存器  
#define RR9   0x9    // %r9寄存器  
#define RR10  0xA    // %r10寄存器  
#define RR11  0xB    // %r11寄存器  
#define RR12  0xC    // %r12寄存器  
#define RR13  0xD    // %r13寄存器  
#define RR14  0xE    // %r14寄存器  
#define RNONE 0xF    // 表明没有任何寄存器文件访问

#define PC 0x10      // PC寄存器

#define MAXREGID PC  // PC寄存器ID是最大的寄存器ID

// 寄存器文件读端口A
int regfile_read_A(unsigned char regid, unsigned long *valA);

// 寄存器文件读端口B
int regfile_read_B(unsigned char regid, unsigned long *valB);

// 寄存器文件写端口E
int regfile_write_E(unsigned char regid, unsigned long valE);

// 寄存器文件写端口M
void regfile_write_M(unsigned char);

#endif  