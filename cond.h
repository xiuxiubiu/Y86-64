#ifndef _Y86_64_CND_H
#define _Y86_64_CND_H

#include "instruction.h"
#include "stat.h"

// 根据指令功能码和条件码设置Cnd信号
char cnd(IFUN ifun, struct CC *cc);

#endif