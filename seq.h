#ifndef _Y86_64_SEQ_H
#define _Y86_64_SEQ_H

#include "register.h"
#include "instruction.h"
#include "alu.h"
#include "stat.h"
#include "memory.h"

// SEQ执行参数
struct seq_data {

    // 指令数据
    struct instr_data *instr;

    // 寄存器操作数据
    struct rfop *rop;

    // ALU计算数据
    struct alu_data *alu;

    // 访存数据
    struct mem_op mop;

    // 程序状态
    enum STAT st;
};

// 取指阶段
int fetch(struct seq_data *sdptr);

// 译码阶段
int decode(struct seq_data *sdptr);

// 执行阶段
int execute(struct seq_data *sdptr);

// 执行阶段
int memory(struct seq_data *sdptr);

// 更新PC
int updatepc(struct seq_data *sdptr);

#endif