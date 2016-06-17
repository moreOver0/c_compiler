#include "common.h"

InterCodeNode* irHead;

int tempVariableCount = 1;
int labelCount = 1;


static Operand* newOperand(OperandTag tag){
    Operand* tmp = (Operand*)malloc(sizeof(Operand));
    memset(tmp, 0, sizeof(Operand));
    tmp->tag = tag;
    if(tag == tempVariable){
        tmp->no = -1;
        // tempVariableCount++;
    }else if(tag == label){
        tmp->no = -1;
        // labelCount++;
    }
    return tmp;
}

Operand* newTemp(){
    return newOperand(tempVariable);
}

Operand* newLabel(){
    return newOperand(label);
}

Operand* newOperandConstant(int value){
    Operand* tmp = newOperand(constant);
    tmp->value = value;
    return tmp;
}

Operand* newOperandFunc(Type* func){
    Operand* tmp = newOperand(function);
    tmp->func = func;
    return tmp;
}

Operand* newOperandVar(Type* var){
    Operand* tmp = newOperand(variable);
    tmp->var = var;
    return tmp;
}

Operand* newOperandAddr(Operand* p){
    Operand* tmp = newOperand(address);
    tmp->pointTo = p;
    return tmp;
}

InterCodeNode* newInterCodeNode(){
    InterCodeNode* tmp = (InterCodeNode*)malloc(sizeof(InterCodeNode));
    tmp->ic = NULL;
    tmp->prev = tmp;
    tmp->next = tmp;
    return tmp;
}

InterCodeNode* newInterCodeNodeWithIC(InterCode* ic){
    InterCodeNode* tmp = newInterCodeNode();
    tmp->ic = ic;
    return tmp;
}

InterCode* newInterCode(InterCodeTag tag, Operand* tar, Operand* o1, Operand* o2){
    InterCode* tmp = (InterCode*)malloc(sizeof(InterCode));
    memset(tmp, 0, sizeof(InterCode));
    tmp->tag = tag, tmp->target = tar, tmp->o1 = o1, tmp->o2 = o2;
    tmp->relop = NULL;
    return tmp;
}

InterCodeNode* newInterCodeNodeWithInfo(InterCodeTag tag, Operand* tar, Operand* o1, Operand* o2){
    return newInterCodeNodeWithIC(newInterCode(tag, tar, o1, o2));
}

InterCodeNode* concat(InterCodeNode* c1, InterCodeNode* c2){
    if(c1 == NULL)return c2;
    if(c2 == NULL)return c1;

    InterCodeNode* res = c1;
    InterCodeNode* tail = c2->prev;
    c1 = c1->prev;
    c1->next = c2;
    c2->prev = c1;
    tail->next = res;
    res->prev = tail;
    return res;
}


char* printOperand(Operand* o){
    if(o == NULL) return "(null)";
    char* s = (char*)malloc(sizeof(char) * 20);
    if(o->tag == tempVariable){
        if(o->no == -1){
            o->no = tempVariableCount++;
        }
        sprintf(s, "t%d", o->no);
    }
    // else if(o->tag == variable) sprintf(s, "%s", o->var->name);
    else if(o->tag == variable) sprintf(s, "%s(%d)", o->var->name, o->var->offset);
    else if(o->tag == constant) sprintf(s, "#%d", o->value);
    else if(o->tag == address){ sprintf(s, "&%s", printOperand(o->pointTo)); }
    else if(o->tag == function) sprintf(s, "%s", o->func->name);
    else if(o->tag == label){
        if(o->no == -1){
            o->no = labelCount++;
        }
        sprintf(s, "label%d", o->no);
    } 
    else sprintf(s, "??");
    return s;
}

void traverseIR(FILE* f){

    InterCodeNode* p = irHead;
    bool begin = false;

    Type* lastFunc = NULL;
    int lastTempCount = 1;


    int funcCount = 0;
    Type* funcList[30];

    while((begin && p != irHead) || !begin){
        begin = true;
        InterCode* ic = p->ic;
        InterCodeTag tag = ic->tag;
        Operand* tar = ic->target;
        Operand* o1 = ic->o1;
        Operand* o2 = ic->o2;
        char* relop = ic->relop;

        if(IR_FUNC == tag && (tar->func != lastFunc || lastFunc == NULL)){
            if(lastFunc != NULL){
                lastFunc->function.tempSize = -4 * (tempVariableCount - lastTempCount);
                lastTempCount = tempVariableCount;
            }
            lastFunc = tar->func;
        }

        if(IR_LABEL == tag){
            fprintf(f, "LABEL %s :", printOperand(tar));
        }else if(IR_GOTO == tag){
            fprintf(f, "GOTO %s", printOperand(tar));
        }else if(IR_IF_GOTO == tag){
            fprintf(f, "IF %s %s %s GOTO %s", printOperand(o1), relop, printOperand(o2), printOperand(tar));
        }else if(IR_FUNC == tag){
            fprintf(f, "FUNCTION %s (dataSize %d):", printOperand(tar), (tar->func->function.dataSize)*(-1)-4);
            tar->func->function.tempBeginNo = tempVariableCount;
            funcList[funcCount] = tar->func;
            funcCount++;
        }else if(IR_PARAM == tag){
            fprintf(f, "PARAM %s", printOperand(tar));
        }else if(IR_DEC == tag){
            fprintf(f, "DEC %s %d", printOperand(tar), o1->value);
        }else if(IR_RETURN == tag){
            fprintf(f, "RETURN %s", printOperand(tar));
        }else if(IR_ARG == tag){
            fprintf(f, "ARG %s", printOperand(tar));
        }else if(IR_CALL_ASSIGN == tag){
            fprintf(f, "%s := CALL %s", printOperand(tar), printOperand(o1));
        }else if(IR_ASSIGN == tag){
            fprintf(f, "%s := %s", printOperand(tar), printOperand(o1));
        }else if(IR_ADD == tag){
            fprintf(f, "%s := %s + %s", printOperand(tar), printOperand(o1), printOperand(o2));
        }else if(IR_SUB == tag){
            fprintf(f, "%s := %s - %s", printOperand(tar), printOperand(o1), printOperand(o2));
        }else if(IR_MUL == tag){
            fprintf(f, "%s := %s * %s", printOperand(tar), printOperand(o1), printOperand(o2));
        }else if(IR_DIV == tag){
            fprintf(f, "%s := %s / %s", printOperand(tar), printOperand(o1), printOperand(o2));
        }else if(IR_READ_FROM_ADDR  == tag){
            fprintf(f, "%s := *%s", printOperand(tar), printOperand(o1));
        }else if(IR_WRITE_TO_ADDR  == tag){
            fprintf(f, "*%s := %s", printOperand(tar), printOperand(o1));
        }else if(IR_READ == tag){
            fprintf(f, "READ %s", printOperand(tar));
        }else if(IR_WRITE == tag){
            fprintf(f, "WRITE %s", printOperand(tar));
        }
        fprintf(f, "\n");
        p = p->next;
    }

    if(lastFunc != NULL){
    lastFunc->function.tempSize = -4 * (tempVariableCount - lastTempCount);
    }

    for(int i = 0; i < funcCount; ++i){
        fprintf(f, "%s (tempSize %d  %d)\n", funcList[i]->name, funcList[i]->function.tempSize, funcList[i]->function.tempBeginNo);
    }
}


