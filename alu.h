#ifndef _Y86_64_ALU_H
#define _Y86_64_ALU_H

// ALU输入参数
struct alu_input {

    // 具体操作
    unsigned char ifun;

    // 第一个输入数字
    long aluA;

    // 第二个输入数字
    // subq为valB - valA
    // 即subq valA, valB = valB - valA
    long aluB;
};

// ALU计算产生的数据
struct alu_data {

    // 输出数据
    long valE;

    // CC条件码
    unsigned char OF : 1;
    unsigned char ZF : 1;
    unsigned char SF : 1;
};

// alu执行操作
int calculate(struct seq_data *sdptr);

#endif