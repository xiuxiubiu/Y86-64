#ifndef _Y86_64_ALU_H
#define _Y86_64_ALU_H

#include "instruction.h"
#include "stat.h"

// alu操作数据
struct alu_data {
    IFUN ifun;
    long aluA;
    long aluB;
    long valE;
    struct CC *cc;
};

// alu执行操作
int calculate(struct alu_data *adptr);

#endif