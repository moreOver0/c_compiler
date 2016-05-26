#ifndef __COMMON_H 
#define __COMMON_H

// #define _DEBUG
#define _TRA_DEBUG

typedef int bool;
extern const int false;
extern const int true;

#include "enumTag.h"
#include "symbolTable.h"
#include "ir.h"
#include "tree.h"
#include "semantic.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>




extern Node* root;
extern int lexicalErrorCount;
extern int syntaxErrorCount;
extern int semanticErrorCount;
extern SymbolTableNode** symbolTable;

void getIR(char* fileName);

#endif