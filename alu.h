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

// addq运算
static long addq(long aluA, long aluB, struct CC *cc);

// subq运算
static long subq(long aluA, long aluB, struct CC *cc);

// andq运算
static long andq(long aluA, long aluB, struct CC *cc);

// xorq运算
static long xorq(long aluA, long aluB, struct CC *cc);

#endif