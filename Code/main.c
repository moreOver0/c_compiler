#include "common.h"

extern FILE* yyin;
extern int yylex(void);
extern void yyrestart(FILE *input_file);
extern int yyparse(void);
extern int yylineno;
extern int yycolumn;

const int false = 0;
const int true = 1;

int main(int argc, char** argv){
    if(argc < 2)
        return 1;
    FILE* file = fopen(argv[1], "r");
    if(!file){
        perror(argv[1]);
        return 1;
    }
   
    // printf("==================BEGIN====================\n");

    yyrestart(file);
    yyparse();

    if(lexicalErrorCount + syntaxErrorCount <= 0){
        // traverse(root);
        semanticCheck();
        // if(semanticErrorCount <= 0){
        //     printf("none.\n");
        // }
    }

    // printf("===================END=====================\n");
    // freeNode(root);
    return 0;
}
