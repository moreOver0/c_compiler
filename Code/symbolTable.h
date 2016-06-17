#ifndef __SYMBOL_TABLE_H
#define __SYMBOL_TABLE_H

#include "common.h"

typedef struct Type{
    char* name;
    TypeTag typeTag;
    int offset;
    union{
        /* if array */
        struct{
            int size;
            struct Type* element;
        }array;

        /* if function */
        struct{
            int argc;
            /* array of arguments */
            struct Type** argv;
            /* point to type of return-value if return a structure */
            struct Type* ret;
            /* 4 + size of all data and variables in this function (4 means the link to next instruction) */
            int dataSize; 
            int tempSize;
            int tempBeginNo;
        }function;

        /* if definition of structure */
        struct{
            int elementCount;
            struct Type** element;
        }structure;

        /* point to specific info if this is a instance of some type */
        struct Type* myType;    
    };

    // int definitionLine;
}Type;

extern Type* BASIC_INT;
extern Type* BASIC_FLOAT;
extern Type* BASIC_UNKNOWN_TYPE;

typedef struct Type Symbol;

typedef struct SymbolTableNode{
    Type* symbol;
    struct SymbolTableNode* next;
}SymbolTableNode;


unsigned int hashPJW(char* name);

void initSymbolTable();
bool insertSymbolTable(SymbolTableNode* node);
bool insertSymbolTableType(Type* symbol);
Symbol* findSymbolTable(char* name);

bool checkType(Type* t1, Type* t2);
void showType(Type* t);
int computeSizeByByte(Type* t);

Type* newType();
// void freeType(Type* t);

SymbolTableNode* newSymbolTableNode();
// void freeSymbolTableNode(SymbolTableNode* t);




#endif