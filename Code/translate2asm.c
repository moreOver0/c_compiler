#include "common.h"

/*
    InterCodeNode* irHead;


 */

void getAsm(char* fileName){
    FILE* fileOut = fopen(fileName, "w");
    
    FILE* prefix = fopen("prefixAsm.txt", "r");
    char tmp[200];
    while(fgets(tmp, 200, prefix) != NULL){
        fputs(tmp, fileOut);
    }
    fclose(prefix);

    InterCodeNode* p = irHead;
    bool begin = false;

    int dataSize = 0, tempSize = 0;
    int argOffset = 0;

    Type* currentFunc = NULL;

    // int i = 1;
    while((begin && p != irHead) || !begin){
        begin = true;
        InterCode* ic = p->ic;
        InterCodeTag tag = ic->tag;
        Operand* tar = ic->target;
        Operand* o1 = ic->o1;
        Operand* o2 = ic->o2;
        char* relop = ic->relop;

        // printf("%d\n", i);i++;

        if(IR_FUNC == tag){
            currentFunc = tar->func;
        }

        getRegister(fileOut, ic, currentFunc);
        Register rtar = ic->rtar;
        Register ro1 = ic->ro1;
        Register ro2 = ic->ro2;

        if(IR_ARG != tag){
            argOffset = 0;
        }

        if(IR_LABEL == tag){
            fprintf(fileOut, "label%d:\n", tar->no);
        }else if(IR_GOTO == tag){
            fprintf(fileOut, "j label%d\n", tar->no);
        }else if(IR_IF_GOTO == tag){
            if(strcmp(relop, "==") == 0){
                fprintf(fileOut, "beq ");
            }else if(strcmp(relop, "!=") == 0){
                fprintf(fileOut, "bne ");
            }else if(strcmp(relop, ">") == 0){
                fprintf(fileOut, "bgt ");
            }else if(strcmp(relop, "<") == 0){
                fprintf(fileOut, "blt ");                
            }else if(strcmp(relop, ">=") == 0){
                fprintf(fileOut, "bge ");
            }else if(strcmp(relop, "<=") == 0){
                fprintf(fileOut, "ble ");
            }
            fprintf(fileOut, "%s, %s, label%d\n", registerStr[ro1], registerStr[ro2], tar->no);
        }else if(IR_FUNC == tag){
            fprintf(fileOut, "\n%s:\n", tar->func->name);
            if(strcmp(tar->func->name, "main") == 0)
                fprintf(fileOut, "addi $sp, $sp, -4\n");
            fprintf(fileOut, "sw $ra, 0($sp)\n");
            currentFunc = tar->func;
            dataSize = tar->func->function.dataSize;
            tempSize = tar->func->function.tempSize;
        }else if(IR_PARAM == tag){
        }else if(IR_DEC == tag){
        }else if(IR_RETURN == tag){
            fprintf(fileOut, "move $v0, %s\n", registerStr[rtar]);
            fprintf(fileOut, "jr $ra\n");
        }else if(IR_ARG == tag){
            // need to push the value of arguments into stack (cause of no array or structure)
            fprintf(fileOut, "sw %s, %d($sp)\n", registerStr[rtar], argOffset+dataSize+tempSize);
            argOffset += 4;
        }else if(IR_CALL_ASSIGN == tag){
            // fprintf(fileOut, "sw $ra, 0($sp)\n");
            fprintf(fileOut, "addi $sp, $sp, %d\n", dataSize+tempSize-4*(o1->func->function.argc));
            fprintf(fileOut, "jal %s\n", o1->func->name);
            fprintf(fileOut, "addi $sp, $sp, %d\n", (-1)*(dataSize+tempSize-4*(o1->func->function.argc)));
            fprintf(fileOut, "lw $ra, 0($sp)\n");

            if(tar != NULL){
                // fprintf(fileOut, "move %s, $v0\n", registerStr[rtar]);    
                // fprintf(fileOut, "sw %s, %d($sp)\n", registerStr[rtar], ); 
                fprintf(fileOut, "sw $v0, %d($sp)\n", getOffset(tar, currentFunc)); 
            }

        }else if(IR_ASSIGN == tag){
            if(constant == o1->tag){
                fprintf(fileOut, "li %s, %d\n", registerStr[rtar], o1->value);     
            }else{
                fprintf(fileOut, "move %s, %s\n", registerStr[rtar], registerStr[ro1]);     
            }
            fprintf(fileOut, "sw %s, %d($sp)\n", registerStr[rtar], getOffset(tar, currentFunc));
        }else if(IR_ADD == tag){
            if(constant == o2->tag && constant != o1->tag){
                fprintf(fileOut, "addi %s, %s, %d\n", registerStr[rtar], registerStr[ro1], o2->value);  
            }else if(constant == o1->tag && constant != o2->tag){
                fprintf(fileOut, "addi %s, %s, %d\n", registerStr[rtar], registerStr[ro2], o1->value);  
            }else if(constant != o1->tag && constant != o2->tag){
                fprintf(fileOut, "add %s, %s, %s\n", registerStr[rtar], registerStr[ro1], registerStr[ro2]);
            }else if(constant == o1->tag && constant == o2->tag){
                fprintf(fileOut, "addi %s, %s, %d\n", registerStr[rtar], registerStr[rtar], o1->value);
                fprintf(fileOut, "addi %s, %s, %d\n", registerStr[rtar], registerStr[rtar], o2->value);
            }
            fprintf(fileOut, "sw %s, %d($sp)\n", registerStr[rtar], getOffset(tar, currentFunc));

        }else if(IR_SUB == tag){
            if(constant == o2->tag && constant != o1->tag){
                fprintf(fileOut, "addi %s, %s, %d\n", registerStr[rtar], registerStr[ro1], (-1)*(o2->value));  
            }else if(constant == o1->tag && constant != o2->tag){
                fprintf(fileOut, "addi %s, %s, %d\n", registerStr[rtar], registerStr[rtar], o1->value); 
                fprintf(fileOut, "sub %s, %s, %s\n", registerStr[rtar], registerStr[rtar], registerStr[ro2]);
            }else if(constant != o1->tag && constant != o2->tag){
                fprintf(fileOut, "sub %s, %s, %s\n", registerStr[rtar], registerStr[ro1], registerStr[ro2]);
            }else if(constant == o1->tag && constant == o2->tag){
                fprintf(fileOut, "addi %s, %s, %d\n", registerStr[rtar], registerStr[rtar], o1->value);
                fprintf(fileOut, "addi %s, %s, %d\n", registerStr[rtar], registerStr[rtar], (-1)*(o2->value));
            }
            fprintf(fileOut, "sw %s, %d($sp)\n", registerStr[rtar], getOffset(tar, currentFunc));

        }else if(IR_MUL == tag){
            fprintf(fileOut, "mul %s, %s, %s\n", registerStr[rtar], registerStr[ro1], registerStr[ro2]);
            fprintf(fileOut, "sw %s, %d($sp)\n", registerStr[rtar], getOffset(tar, currentFunc));

        }else if(IR_DIV == tag){
            fprintf(fileOut, "div %s, %s\n", registerStr[ro1], registerStr[ro2]);
            fprintf(fileOut, "mflo %s", registerStr[rtar]);
            fprintf(fileOut, "sw %s, %d($sp)\n", registerStr[rtar], getOffset(tar, currentFunc));

        }else if(IR_READ_FROM_ADDR  == tag){
            fprintf(fileOut, "lw %s, 0(%s)\n", registerStr[rtar], registerStr[ro1]);
            fprintf(fileOut, "sw %s, %d($sp)\n", registerStr[rtar], getOffset(tar, currentFunc));

        }else if(IR_WRITE_TO_ADDR  == tag){
            fprintf(fileOut, "sw %s, 0(%s)\n", registerStr[ro1], registerStr[rtar]);
        }else if(IR_READ == tag){
            // fprintf(fileOut, "sw $ra, 0($sp)\n");
            // fprintf(fileOut, "addi $sp, $sp, -4\n");
            fprintf(fileOut, "jal read\n");
            // fprintf(fileOut, "addi $sp, $sp, 4\n");
            fprintf(fileOut, "lw $ra, 0($sp)\n");

            fprintf(fileOut, "sw $v0, %d($sp)\n", getOffset(tar, currentFunc));

        }else if(IR_WRITE == tag){
            fprintf(fileOut, "move $a0, %s\n", registerStr[rtar]);
            // fprintf(fileOut, "sw $ra, 0($sp)\n");
            // fprintf(fileOut, "addi $sp, $sp, -4\n");
            fprintf(fileOut, "jal write\n");
            // fprintf(fileOut, "addi $sp, $sp, 4\n");
            fprintf(fileOut, "lw $ra, 0($sp)\n");
        }
        p = p->next;
    }


    fclose(fileOut);
}
