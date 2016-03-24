#include <stdio.h>
#include <stdlib.h>
#include "tree.h"

extern FILE* yyin;
extern int yylex(void);
extern void yyrestart (FILE *input_file);
extern int yyparse (void);
extern int yylineno;
extern int yycolumn;
extern Node* root;
extern int lexicalErrorCount;
extern int syntaxErrorCount;

int main(int argc, char** argv){
    if(argc < 2)
        return 1;
    FILE* file = fopen(argv[1], "r");
    if(!file){
        perror(argv[1]);
        return 1;
    }
   

    printf("==================BEGIN====================\n");

    yyrestart(file);
    yyparse();

    // printf("lexical error count: %d\n", lexicalErrorCount);
    // printf(" syntax error count: %d\n", syntaxErrorCount);
    if(lexicalErrorCount + syntaxErrorCount == 0)
        traverse(root);

    printf("===================END=====================\n");

    return 0;
}
