#include "seq.h"
#include "register.h"

#include <stdio.h>
#include <stdlib.h>

// 上一次指令执行的数据
static struct seq_data *old_seq_data;

int main() {

    unsigned char *pc = malloc(sizeof(char) * 2);
    *pc = 0x20;
    *(pc+1) = 0x45;

    struct rfop op = { 
        .srcA = RNONE,
        .srcB = RNONE,
        .dstE = PC,
        .valE = (unsigned long)pc,
        .dstM = RNONE,
    };
    regfile_operate(&op);


    struct rfop rsp_op = {
        .srcA = RNONE,
        .srcB = RNONE,
        .dstE = RRSP,
        .valE = (unsigned long)2222,
        .dstM = RNONE,
    };
    regfile_operate(&rsp_op);


    struct seq_data *sdptr = malloc(sizeof(struct seq_data));

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
    sdptr->instr = &data;

    sdptr->alu = malloc(sizeof(struct alu_data));
    sdptr->alu->cc = malloc(sizeof(struct CC));

    int errcode;
    if ((errcode = fetch(sdptr)) != 0)
        printf("error: fetch error [%d]\n", errcode);

    printf(
        "icode:%d; ifun:%d; instr_valid: %d; need_regids: %d; need_valC: %d; rA: %d; rB: %d; valP: %p\n",
        data.icode, data.ifun, data.instr_valid, data.need_regids, data.need_valC, data.rA, data.rB, data.valP
    );


    sdptr->rop = malloc(sizeof(struct rfop));
    decode(sdptr);

    return 0;
}