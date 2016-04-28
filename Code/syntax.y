%locations
%{
    #include "tree.h"
    #include "lex.yy.c"  
    #include <stdlib.h>
    #include <stdarg.h>
    #include <string.h>

    extern int yylineno;
    extern int yycolumn;

    int yyerror(char* msg);

    int syntaxErrorCount = 0;
    Node* root;


%}

%union {
    Node* node;
}


/*declared tokens*/

%token      <node> INT FLOAT TYPE ID SEMI COMMA
%right      <node> ASSIGNOP NOT
%left       <node> PLUS MINUS STAR DIV RELOP AND OR 
%left       <node> DOT LP RP LB RB LC RC
%nonassoc   <node> STRUCT RETURN IF ELSE WHILE

/*declared non-terminals*/

%type <node> Program ExtDefList ExtDef ExtDecList
%type <node> Specifier StructSpecifier OptTag Tag
%type <node> VarDec FunDec VarList ParamDec
%type <node> CompSt StmtList Stmt
%type <node> DefList Def DecList Dec
%type <node> Exp Args


%%

/* High-level Definitions */
Program     : ExtDefList                { $$ = addToTree(_Program, 1, $1); root = $$; }
            ;

ExtDefList  : ExtDef ExtDefList         { $$ = addToTree(_ExtDefList, 2, $1, $2); }
            | /* empty */               { $$ = NULL; }
            ;

ExtDef      : Specifier ExtDecList SEMI { $$ = addToTree(_ExtDef, 3, $1, $2, $3); }         //全局变量声明 int x, y, z;
            | Specifier SEMI            { $$ = addToTree(_ExtDef, 2, $1, $2); }             //类型声明 struct xxx; 
            | Specifier FunDec CompSt   { $$ = addToTree(_ExtDef, 3, $1, $2, $3); }         //函数 int f(...){...}
            | error SEMI                { syntaxErrorCount++; }
            ;

ExtDecList  : VarDec                    { $$ = addToTree(_ExtDecList, 1, $1); }             
            | VarDec COMMA ExtDecList   { $$ = addToTree(_ExtDecList, 3, $1, $2, $3); }     //变量序列 x, y, z,...
            ;


/* Specifiers */
Specifier   :   TYPE                    { $$ = addToTree(_Specifier, 1, $1); }              //基本类型
            |   StructSpecifier         { $$ = addToTree(_Specifier, 1, $1); }              //结构体
            ;

StructSpecifier :   STRUCT OptTag LC DefList RC { $$ = addToTree(_StructSpecifier, 5, $1, $2, $3, $4, $5); }//结构体定义
                |   STRUCT Tag                  { $$ = addToTree(_StructSpecifier, 2, $1, $2); }            //结构体
                ;

OptTag      :   ID                      { $$ = addToTree(_OptTag, 1, $1); }                 //optional tag 可选的结构体名
            |   /* empty */             { $$ = NULL; }
            ;

Tag         :   ID                      { $$ = addToTree(_Tag, 1, $1); }
            ;


/* Declarators */
VarDec      : ID                        { $$ = addToTree(_VarDec, 1, $1); }                 //变量描述符 id
            | VarDec LB INT RB          { $$ = addToTree(_VarDec, 4, $1, $2, $3, $4); }     //数组变量表示 id[i][j]...
            ;

FunDec      : ID LP VarList RP          { $$ = addToTree(_FunDec, 4, $1, $2, $3, $4); }     //函数描述符 f(参数表)
            | ID LP RP                  { $$ = addToTree(_FunDec, 3, $1, $2, $3); }         //函数描述符 f()
            | error RP                  { syntaxErrorCount++; }
            ;

VarList     : ParamDec COMMA VarList    { $$ = addToTree(_VarList, 3, $1, $2, $3); }        //int x, int y,...
            | ParamDec                  { $$ = addToTree(_VarList, 1, $1); }                //int x
            ;

ParamDec    : Specifier VarDec          { $$ = addToTree(_ParamDec, 2, $1, $2); }           //int x
            ;


/* Statements */
CompSt      : LC DefList StmtList RC    { $$ = addToTree(_CompSt, 4, $1, $2, $3, $4); }     //{定义、初始化，stmtlist}
            | error RC                  { syntaxErrorCount++; }
            ;

StmtList    : Stmt StmtList             { $$ = addToTree(_StmtList, 2, $1, $2); }
            | /* empty */               { $$ = NULL; }          
            ;

Stmt        : Exp SEMI                      { $$ = addToTree(_Stmt, 2, $1, $2); }           //表达式;
            | CompSt                        { $$ = addToTree(_Stmt, 1, $1); }               //{定义、初始化，stmtlist}
            | RETURN Exp SEMI               { $$ = addToTree(_Stmt, 3, $1, $2, $3); }       //return 表达式;
            | IF LP Exp RP Stmt             { $$ = addToTree(_Stmt, 5, $1, $2, $3, $4, $5); }           //if(表达式) stmt
            | IF LP Exp RP Stmt ELSE Stmt   { $$ = addToTree(_Stmt, 7, $1, $2, $3, $4, $5, $6, $7); }   //if(表达式) stmt else stmt
            | WHILE LP Exp RP Stmt          { $$ = addToTree(_Stmt, 5, $1, $2, $3, $4, $5); }           //while(表达式) stmt
            | error SEMI                    { syntaxErrorCount++; }
            | error                         { syntaxErrorCount++; }
            ;


/* Local Definitions */
DefList     : Def DefList               { $$ = addToTree(_DefList, 2, $1, $2); }            //int x, y=1; float m=1.0; 定义、初始化列表
            | /* empty */               { $$ = NULL; }                                      //
            ;

Def         : Specifier DecList SEMI    { $$ = addToTree(_Def, 3, $1, $2, $3); }            //int x, y=1;
            | error SEMI                { syntaxErrorCount++; }                             //
            ;                   

DecList     : Dec                       { $$ = addToTree(_DecList, 1, $1); }                //
            | Dec COMMA DecList         { $$ = addToTree(_DecList, 3, $1, $2, $3); }        //id1, id2=表达式, id3...
            ;

Dec         : VarDec                    { $$ = addToTree(_Dec, 1, $1); }                    //id
            | VarDec ASSIGNOP Exp       { $$ = addToTree(_Dec, 3, $1, $2, $3); }            //id = 表达式
            ; 


/* Expressions */
Exp         : Exp ASSIGNOP Exp  { $$ = addToTree(_Exp, 3, $1, $2, $3); }
            | Exp AND Exp       { $$ = addToTree(_Exp, 3, $1, $2, $3); }
            | Exp OR Exp        { $$ = addToTree(_Exp, 3, $1, $2, $3); }
            | Exp RELOP Exp     { $$ = addToTree(_Exp, 3, $1, $2, $3); }
            | Exp PLUS Exp      { $$ = addToTree(_Exp, 3, $1, $2, $3); }
            | Exp MINUS Exp     { $$ = addToTree(_Exp, 3, $1, $2, $3); }
            | Exp STAR Exp      { $$ = addToTree(_Exp, 3, $1, $2, $3); }
            | Exp DIV Exp       { $$ = addToTree(_Exp, 3, $1, $2, $3); }
            | LP Exp RP         { $$ = addToTree(_Exp, 3, $1, $2, $3); }
            | MINUS Exp         { $$ = addToTree(_Exp, 2, $1, $2); }
            | NOT Exp           { $$ = addToTree(_Exp, 2, $1, $2); }
            | ID LP Args RP     { $$ = addToTree(_Exp, 4, $1, $2, $3, $4); }
            | ID LP RP          { $$ = addToTree(_Exp, 3, $1, $2, $3); }
            | Exp LB Exp RB     { $$ = addToTree(_Exp, 4, $1, $2, $3, $4); }
            | Exp DOT ID        { $$ = addToTree(_Exp, 3, $1, $2, $3); }
            | ID                { $$ = addToTree(_Exp, 1, $1); }
            | INT               { $$ = addToTree(_Exp, 1, $1); }
            | FLOAT             { $$ = addToTree(_Exp, 1, $1); }
            | error             { syntaxErrorCount++; }
            ;

Args        : Exp COMMA Args    { $$ = addToTree(_Args, 3, $1, $2, $3); }
            | Exp               { $$ = addToTree(_Args, 1, $1); }
            ;




%%


int yyerror(char* msg){
    printf("Error type B at line %d, column %d: %s\n", yylineno, yycolumn, msg);
    return 0;
}

Node* addToTree(int nodeTag, int argc, ...){
    if(argc <= 0) return NULL;
    Node* res = newNode();
    res->tag = nodeTag;
    res->firstChild = NULL;

    va_list argv;
    va_start(argv, argc);

    Node* child = NULL;
    int i = 0;
    while(i < argc && child == NULL){
        child = va_arg(argv, Node*);
        i++;
    }
    if(child != NULL){
        res->line = child->line;
        res->column = child->column;
        res->firstChild = child;  

        for(; i < argc; ++i){
            child->nextSibling = va_arg(argv, Node*);
            if(child->nextSibling != NULL)
                child = child->nextSibling;
        }
        child->nextSibling = NULL;
    }
    va_end(argv);

    //printf("get %s line:%d\n", nodeTagStr[nodeTag], yylineno);

    return res;
}




