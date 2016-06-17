#include "common.h"

char registerStr[][10] = {
    "$zero",                                                  // $0
    "$at",                                                    // $1
    "$v0", "$v1",                                             // $2  ~ $3
    "$a0", "$a1", "$a2", "$a3",                               // $4  ~ $7
    "$t0", "$t1", "$t2", "$t3", "$t4", "$t5", "$t6", "$t7",   // $8  ~ $15
    "$s0", "$s1", "$s2", "$s3", "$s4", "$s5", "$s6", "$s7",   // $16 ~ $23
    "$t8", "$t9",                                             // $24 ~ $25
    "$k0", "$k1",                                             // $26 ~ $27
    "$gp",                                                    // $28
    "$sp",                                                    // $29
    "$s8",                                                    // $30
    "$ra"                                                     // $31
};


int getOffset(Operand* o, Type* currentFunc){
    if(o == NULL) assert(0);
    if(currentFunc == NULL) assert(0);
    if(o->tag == variable){
        return o->var->offset;
    }else if(o->tag == tempVariable){
        int tmp = o->no - currentFunc->function.tempBeginNo;
        return currentFunc->function.dataSize - tmp * 4;
    }else if(o->tag == address){
        // printf("\n\n%d\n\n", getOffset(o->pointTo, currentFunc));
        return getOffset(o->pointTo, currentFunc);
    }else{

        assert(0);
    }
}

void getRegister(FILE* fileOut, InterCode* ic, Type* currentFunc){
    if(ic == NULL) assert(0);
    if(currentFunc == NULL) assert(0);

    InterCodeTag tag = ic->tag;
    Operand* tar = ic->target;
    Operand* o1 = ic->o1;
    Operand* o2 = ic->o2;

    ic->rtar = T0;
    ic->ro1 = T1;
    ic->ro2 = T2;

    if(IR_IF_GOTO == tag){
        if(o1->tag == constant)
            fprintf(fileOut, "li $t1, %d\n", o1->value);
        else
            fprintf(fileOut, "lw $t1, %d($sp)\n", getOffset(o1, currentFunc));
        if(o2->tag == constant)
            fprintf(fileOut, "li $t2, %d\n", o2->value);
        else
            fprintf(fileOut, "lw $t2, %d($sp)\n", getOffset(o2, currentFunc));

    }else if(IR_RETURN == tag){
        if(tar->tag == constant)
            fprintf(fileOut, "li $t0, %d\n", tar->value);
        else
            fprintf(fileOut, "lw $t0, %d($sp)\n", getOffset(tar, currentFunc));

    }else if(IR_ARG == tag){
        if(tar->tag == constant)
            fprintf(fileOut, "li $t0, %d\n", tar->value);
        else
            fprintf(fileOut, "lw $t0, %d($sp)\n", getOffset(tar, currentFunc));
 
    }else if(IR_CALL_ASSIGN == tag){

    }else if(IR_MUL == tag || IR_DIV == tag){
        if(o1->tag == constant)
            fprintf(fileOut, "li $t1, %d\n", o1->value);
        else
            fprintf(fileOut, "lw $t1, %d($sp)\n", getOffset(o1, currentFunc));
        if(o2->tag == constant)
            fprintf(fileOut, "li $t2, %d\n", o2->value);
        else
            fprintf(fileOut, "lw $t2, %d($sp)\n", getOffset(o2, currentFunc));

    }else if(IR_READ_FROM_ADDR  == tag || IR_WRITE_TO_ADDR  == tag){
        if(tar->tag == constant)
            fprintf(fileOut, "li $t0, %d\n", tar->value);
        else
            fprintf(fileOut, "lw $t0, %d($sp)\n", getOffset(tar, currentFunc));
        if(o1->tag == constant)
            fprintf(fileOut, "li $t1, %d\n", o1->value);
        else
            fprintf(fileOut, "lw $t1, %d($sp)\n", getOffset(o1, currentFunc));
    
    }else if(IR_ADD == ic->tag || IR_SUB == ic->tag){
        // 常数不用放进寄存器
        if(o1->tag == address){
            fprintf(fileOut, "addi $t1, $sp, %d\n", getOffset(o1->pointTo, currentFunc));
        }
        else if(o1->tag != constant)
            fprintf(fileOut, "lw $t1, %d($sp)\n", getOffset(o1, currentFunc));
        if(o2->tag == address){
            fprintf(fileOut, "addi $t2, $sp, %d\n", getOffset(o2->pointTo, currentFunc));
        }
        else if(o2->tag != constant)
            fprintf(fileOut, "lw $t2, %d($sp)\n", getOffset(o2, currentFunc));

    }else if(IR_ASSIGN == ic->tag){
        // 常数不用放进寄存器
        if(o1->tag == address)
            fprintf(fileOut, "addi $t1, $sp, %d\n", getOffset(o1->pointTo, currentFunc));
        else if(o1->tag != constant)
            fprintf(fileOut, "lw $t1, %d($sp)\n", getOffset(o1, currentFunc));
    }else if(IR_WRITE == tag){
        if(tar->tag != constant)
            fprintf(fileOut, "lw $t0, %d($sp)\n", getOffset(tar, currentFunc));
        else 
            fprintf(fileOut, "li $t0, %d\n", tar->value);
    }
}