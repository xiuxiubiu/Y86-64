#include "seq.h"
#include "register.h"

#include <stdio.h>
#include <stdlib.h>

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

int main() {

    unsigned char *pc = malloc(sizeof(char) * 2);
    regfile_write_E(PC, (unsigned long)pc);

    *pc = 0x20;
    *(pc+1) = 0x45;

    struct instr_data data = {
        .icode = 0x0,
        .ifun = FNONE,
        .instr_valid = 0,
        .need_regids = 0,
        .need_valC = 0,
        .rA = RNONE,
        .rB = RNONE,
        .valC = 0x0,
        .valP = 0x0,
    };

    int errcode;
    if ((errcode = fetch(&data)) != 0)
        printf("error: fetch error [%d]\n", errcode);

    printf(
        "icode:%d; ifun:%d; instr_valid: %d; need_regids: %d; need_valC: %d; rA: %d; rB: %d; valP: %p\n",
        data.icode, data.ifun, data.instr_valid, data.need_regids, data.need_valC, data.rA, data.rB, data.valP
    );


    return 0;
}