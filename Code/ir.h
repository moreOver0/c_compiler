#ifndef __IR_H
#define __IR_H

#include "common.h"


typedef struct Operand{
    OperandTag tag;
    /*     function,
    variable, constant, 
    tempVariable, label,
    address
    */
    union{
        Type* func;
        Type* var;
        int value; // for constant
        struct Operand* pointTo; // for address
        int no;// for temp variable and label
    };
}Operand;


typedef struct InterCode{
    InterCodeTag tag;

    Operand* target;
    Operand* o1;
    Operand* o2;
    char* relop;

}InterCode;

typedef struct InterCodeNode{
    InterCode* ic;
    struct InterCodeNode* prev;
    struct InterCodeNode* next;
}InterCodeNode;

Operand* newTemp();
Operand* newLabel();
Operand* newOperandConstant(int value);
Operand* newOperandFunc(Type* func);
Operand* newOperandVar(Type* var);
Operand* newOperandAddr(Operand* p);
InterCodeNode* newInterCodeNode();
InterCodeNode* newInterCodeNodeWithIC(InterCode* ic);
InterCodeNode* newInterCodeNodeWithInfo(InterCodeTag tag, Operand* tar, Operand* o1, Operand* o2);
InterCode* newInterCode(InterCodeTag tag, Operand* tar, Operand* o1, Operand* o2);



extern InterCodeNode* irHead;
void traverseIR();
InterCodeNode* concat(InterCodeNode* c1, InterCodeNode* c2);

#endif