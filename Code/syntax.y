%locations
%{
    #include "common.h"
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
%right      <node> ASSIGNOP
%left       <node> OR AND RELOP
%left       <node> PLUS MINUS 
%left       <node> STAR DIV 
%right      <node> NOT
%left       <node> LP RP DOT LB RB LC RC
%nonassoc   <node> STRUCT RETURN IF ELSE WHILE



/*declared non-terminals*/

%type <node> Program ExtDefList ExtDef ExtDecList
%type <node> Specifier StructSpecifier
%type <node> VarDec FunDec VarList ParamDec
%type <node> CompSt StmtList Stmt
%type <node> DefList Def DecList Dec
%type <node> Exp Args


%%

/* High-level Definitions */
Program     : ExtDefList                { $$ = addToTree(_Program, Program__ExtDefList, 1, $1); root = $$; }
            ;

ExtDefList  : ExtDef ExtDefList         { $$ = addToTree(_ExtDefList, ExtDefList__ExtDef_ExtDefList, 2, $1, $2); }
            | /* empty */               { $$ = NULL; }
            ;

ExtDef      : Specifier ExtDecList SEMI { $$ = addToTree(_ExtDef, ExtDef__Specifier_ExtDecList_SEMI, 3, $1, $2, $3); }         //全局变量声明 int x, y, z;
            | Specifier SEMI            { $$ = addToTree(_ExtDef, ExtDef__Specifier_SEMI, 2, $1, $2); }             //类型声明 struct xxx; 
            | Specifier FunDec CompSt   { $$ = addToTree(_ExtDef, ExtDef__Specifier_FunDec_CompSt, 3, $1, $2, $3); }         //函数 int f(...){...}
            | error SEMI                { syntaxErrorCount++; }
            ;

ExtDecList  : VarDec                    { $$ = addToTree(_ExtDecList, ExtDecList__VarDec, 1, $1); }             
            | VarDec COMMA ExtDecList   { $$ = addToTree(_ExtDecList, ExtDecList__VarDec_COMMA_ExtDecList, 3, $1, $2, $3); }     //变量序列 x, y, z,...
            ;


/* Specifiers */
Specifier   :   TYPE                    { $$ = addToTree(_Specifier, Specifier__TYPE, 1, $1); }              //基本类型
            |   StructSpecifier         { $$ = addToTree(_Specifier, Specifier__StructSpecifier, 1, $1); }              //结构体
            ;

StructSpecifier :   STRUCT ID LC DefList RC { $$ = addToTree(_StructSpecifier, StructSpecifier__STRUCT_ID_LC_DefList_RC, 5, $1, $2, $3, $4, $5); }//结构体定义
                |   STRUCT LC DefList RC { $$ = addToTree(_StructSpecifier, StructSpecifier__STRUCT_LC_DefList_RC, 4, $1, $2, $3, $4); }//结构体定义
                |   STRUCT ID                  { $$ = addToTree(_StructSpecifier, StructSpecifier__STRUCT_ID, 2, $1, $2); }            //结构体
                ;


/* Declarators */
VarDec      : ID                        { $$ = addToTree(_VarDec, VarDec__ID, 1, $1); }                 //变量描述符 id
            | VarDec LB INT RB          { $$ = addToTree(_VarDec, VarDec__VarDec_LB_INT_RB, 4, $1, $2, $3, $4); }     //数组变量表示 id[i][j]...
            ;

FunDec      : ID LP VarList RP          { $$ = addToTree(_FunDec, FunDec__ID_LP_VarList_RP, 4, $1, $2, $3, $4); }     //函数描述符 f(参数表)
            | ID LP RP                  { $$ = addToTree(_FunDec, FunDec__ID_LP_RP, 3, $1, $2, $3); }         //函数描述符 f()
            | error RP                  { syntaxErrorCount++; }
            ;

VarList     : ParamDec COMMA VarList    { $$ = addToTree(_VarList, VarList__ParamDec_COMMA_VarList, 3, $1, $2, $3); }        //int x, int y,...
            | ParamDec                  { $$ = addToTree(_VarList, VarList__ParamDec, 1, $1); }                //int x
            ;

ParamDec    : Specifier VarDec          { $$ = addToTree(_ParamDec, ParamDec__Specifier_VarDec, 2, $1, $2); }           //int x
            ;


/* Statements */
CompSt      : LC DefList StmtList RC    { $$ = addToTree(_CompSt, CompSt__LC_DefList_StmtList_RC, 4, $1, $2, $3, $4); }     //{定义、初始化，stmtlist}
            | error RC                  { syntaxErrorCount++; }
            ;

StmtList    : Stmt StmtList             { $$ = addToTree(_StmtList, StmtList__Stmt_StmtList, 2, $1, $2); }
            | /* empty */               { $$ = NULL; }          
            ;

Stmt        : Exp SEMI                      { $$ = addToTree(_Stmt, Stmt__Exp_SEMI, 2, $1, $2); }           //表达式;
            | CompSt                        { $$ = addToTree(_Stmt, Stmt__CompSt, 1, $1); }               //{定义、初始化，stmtlist}
            | RETURN Exp SEMI               { $$ = addToTree(_Stmt, Stmt__RETURN_Exp_SEMI, 3, $1, $2, $3); }       //return 表达式;
            | IF LP Exp RP Stmt             { $$ = addToTree(_Stmt, Stmt__IF_LP_Exp_RP_Stmt, 5, $1, $2, $3, $4, $5); }           //if(表达式) stmt
            | IF LP Exp RP Stmt ELSE Stmt   { $$ = addToTree(_Stmt, Stmt__IF_LP_Exp_RP_Stmt_ELSE_Stmt, 7, $1, $2, $3, $4, $5, $6, $7); }   //if(表达式) stmt else stmt
            | WHILE LP Exp RP Stmt          { $$ = addToTree(_Stmt, Stmt__WHILE_LP_Exp_RP_Stmt, 5, $1, $2, $3, $4, $5); }           //while(表达式) stmt
            | error SEMI                    { syntaxErrorCount++; }
            | error                         { syntaxErrorCount++; }
            ;


/* Local Definitions */
DefList     : Def DefList               { $$ = addToTree(_DefList, DefList__Def_DefList, 2, $1, $2); }            //int x, y=1; float m=1.0; 定义、初始化列表
            | /* empty */               { $$ = NULL; }                                      //
            ;

Def         : Specifier DecList SEMI    { $$ = addToTree(_Def, Def__Specifier_DecList_SEMI, 3, $1, $2, $3); }            //int x, y=1;
            | error SEMI                { syntaxErrorCount++; }                             //
            ;                   

DecList     : Dec                       { $$ = addToTree(_DecList, DecList__Dec, 1, $1); }                //
            | Dec COMMA DecList         { $$ = addToTree(_DecList, DecList__Dec_COMMA_DecList, 3, $1, $2, $3); }        //id1, id2=表达式, id3...
            ;

Dec         : VarDec                    { $$ = addToTree(_Dec, Dec__VarDec, 1, $1); }                    //id
            | VarDec ASSIGNOP Exp       { $$ = addToTree(_Dec, Dec__VarDec_ASSIGNOP_Exp, 3, $1, $2, $3); }            //id = 表达式
            ; 


/* Expressions */
Exp         : Exp ASSIGNOP Exp  { $$ = addToTree(_Exp, Exp__Exp_ASSIGNOP_Exp, 3, $1, $2, $3); }
            | Exp AND Exp       { $$ = addToTree(_Exp, Exp__Exp_AND_Exp, 3, $1, $2, $3); }
            | Exp OR Exp        { $$ = addToTree(_Exp, Exp__Exp_OR_Exp, 3, $1, $2, $3); }
            | Exp RELOP Exp     { $$ = addToTree(_Exp, Exp__Exp_RELOP_Exp, 3, $1, $2, $3); }
            | Exp PLUS Exp      { $$ = addToTree(_Exp, Exp__Exp_PLUS_Exp, 3, $1, $2, $3); }
            | Exp MINUS Exp     { $$ = addToTree(_Exp, Exp__Exp_MINUS_Exp, 3, $1, $2, $3); }
            | Exp DIV Exp       { $$ = addToTree(_Exp, Exp__Exp_DIV_Exp, 3, $1, $2, $3); }
            | Exp STAR Exp      { $$ = addToTree(_Exp, Exp__Exp_STAR_Exp, 3, $1, $2, $3); }
            | LP Exp RP         { $$ = addToTree(_Exp, Exp__LP_Exp_RP, 3, $1, $2, $3); }
            | MINUS Exp         { $$ = addToTree(_Exp, Exp__MINUS_Exp, 2, $1, $2); }
            | NOT Exp           { $$ = addToTree(_Exp, Exp__NOT_Exp, 2, $1, $2); }
            | ID LP Args RP     { $$ = addToTree(_Exp, Exp__ID_LP_Args_RP, 4, $1, $2, $3, $4); }
            | ID LP RP          { $$ = addToTree(_Exp, Exp__ID_LP_RP, 3, $1, $2, $3); }
            | Exp LB Exp RB     { $$ = addToTree(_Exp, Exp__Exp_LB_Exp_RB, 4, $1, $2, $3, $4); }
            | Exp DOT ID        { $$ = addToTree(_Exp, Exp__Exp_DOT_ID, 3, $1, $2, $3); }
            | ID                { $$ = addToTree(_Exp, Exp__ID, 1, $1); }
            | INT               { $$ = addToTree(_Exp, Exp__INT, 1, $1); }
            | FLOAT             { $$ = addToTree(_Exp, Exp__FLOAT, 1, $1); }
            | error             { syntaxErrorCount++; }
            ;

Args        : Exp COMMA Args    { $$ = addToTree(_Args, Args__Exp_COMMA_Args, 3, $1, $2, $3); }
            | Exp               { $$ = addToTree(_Args, Args__Exp, 1, $1); }
            ;




%%


int yyerror(char* msg){
    printf("Error type B at line %d, column %d: %s\n", yylineno, yycolumn, msg);
    return 0;
}

Node* addToTree(int nodeTag, int rule, int argc, ...){
    if(argc <= 0) return NULL;
    Node* res = newNode();
    res->tag = nodeTag;
    res->rule = rule;
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




