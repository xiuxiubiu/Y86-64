#include "alu.h"

// addq运算
static long addq(long aluA, long aluB, struct CC *cc);

// subq运算
static long subq(long aluA, long aluB, struct CC *cc);

// andq运算
static long andq(long aluA, long aluB, struct CC *cc);

// xorq运算
static long xorq(long aluA, long aluB, struct CC *cc);

// alu执行操作
int calculate(struct alu_data *adptr){

    // alu操作方法
    long (*alufun)(long, long, struct CC *);

    // 初始溢出状态
    adptr->cc->OF = 0;
    adptr->cc->OF = 0;
    adptr->cc->OF = 0;

    // 根据操作码判断对应的操作
    switch (adptr->ifun) {
    case FADDQ:
        alufun = &addq;
        break;
    case FSUBQ:
        alufun = &subq;
        break;
    case FANDQ:
        alufun = &andq;
        break;
    case FXORQ:
        alufun = &xorq;
        break;
    default:
        return 0;
    }

    // 调用对应的整数操作函数
    adptr->valE = (*alufun)(adptr->aluA, adptr->aluB, adptr->cc);

    // 判断是否为0
    if (adptr->valE == 0)
        adptr->cc->ZF = 1;

    // 判断是否负数
    if (adptr->valE < 0)
        adptr->cc->SF = 1;       

    return 0;
}

// 加法运算
static long addq(long aluA, long aluB, struct CC *cc) {
    long valE = aluB + aluA;

    // 判断正数加法是否溢出
    if (aluA > 0 && aluB > 0 && valE <= 0)
        cc->OF = 1;

    // 判断负数加法是否溢出
    if (aluA < 0 && aluB < 0 && valE >= 0)
        cc->OF = 1;

    return valE;
}

// 减法运算
static long subq(long aluA, long aluB, struct CC *cc) {
    long valE = aluB - aluA;

    // 判断正数减负数
    if (aluB > 0 && aluA < 0 && valE <= 0)
        cc->OF = 1;

    // 判断负数减正数
    if (aluB < 0 && aluA > 0 && valE >= 0)
        cc->OF = 1;
    
    return aluB - aluA;
}

// 按位与运算
static long andq(long aluA, long aluB, struct CC *cc) {
    return aluB & aluA;
}

// 异或运算
static long xorq(long aluA, long aluB, struct CC *cc) {
    return aluB ^ aluA;
}