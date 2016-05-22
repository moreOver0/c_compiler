#ifndef __TREE_H
#define __TREE_H

#include "common.h"

/* Definition of syntax tree node */
typedef struct Node {
    int line, column;
    int isToken;        /* if production of lexical.l */
    NodeTag tag; 
    RuleTag rule;
    char* text;         /* 原文 */
    union {
        float floatVal;
        int intVal;
    };

    struct Node* firstChild;
    struct Node* nextSibling;

    // struct {
        Type *type;
        // const char *name;
        // int lineno;
    // }semantic;


}Node;


Node* newNode();
// void freeNode(Node* nodePtr);
void traverse(Node* node);
Node* addToTree(int nodeTag, int rule, int argc, ...);

extern char nodeTagStr[][25];

#endif