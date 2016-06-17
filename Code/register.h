#ifndef __REGISTER_H
#define __REGISTER_H

#include "common.h"

typedef enum _register{
    ZERO,
    AT,
    V0, V1,
    A0, A1, A2, A3,
    T0, T1, T2, T3, T4, T5, T6, T7,
    S0, S1, S2, S3, S4, S5, S6, S7,
    T8, T9,
    K0, k1,
    GP,
    SP,
    S8,
    RA
}Register;

extern char registerStr[][10];
void getRegister(FILE* fileOut, InterCode* ic, Type* currentFunc);
int getOffset(Operand* o, Type* currentFunc);





#endif