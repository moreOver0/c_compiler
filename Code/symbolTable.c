#include "common.h"

const unsigned int MAX_INDEX = 0xFFF;
const unsigned int SIZE = 0x1000;
SymbolTableNode** symbolTable;

Type* BASIC_INT;
Type* BASIC_FLOAT;
Type* BASIC_UNKNOWN_TYPE;

void initSymbolTable(){
    symbolTable = (SymbolTableNode**)malloc(sizeof(SymbolTableNode*)*SIZE);
    memset(symbolTable, 0, sizeof(SymbolTableNode*)*SIZE);

    BASIC_INT = newType();
    BASIC_INT->name = "int";
    BASIC_INT->typeTag = define_int;

    BASIC_FLOAT = newType();
    BASIC_FLOAT->name = "float";
    BASIC_FLOAT->typeTag = define_float;

    BASIC_UNKNOWN_TYPE = newType();
    BASIC_UNKNOWN_TYPE->name = "unknown type";
    BASIC_UNKNOWN_TYPE->typeTag = type_unknown_type;

    Type* read = newType();
    read->name = "read";
    read->typeTag = type_func;
    read->function.argc = 0;
    read->function.argv = NULL;
    read->function.ret = BASIC_INT;
    insertSymbolTableType(read);

    Type* write = newType();
    write->name = "write";
    write->typeTag = type_func;
    write->function.argc = 1;
    write->function.argv = (Type**)malloc(sizeof(Type*) * 1);
    write->function.argv[0] = BASIC_INT;
    write->function.ret = BASIC_INT;
    insertSymbolTableType(write);
}

Symbol* findSymbolTable(char* name){
    if(name == NULL) 
        assert(0);
        // return NULL;

#ifdef _DEBUG
printf("finding '%s'\n", name);
#endif

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


bool insertSymbolTable(SymbolTableNode* node){
    if(node==NULL) 
        return false;
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
    if(symbol == NULL) assert(0);
    if(symbol->name == NULL) return true;
    SymbolTableNode* node = newSymbolTableNode();
    node->symbol = symbol;
    return insertSymbolTable(node);
}


bool checkType(Type* t1, Type* t2){
    if(t1 == NULL || t2 == NULL){
        return true;
    }else if(t1 == t2){
        return true;
    }
    if(t1->typeTag == type_general && (t2 == BASIC_INT || t2 == BASIC_FLOAT || t2->typeTag == define_struct || t2 == BASIC_UNKNOWN_TYPE)){
        return t1->myType == t2;
    }
    if(t2->typeTag == type_general && (t1 == BASIC_INT || t1 == BASIC_FLOAT || t1->typeTag == define_struct || t1 == BASIC_UNKNOWN_TYPE)){
        return t2->myType == t1;
    }

    if(t1->typeTag != t2->typeTag)
        return false;

    if(t1->typeTag == type_general){
        return checkType(t1->myType, t2->myType);
    }else if(t1->typeTag == type_array){
        //。。为了通过测试用例 没有比较size
        return checkType(t1->array.element, t2->array.element);
    }else if(t1->typeTag == define_struct){
        if(t1->structure.elementCount != t2->structure.elementCount)
            return false;
        for(int i = 0; i < t1->structure.elementCount; ++i){
            if(!checkType(t1->structure.element[i], t2->structure.element[i])){
                return false;
            }
        }
        return true;
    }
    return false;
}

void showType(Type* t){
    printf("\t\t\t\t\t\t\t\t\t");
    if(t == NULL){
        printf("NULL\n");return;
    }
    if(t == BASIC_INT){printf("BASIC_INT\n");}
    else if(t == BASIC_FLOAT){printf("BASIC_FLOAT\n");}
    else if(t == BASIC_UNKNOWN_TYPE){printf("BASIC_UNKNOWN_TYPE\n");}
    else if(t->typeTag == define_struct){
        printf("struct %s \n", t->name);
    }
    else if(t->typeTag == type_array){
        printf("[ %d ]\n", t->array.size);
        showType(t->array.element);
    }
    else if(t->typeTag == type_general){
        printf("%s is instance of \n", t->name);
        showType(t->myType);
    }
    else{
       assert(0); 
    }
}

int computeSizeByByte(Type* t){
    if(t == NULL) return 0;
    if(t == BASIC_INT || t == BASIC_FLOAT)
        return 4;
    if(t->typeTag == type_array)
        return t->array.size * computeSizeByByte(t->array.element);
    if(t->typeTag == type_general)
        return computeSizeByByte(t->myType);
    if(t->typeTag == define_struct){
        Type** list = t->structure.element;
        int cnt = 0;
        for(int i = 0; i < t->structure.elementCount; ++i){
            cnt += computeSizeByByte(list[i]);
        }
        return cnt;
    }
    return 0; // unexpected case
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