typedef enum _kind_of_node {
    _INT, _FLOAT, _ID, 
    _SEMI, _COMMA, _RELOP, _ASSIGNOP,
    _PLUS, _MINUS, _STAR, _DIV, _AND, _OR, _NOT, _DOT,
    _LP, _RP, _LB, _RB, _LC, _RC,
    _TYPE, _STRUCT, _RETURN, _IF, _ELSE, _WHILE,

    _Program, _ExtDefList, _ExtDef, _ExtDecList,
    _Specifier, _StructSpecifier, _OptTag, _Tag,
    _VarDec, _FunDec, _VarList, _ParamDec,
    _CompSt, _StmtList, _Stmt,
    _DefList, _Def, _DecList, _Dec,
    _Exp, _Args
}NodeTag;

/* Definition of syntax tree node */
typedef struct Node {
    int line, column;
    int isToken;
    NodeTag tag; //
    char* text; //原文
    union {
        float floatVal;
        int intVal;
    };


    struct Node* firstChild;
    struct Node* nextSibling;
}Node;


Node* newNode();
void freeNode(Node* nodePtr);
void traverse(Node* node);

extern char nodeTagStr[][25];
