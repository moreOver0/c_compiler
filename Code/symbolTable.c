#include "common.h"

const unsigned int MAX_INDEX = 0xFFF;
const unsigned int SIZE = 0x1000;
SymbolTableNode** symbolTable;

Type* BASIC_INT;
Type* BASIC_FLOAT;
Type* BASIC_UNKNOWN;

void initSymbolTable(){
    symbolTable = (SymbolTableNode**)malloc(sizeof(SymbolTableNode*)*SIZE);
    memset(symbolTable, 0, sizeof(SymbolTableNode*)*SIZE);

    BASIC_INT = newType();
    BASIC_INT->name = "int";
    BASIC_INT->typeTag = define_int;

    BASIC_FLOAT = newType();
    BASIC_FLOAT->name = "float";
    BASIC_FLOAT->typeTag = define_float;

    BASIC_UNKNOWN = newType();
    BASIC_UNKNOWN->name = "unknown";
    BASIC_UNKNOWN->typeTag = type_unknown;
}

Symbol* findSymbolTable(char* name){
    if(name == NULL) return NULL;
    unsigned int hashCode = hashPJW(name);
    SymbolTableNode* bucket = symbolTable[hashCode];
    while(bucket != NULL){
        Symbol* tmp = bucket->symbol;
        if(strcmp(tmp->name, name) == 0)
            return tmp;
        bucket = bucket->next;
    }
    return NULL;
}

static bool alreadyExists(char* name){
    if(name == NULL) return true;
    if(findSymbolTable(name) != NULL) return true;
    return false;
}

// 两个无名struct " struct <cnt>" 不加进去

bool insertSymbolTable(SymbolTableNode* node){
    if(node==NULL) return false;
    Symbol* symbol = node->symbol;
    if(symbol->name == NULL) return false;

    if(alreadyExists(symbol->name))
        return false;
    unsigned int hashCode = hashPJW(symbol->name);
    node->next = symbolTable[hashCode];
    symbolTable[hashCode] = node;
    return true;
}

bool insertSymbolTableType(Type* symbol){
    SymbolTableNode* node = newSymbolTableNode();
    node->symbol = symbol;
    return insertSymbolTable(node);
}

Type* newType(){
    Type* res = (Type*)malloc(sizeof(Type));
    memset(res, 0, sizeof(Type));
    return res;
}

// void freeType(Type* t){
//     if(t == NULL) return;
//     free(t);
//     t = NULL;
// }

SymbolTableNode* newSymbolTableNode(){
    SymbolTableNode* res = (SymbolTableNode*)malloc(sizeof(SymbolTableNode));
    memset(res, 0, sizeof(SymbolTableNode));
    return res;
}

// void freeSymbolTableNode(SymbolTableNode* t){
//     if(t == NULL) return;
//     free(t);
//     t = NULL;
// }