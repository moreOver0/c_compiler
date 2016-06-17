#ifndef __COMMON_H 
#define __COMMON_H

// #define _DEBUG
// #define _TRA_DEBUG

typedef int bool;
extern const int false;
extern const int true;

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "enumTag.h"
#include "symbolTable.h"
#include "ir.h"
#include "tree.h"
#include "semantic.h"
#include "register.h"





extern Node* root;
extern int lexicalErrorCount;
extern int syntaxErrorCount;
extern int semanticErrorCount;
extern SymbolTableNode** symbolTable;

void getIR(char* fileName);
void getAsm(char* fileName);


#endif