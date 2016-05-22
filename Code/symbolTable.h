#ifndef __SYMBOL_TABLE_H
#define __SYMBOL_TABLE_H

#include "common.h"

typedef struct Type{
    char* name;
    TypeTag typeTag;
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
            // TypeTag returnType;
            /* point to type of return-value if return a structure */
            struct Type* ret;
            
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
extern Type* BASIC_UNKNOWN;

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


Type* newType();
// void freeType(Type* t);

SymbolTableNode* newSymbolTableNode();
// void freeSymbolTableNode(SymbolTableNode* t);




#endif