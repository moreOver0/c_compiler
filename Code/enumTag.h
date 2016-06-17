#ifndef __ENUM_TAG_H
#define __ENUM_TAG_H


typedef enum _build_rule_of_syntax{
    Program__ExtDefList,
    ExtDefList__ExtDef_ExtDefList,
    ExtDef__Specifier_ExtDecList_SEMI,
    ExtDef__Specifier_SEMI,
    ExtDef__Specifier_FunDec_CompSt,
    ExtDecList__VarDec,
    ExtDecList__VarDec_COMMA_ExtDecList,
    Specifier__TYPE,
    Specifier__StructSpecifier,
    StructSpecifier__STRUCT_ID_LC_DefList_RC,
    StructSpecifier__STRUCT_LC_DefList_RC,
    StructSpecifier__STRUCT_ID,
    OptTag__ID,
    Tag__ID,
    VarDec__ID,
    VarDec__VarDec_LB_INT_RB,
    FunDec__ID_LP_VarList_RP,
    FunDec__ID_LP_RP,
    VarList__ParamDec_COMMA_VarList,
    VarList__ParamDec,
    ParamDec__Specifier_VarDec,
    CompSt__LC_DefList_StmtList_RC,
    StmtList__Stmt_StmtList,
    Stmt__Exp_SEMI,
    Stmt__CompSt,
    Stmt__RETURN_Exp_SEMI,
    Stmt__IF_LP_Exp_RP_Stmt,
    Stmt__IF_LP_Exp_RP_Stmt_ELSE_Stmt,
    Stmt__WHILE_LP_Exp_RP_Stmt,
    DefList__Def_DefList,
    Def__Specifier_DecList_SEMI,
    DecList__Dec,
    DecList__Dec_COMMA_DecList,
    Dec__VarDec,
    Dec__VarDec_ASSIGNOP_Exp,
    Exp__Exp_ASSIGNOP_Exp,
    Exp__Exp_AND_Exp,
    Exp__Exp_OR_Exp,
    Exp__Exp_RELOP_Exp,
    Exp__Exp_PLUS_Exp,
    Exp__Exp_MINUS_Exp,
    Exp__Exp_STAR_Exp,
    Exp__Exp_DIV_Exp,
    Exp__LP_Exp_RP,
    Exp__MINUS_Exp,
    Exp__NOT_Exp,
    Exp__ID_LP_Args_RP,
    Exp__ID_LP_RP,
    Exp__Exp_LB_Exp_RB,
    Exp__Exp_DOT_ID,
    Exp__ID,
    Exp__INT,
    Exp__FLOAT,
    Args__Exp_COMMA_Args,
    Args__Exp
}RuleTag;

typedef enum _type_of_semantic {
    type_general,
    type_unknown_type,
    define_int, define_float, define_struct,

    type_array,
    type_func
}TypeTag;

typedef enum _kind_of_node {
    _UNKNOWN,
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

typedef enum _kind_of_operand{
    function,
    variable, constant, address, 
    tempVariable, label
}OperandTag;

typedef enum _kind_of_intercode_op{
    IR_LABEL,
    IR_GOTO,
    IR_IF_GOTO,

    IR_FUNC,
    IR_PARAM,
    IR_DEC,
    IR_RETURN,
    IR_ARG,
    IR_CALL_ASSIGN,

    IR_ASSIGN,
    IR_ADD,
    IR_SUB,
    IR_MUL,
    IR_DIV,
    
    // IR_ADDR,         // x = &y
    IR_READ_FROM_ADDR,  // x = *y
    IR_WRITE_TO_ADDR,   // *x = y

    IR_READ,
    IR_WRITE
}InterCodeTag;


#endif
