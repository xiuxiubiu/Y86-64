#include <stdio.h>

// 字长对应的c语言类型
#define WORD_TYPE char

// 位宽支持的有符号类型
typedef WORD_TYPE wt;

// 位宽支持的无符号类型
typedef unsigned WORD_TYPE u_wt;

// 字的字节长度
#define WORD_BYTE_SIZE ((u_wt) sizeof(WORD_TYPE))

// 字长
#define WORD_SIZE ((u_wt) (WORD_BYTE_SIZE << 3));

// 位宽支持类型的无符号最大值
#define UBWT_MAX ((u_wt) -1)

// bit位标志
typedef enum {
    B_ZERO = 0,  // 位值为0
    B_ONE,       // 位值为1 
} bit;

// 指令代码
enum ICODE {

    // halt指令的代码
    IHALT = 0x0, 
    
    // nop指令的代码
    INOP = 0x1, 
    
    // rrmovq指令的代码
    IRRMOVQ = 0x2, 
    
    // irmovq指令的代码
    IIRMOVQ = 0x3, 
    
    // rmovq指令的代码
    IRMMOVQ = 0x4, 

    // mrmovq指令的代码
    IMRMOVQ = 0x5,
    
    // 整数运算指令的代码
    IOPQ = 0x6, 
    
    // 跳转指令的代码
    IJXX = 0x7, 
    
    // call指令的代码
    ICALL = 0x8, 
    
    // ret指令的代码
    IRET = 0x9, 
    
    // pushq指令的代码
    IPUSHQ = 0xA, 
    
    // popq指令的代码
    IPOPQ = 0xB
}

// 默认功能码
const FNONE = 0x0;

// 寄存器
enum REG {

    // %rax寄存器
    RRAX = 0x0, 
    
    // %rcx寄存器
    RRCX = 0x1, 
    
    // %rdx寄存器
    RRDX = 0x2, 
    
    // %rbx寄存器
    RRBX = 0x3, 
    
    // %rsp寄存器
    RRSP = 0x4, 
    
    // %rbp寄存器
    RRBP = 0x5, 
    
    // %rsi寄存器
    RRSI = 0x6, 
    
    // %rdi寄存器
    RRDI = 0x7, 
    
    // %r8寄存器
    RR8 = 0x8, 
    
    // %9寄存器
    RR9 = 0x9, 
    
    // %r10寄存器
    RR10 = 0xA, 
    
    // %r11寄存器
    RR11 = 0xB, 
    
    // %r12寄存器
    RR12 = 0xC, 
    
    // %r13寄存器
    RR13 = 0xD, 
    
    // %r14寄存器
    RR14 = 0xE, 
    
    // 表明没有任何寄存器文件访问
    RNONE = 0xF
};

// 加法运算的功能
const unsigned char ALUADD = 0x0;

// 程序状态
enum STAT {

    // 1 正常操作状态码
    SAOK = 0x1,

    // 2 地址异常状态码
    SADR = 0x2,

    // 3 非法指令异常状态码 
    SINS = 0x3,

    // 4 halt状态码
    SHLT = 0x4,
};

// 随机内存存储
// 使用unsigned类型防止算数右移的影响
unsigned char DMEM[UBWT_MAX];

// 程序计数器
u_wt PC = 0x0;

// 指令合法标志位
unsigned char instr_valid = 1;

// 指令包含常数标志位
unsigned char need_regids = 1 << 1;

// 指令包含寄存器标志位
unsigned char need_valC = 1 << 2;

// 指令相关数据
struct instr_data {
    unsigned char icode;        // 指令代码
    unsigned char ifun;         // 指令方法
    bit instr_valid;            // 指令合法标志位
    bit need_regids;            // 指令包含常数标志位
    bit need_valC;              // 指令包含寄存器标志位
    unsigned char signal;       // 根据icode计算出来的信号
    unsigned char rA;           // 操作寄存器rA
    unsigned char rB;           // 操作寄存器rB
    u_wt valC;                  // 指令包含的常数
    u_wt valP;                  // 下一条指令的地址（即将放进PC寄存器的指令地址）   
};

// 寄存器文件
u_wt register_file[0x10];

// 取指阶段
struct instr_data fetch();

// 内存地址读取字节
unsigned char read_byte_memory(u_wt addr);

// 内存地址读取字长的常数
u_wt read_word_memory(u_wt addr);

// 寄存器文件读端口A
u_wt regfile_read_A(enum REG srcA);

// 寄存器文件读端口B
u_wt regfile_read_B(enum REG srcB);

// 寄存器文件写端口E
void regfile_write_E(enum REG dstE, u_wt valE);

// 寄存器文件写端口M
void regfile_write_M(enum REG dstM, u_wt valM);

int main() {

    PC = 0x0;

    DMEM[0] = 0x20;
    DMEM[1] = 0x45;

    struct instr_data data = fetch();

    printf(
        "icode:%d; ifun:\%d; signal: %d; rA: %d; rB: %d; valP: %d\n",
        data.icode, data.ifun, data.signal, data.rA, data.rB, data.valP
    );


    return 0;
}

// 内存地址读取字节
unsigned char read_byte_memory(u_wt addr) {
    return DMEM[addr];
}

// 取指阶段
struct instr_data fetch() {

    // 返回的指令信息
    struct instr_data data = {
        .icode = 0x0,
        .ifun = FNONE,
        .instr_valid = B_ZERO,
        .need_regids = B_ZERO,
        .need_valC = B_ZERO,
        .rA = RNONE,
        .rB = RNONE,
        .valC = 0x0,
        .valP = PC + 1,
    };

    // PC作为指令的第一个字节的地址
    u_wt split = DMEM[PC];

    // 指令代码
    data.icode = split >> 4 & 0x0F;

    // 指令合法
    data.instr_valid = B_ONE;

    // 是否需要寄存器
    if (data.icode == IRRMOVQ || data.icode == IOPQ || data.icode == IPUSHQ || data.icode == IPOPQ || 
    data.icode == IIRMOVQ || data.icode == IRMMOVQ || data.icode == IMRMOVQ)
        data.need_regids = B_ONE;

    // 是否有常数
    if (data.icode ==  IIRMOVQ || data.icode == IRMMOVQ || data.icode == IMRMOVQ || 
    data.icode == IJXX || data.icode ==ICALL)
        data.need_valC = B_ONE;

    // 指令方法
    data.ifun =  split & 0x0F;

    // 当计算出need_regids为1时，第二个字节分开分别放入rA和rB
    if (data.signal & need_regids) {
        
        // 读取指令第二个字节
        unsigned char regbyte = read_byte_memory(PC + 1);

        // 分别读取rA和rB寄存器标识
        data.rA = (regbyte & 0xF0) >> 4;
        data.rB = regbyte & 0x0F;
    }

    // 读取常数valC
    if (data.signal & need_valC) {

        // 有寄存器则读取第三个字节
        if (data.signal & need_regids) {
            data.valC = read_byte_memory(PC + 2);
        } else {
            
            // 没有寄存器则读取第二个字节
            data.valC = read_byte_memory(PC + 1);
        }
    }

    // 下一条指令的地址
    // 对于PC值p、need_regids值r、need_calC值i、常数长度为w字节
    // 产生的valP信号为：p + 1 + r + w*i
    data.valP = PC + 1 + data.need_regids + data.need_valC * WORD_BYTE_SIZE;

    return data;
}






/**
 * ----------------------------------------------------------------
 */

// // 寄存器内容
// long regstores[0xF];

// // 指令
// enum INSTRUCTIONS {
//     HALT = 0x00, NOP = 0x10,
//     RRMOVEQ = 0x20, CMOVLE = 0x21, CMOVL = 0x22, CMOVE = 0x23, CMOVNE = 0x24, CMOVGE = 0x25, CMOVG = 0x26,
//     IRMOVQ = 0x30, RMMOVQ = 0x40, MRMOVQ = 0x50,
//     JMP = 0x70, JLE = 0x71, JL = 0x72, JE = 0x73, JNE = 0x74, JGE = 0x75, JG = 0x76,
//     CALL = 0x80, RET = 0x90, PUSHQ = 0xA0, POPQ = 0xB0
// };

// enum OPQ {
//     ADDQ = 0x60, SUBQ = 0x61, ANDQ = 0x62, XORQ = 0x63
// };

// // 条件码
// struct CC {
//     char ZF;
//     char SF;
//     char OF;
// };

// // 程序计数器
// unsigned char PC;

// // 内存
// unsigned char DMEM[UCHAR_MAX];

// // 程序状态
// char STAT;

// // 处理完整指令
// void sequential();

// // arithmetic logic unit
// long alu(enum OPQ op, long y, long x);

// // 小端序long类型
// long little_endian_long(unsigned char address);

// // 小端序保存到内存®
// long little_endian_save_memory(unsigned char address, long l);

// int main() {

//     // 初始化寄存器内容
//     for (int i  = 0; i < 0xF; i++)
//         regstores[i] = 0;

//     // 初始化内存
//     for (int i = 0; i < UCHAR_MAX; i++)
//         DMEM[i] = 0;

//     PC = 0;
//     DMEM[PC] = PUSHQ;
//     // 0011 0010
//     // 50   2 + 16 + 32 = 50
//     DMEM[PC + 1] = 0x3F;

//     // 0011 0010 0001
//     // 3 * 256 + 2 * 16 + 1
//     // 768 + 32 + 1 = 801
//     regstores[0x3] = 0x321;

//     regstores[RSP] = 0xFE;

//     // regstores[0x2] = 0x10;
//     // DMEM[PC + 2] = 0x10;

//     // 0x 01 11
//     // 256 16 1
//     // 273
//     // DMEM[PC + 32] = 0x11;
//     // DMEM[PC + 33] = 0x01;

//     sequential();


//     // printf("RMMOVQ: %ld\n", little_endian_long(0x10 + 0x10));
//     // printf("rB: %ld\n", regstores[0x3]);

//     printf("RSP: %ld; stack: %ld, PC: %d\n", regstores[RSP], little_endian_long(regstores[RSP]), PC);

//     DMEM[PC] = POPQ;
//     DMEM[PC + 1] = 0x2F;

//     sequential();

//     printf("RSP: %ld; 0x2 reg: %ld\n", regstores[RSP], regstores[0x2]);


//     return 0;
// }

// // 处理完整指令
// void sequential() {

//     char rA = NOREG, rB = NOREG;
//     long valA, valB, valC, valE, valM;
//     unsigned char valP;

//     // 获取指令
//     long intruction = (long)DMEM[PC];

//     // 处理指令
//     switch (intruction) {
//     case HALT:
//         break;

//     case NOP:
//         break;

//     case RRMOVEQ:

//         // fetch
//         rA = (DMEM[PC + 1] & 0xF0) >> 4;
//         rB = DMEM[PC + 1] & 0xF;
//         valP = PC + 2;

//         // decode
//         valA = regstores[rA];

//         // execute
//         valE = alu(ADDQ, 0, valA);

//         // memory

//         // write back
//         regstores[rB] = valE;

//         // PC update
//         PC = valP;

//         break;

//     case CMOVLE:
//     case CMOVL:
//     case CMOVE:
//     case CMOVNE:
//     case CMOVGE:
//     case CMOVG:
//         break;

//     case IRMOVQ:

//         // fetch
//         rB = DMEM[PC + 1] & 0x0F;
//         valC = little_endian_long(PC + 2);
//         valP = PC + 10;

//         // decode

//         // execute
//         valE = 0 + valC;

//         // write back
//         regstores[rB] = valE;

//         // PC update
//         PC = valP;

//         break;
    
//     case RMMOVQ:

//         // fetch
//         rA = (DMEM[PC + 1] & 0xF0) >> 4;
//         rB = DMEM[PC + 1] & 0xF;
//         valC = little_endian_long(PC + 2);
//         valP = PC + 10;

//         // decode
//         valA = regstores[rA];
//         valB = regstores[rB];

//         // execute
//         valE = alu(ADDQ, valC, valB);

//         // memory
//         little_endian_save_memory((unsigned char)valE, valA);

//         // PC update
//         PC = valP;

//         break;

//     case MRMOVQ:

//         // fetch
//         rA = (DMEM[PC + 1] & 0xF0) >> 4;
//         rB = DMEM[PC + 1] & 0xF;
//         valC = little_endian_long(PC + 2);
//         valP = PC + 10;

//         // decode
//         valB = regstores[rB];

//         // execute
//         valE = alu(ADDQ, valC, valB);

//         // memory
//         valE = little_endian_long((unsigned char)valE);

//         // write back
//         regstores[rA] = valE;

//         // PC update
//         PC = valP;

//         break;

//     case ADDQ:
//     case SUBQ:
//     case ANDQ:
//     case XORQ:

//         // fetch
//         rA = (DMEM[PC + 1] & 0xF0) >> 4;
//         rB = DMEM[PC + 1] & 0xF;
//         valP = PC + 2;

//         // decode
//         valA = regstores[rA];
//         valB = regstores[rB];

//         // excute
//         valE = alu(intruction, valA, valB);

//         // memory

//         // write back
//         regstores[rB] = valE;

//         // PC update
//         PC = valP;

//         break;

//     case JMP:
//     case JLE:
//     case JL:
//     case JE:
//     case JNE:
//     case JGE:
//     case JG:




//         break;

//     case CALL:
//         break;

//     case RET:
//         break;

//     // pushq rA
//     case PUSHQ:

//         // fetch
//         rA = (DMEM[PC + 1] & 0xF0) >> 4;
//         valP = PC + 2;

//         // decode
//         valA = regstores[rA];
//         valB = regstores[RSP];

//         // execute
//         valE = alu(ADDQ, -8, valB);

//         // memory
//         little_endian_save_memory((unsigned char)valE, valA);

//         // write back
//         regstores[RSP] = valE;

//         // PC update
//         PC = valP;

//         break;

//     case POPQ:

//         // fetch
//         rA = (DMEM[PC + 1] & 0xF0) >> 4;
//         valP = PC + 2;

//         // decode
//         valA = regstores[RSP];
//         valB = regstores[RSP];

//         // execuate
//         valE = alu(ADDQ, 8, valB);

//         // memory
//         valM = little_endian_long(valA);

//         // write back
//         regstores[RSP] = valE;
//         regstores[rA] = valM;

//         // PC update
//         PC = valP;

//         break;
    
//     default:
//         break;
//     }
// }

// // arithmetic logic unit
// long alu(enum OPQ op, long y, long x) {
//     long valE;
//     switch (op) {
//     case ADDQ:
//         valE = x + y;
//         break;
//     case SUBQ:
//         valE = x - y;
//         break;
//     case ANDQ:
//         valE = x & y;
//         break;
//     case XORQ:
//         valE = x ^ y;
//         break;
//     }
//     return valE;
// }

// // 小端序转long
// long little_endian_long(unsigned char addres) {
//     long l = 0x0;
//     for (int i = 0; i < 8; i++)
//         l |= DMEM[addres + i] << (8 * i);
//     return l;
// }

// // 小端序保存到内存
// long little_endian_save_memory(unsigned char address, long l) {
//     for (int i = 0; i < 8; i++) {
//         DMEM[address + i] ^= DMEM[address + i];
//         DMEM[address + i] = DMEM[address + i] | (l >> (8 * i));
//     }
// }