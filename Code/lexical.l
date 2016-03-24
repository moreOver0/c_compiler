%option yylineno
%{
    #include "tree.h"
    #include "syntax.tab.h"

    extern char* yytext;
    extern int yylineno;
    int yycolumn = 1;
    int lexicalErrorCount = 0;

    #define YY_USER_ACTION \
        yylloc.first_line = yylloc.last_line = yylineno; \
        yylloc.first_column = yycolumn; \
        yylloc.last_column = yycolumn + yyleng - 1; \
        yycolumn += yyleng;


    int checkInt(int base, char* str);

    void setNode(int nodeTag, char* text);

%}


SEMI            [;]
COMMA           [,]
RELOP           (>=)|(<=)|(==)|(!=)|>|<
ASSIGNOP        [=]
PLUS            [+]
MINUS           [-]
STAR            [*]
DIV             [/]
AND             [&][&]
OR              [|][|]
DOT             [.]
NOT             [!]
LP              \(
RP              \)
LB              \[
RB              \]
LC              \{
RC              \}
TYPE            int|float
STRUCT          struct
RETURN          return
IF              if
ELSE            else
WHILE           while
SPACE           [ \t\r]
WS              {SPACE}+
LINE            [\n]

FLOAT           ([0-9]*[.][0-9]+f?)|([0-9]+[.][0-9]*f?) 
INT10           ([1-9][0-9]*)|[0]
INT8            [0][0-9]+
INT16           [0][Xx][0-9a-zA-Z]+
ID              [a-zA-Z_][a-zA-Z_0-9]*


%%
{LINE} { yycolumn = 1; }

{WS}    {}
{SEMI}  { setNode(_SEMI, yytext);return SEMI; }
{COMMA} { setNode(_COMMA, yytext);return COMMA; }

{LP} { setNode(_LP, yytext);return LP; }
{RP} { setNode(_RP, yytext);return RP; }
{LB} { setNode(_LB, yytext);return LB; }
{RB} { setNode(_RB, yytext);return RB; }
{LC} { setNode(_LC, yytext);return LC; }
{RC} { setNode(_RC, yytext);return RC; }

{RELOP}     { setNode(_RELOP, yytext);return RELOP; }
{ASSIGNOP}  { setNode(_ASSIGNOP, yytext);return ASSIGNOP; }

{PLUS}  { setNode(_PLUS, yytext);return PLUS; }
{MINUS} { setNode(_MINUS, yytext);return MINUS; }
{DIV}   { setNode(_DIV, yytext);return DIV; }
{STAR}  { setNode(_STAR, yytext);return STAR; }
{AND}   { setNode(_AND, yytext);return AND; }
{OR}    { setNode(_OR, yytext);return OR; }
{NOT}   { setNode(_NOT, yytext);return NOT; }
{DOT}   { setNode(_DOT, yytext);return DOT; }

{TYPE}      { setNode(_TYPE, yytext);return TYPE; }
{STRUCT}    { setNode(_STRUCT, yytext);return STRUCT; }
{RETURN}    { setNode(_RETURN, yytext);return RETURN; }
{IF}        { setNode(_IF, yytext);return IF; }
{ELSE}      { setNode(_ELSE, yytext);return ELSE; }
{WHILE}     { setNode(_WHILE, yytext);return WHILE; }

{ID} { setNode(_ID, yytext);return ID; }

{INT10} { setNode(_INT, yytext);return INT; }

{INT8} { 
    if(!checkInt(8, yytext)){
        lexicalErrorCount++;
        printf("Error type A at Line %d, Column %d: invalid digit in octal constant '%s' \n", yylloc.first_line, yylloc.first_column, yytext);
    }
    setNode(_INT, yytext);
    return INT;
}

{INT16} { 
    if(!checkInt(16, yytext)){
        lexicalErrorCount++;
        printf("Error type A at Line %d, Column %d: invalid letter in hexadecimal constant '%s' \n", yylloc.first_line, yylloc.first_column, yytext);
    }
    setNode(_INT, yytext);
    return INT;
}  

{FLOAT} { setNode(_FLOAT, yytext);return FLOAT; }

. {
    lexicalErrorCount++;
    printf("Error type A at Line %d, Column %d: Mysterious character '%s' \n", yylloc.first_line, yylloc.first_column, yytext);
}



%%

int checkInt(int base, char* str){
    int i = 0;
    if(8 == base){
        i += 1;
        while(str[i] != '\0'){
            if(!(str[i] >= '0' && str[i] <= '7'))return 0;
            i++;
        }
    }
    if(16 == base){
        i += 2;
        while(str[i] != '\0'){
            if(!((str[i] >= '0' && str[i] <= '9') || (str[i] >= 'a' && str[i] <= 'f') || (str[i] >= 'A' && str[i] <= 'F')))return 0;
            i++;
        }
    }
    return 1;
}

void setNode(int nodeTag, char* text){
    yylval.node = newNode();
    yylval.node->line = yylloc.first_line;
    yylval.node->column = yylloc.first_column;
    yylval.node->isToken = 1;
    yylval.node->tag = nodeTag;
    yylval.node->text = (char*)malloc(sizeof(char)*strlen(text)+1);
    memcpy(yylval.node->text, text, strlen(text)+1);

    if(_INT == nodeTag){
        int base = 10;
        int len = strlen(yylval.node->text);
        if(len >= 3 && (text[1]=='x'||text[1]=='X'))
            base = 16;
        else if(len >= 2 && text[0] == '0')
            base = 8;

        if(8 == base){
            yylval.node->intVal = (int)strtoul(yylval.node->text+1, 0, 8);
        }else if(10 == base){
            yylval.node->intVal = atoi(yylval.node->text);
        }else if(16 == base){
            yylval.node->intVal = (int)strtoul(yylval.node->text+2, 0, 16);
        } 
    }
    else if(_FLOAT == nodeTag){
        yylval.node->floatVal = atof(yylval.node->text);
    }
}

