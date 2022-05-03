#include "stat.h"

// 根据条件码判断指令
unsigned char cnd(IFUN ifun, struct CC *cc) {
    unsigned char code;

    switch (ifun) {

    // 默认功能码
    case FNONE:
        code = 1;
        break;

    // 小于等于
    case FCLE:
        if (cc->ZF | (cc->SF ^ cc->OF))
            code = 1;
        break;
    
    // 小于
    case FCL:
        if (cc->SF ^ cc->OF)
            code = 1;
        break;

    // 等于
    case FCE:
        if (cc->ZF)
            code = 1;
        break;

    // 大于等于
    case FCGE:
        if (~(cc->SF ^ cc->OF))
            code = 1;
        break;

    // 大于
    case FCG:
        if (~(cc->SF ^ cc->OF) & ~cc->ZF)
            code = 1;
        break;

    // 默认code为0
    default: 
        code = 0;
    }

    return code;
}