#ifndef _Y86_64_STAT_H
#define _Y86_64_STAT_H

#include "instruction.h"

// Conditionn Code
struct CC {
    unsigned char ZF : 1;
    unsigned char SF : 1;
    unsigned char OF : 1;
};

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

// 根据条件码判断指令
unsigned char cnd(IFUN ifun, struct CC *cc);

#endif