#include "common.h"

char nodeTagStr[][25] = {
    [_INT] = "INT",
    [_FLOAT] = "FLOAT",
    [_ID] = "ID",
    [_SEMI] = "SEMI",
    [_COMMA] = "COMMA",
    [_RELOP] = "RELOP",
    [_ASSIGNOP] = "ASSIGNOP",
    [_PLUS] = "PLUS",
    [_MINUS] = "MINUS",
    [_STAR] = "STAR",
    [_DIV] = "DIV",
    [_AND] = "AND",
    [_OR] = "OR",
    [_NOT] = "NOT",
    [_DOT] = "DOT",
    [_LP] = "LP",
    [_RP] = "RP",
    [_LB] = "LB",
    [_RB] = "RB",
    [_LC] = "LC",
    [_RC] = "RC",
    [_TYPE] = "TYPE",
    [_STRUCT] = "STRUCT",
    [_RETURN] = "RETURN",
    [_IF] = "IF",
    [_ELSE] = "ELSE",
    [_WHILE] = "WHILE",
    [_Program] = "Program",
    [_ExtDefList] = "ExtDefList",
    [_ExtDef] = "ExtDef",
    [_ExtDecList] = "ExtDecList",
    [_Specifier] = "Specifier",
    [_StructSpecifier] = "StructSpecifier",
    [_OptTag] = "OptTag",
    [_Tag] = "Tag",
    [_VarDec] = "VarDec",
    [_FunDec] = "FunDec",
    [_VarList] = "VarList",
    [_ParamDec] = "ParamDec",
    [_CompSt] = "CompSt",
    [_StmtList] = "StmtList",
    [_Stmt] = "Stmt",
    [_DefList] = "DefList",
    [_Def] = "Def",
    [_DecList] = "DecList",
    [_Dec] = "Dec",
    [_Exp] = "Exp",
    [_Args] = "Args"
};


Node* newNode(){
    Node* tmp = (Node*)malloc(sizeof(Node));
    memset(tmp, 0, sizeof(Node));
    return tmp;
}

// void freeNode(Node* nodePtr){
//     if(nodePtr == NULL)
//         return;
//     freeNode(nodePtr->firstChild);
//     freeNode(nodePtr->nextSibling);    
//     free(nodePtr);
//     nodePtr = NULL:
// }

void traverseSub(Node* node, int level);

void traverse(Node* rt){
    if(rt != NULL){     
        traverseSub(rt, 0);
    }
}

void traverseSub(Node* node, int level){
    if(node == NULL) return;
    for(int i = 0; i < level; ++i)
        printf("  ");
    printf("%s", nodeTagStr[node->tag]);
    if(node->isToken){
        if(_ID == node->tag || _TYPE == node->tag) printf(": %s", node->text);
        else if(_INT == node->tag) printf(": %d", node->intVal);
        else if(_FLOAT == node->tag) printf(": %f", node->floatVal);
    }else{
        printf(" (%d)", node->line);
    }
    printf("\n");

    Node* child = node->firstChild;
    while(child != NULL){
        traverseSub(child, level+1);
        child = child->nextSibling;
    }

} 