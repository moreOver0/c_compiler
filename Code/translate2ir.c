#include "common.h"


typedef InterCodeNode* (*translaterPoint)(Node*);
static translaterPoint translater[];
bool anyStruct;

static InterCodeNode* program__extdeflist(Node* node);
static InterCodeNode* extdeflist__extdef_extdeflist(Node* node);
static InterCodeNode* extdef__specifier_extdeclist_semi(Node* node);
static InterCodeNode* extdef__specifier_semi(Node* node);
static InterCodeNode* extdef__specifier_fundec_compst(Node* node);
static InterCodeNode* extdeclist__vardec(Node* node);
static InterCodeNode* extdeclist__vardec_comma_extdeclist(Node* node);
static InterCodeNode* vardec__id(Node* node);
static InterCodeNode* vardec__vardec_lb_int_rb(Node* node);
static InterCodeNode* fundec__id_lp_varlist_rp(Node* snode);
static InterCodeNode* fundec__id_lp_rp(Node* node);
static InterCodeNode* compst__lc_deflist_stmtlist_rc(Node* node);
static InterCodeNode* stmtlist__stmt_stmtlist(Node* node);
static InterCodeNode* stmt__exp_semi(Node* node);
static InterCodeNode* stmt__compst(Node* node);
static InterCodeNode* stmt__return_exp_semi(Node* node);
static InterCodeNode* stmt__if_lp_exp_rp_stmt(Node* node);
static InterCodeNode* stmt__if_lp_exp_rp_stmt_else_stmt(Node* node);
static InterCodeNode* stmt__while_lp_exp_rp_stmt(Node* node);
static InterCodeNode* deflist__def_deflist(Node* node);
static InterCodeNode* def__specifier_declist_semi(Node* node);
static InterCodeNode* declist__dec(Node* node);
static InterCodeNode* declist__dec_comma_declist(Node* node);
static InterCodeNode* dec__vardec(Node* node);
static InterCodeNode* dec__vardec_assignop_exp(Node* node);
static InterCodeNode* exp__exp_assignop_exp(Node* node);
static InterCodeNode* exp__exp_op_exp(Node* node);
static InterCodeNode* exp__lp_exp_rp(Node* node);
static InterCodeNode* exp__minus_exp(Node* node);
static InterCodeNode* exp__not_exp(Node* node);
static InterCodeNode* exp__id_lp_args_rp(Node* node);
static InterCodeNode* exp__id_lp_rp(Node* node);
static InterCodeNode* exp__exp_lb_exp_rb(Node* node);
static InterCodeNode* exp__exp_dot_id(Node* node);
static InterCodeNode* exp__id(Node* node);
static InterCodeNode* exp__int(Node* node);
static InterCodeNode* exp__float(Node* node);
static InterCodeNode* args__exp_comma_args(Node* node);
static InterCodeNode* args__exp(Node* node);
static InterCodeNode* translate_Cond(Node* exp, Operand* ltrue, Operand* lfalse);


static char handlestr[][100] = {
    [Program__ExtDefList] = "program__extdeflist",
    [ExtDefList__ExtDef_ExtDefList] = "extdeflist__extdef_extdeflist",
    [ExtDef__Specifier_ExtDecList_SEMI] = "extdef__specifier_extdeclist_semi",
    [ExtDef__Specifier_SEMI] = "extdef__specifier_semi",
    [ExtDef__Specifier_FunDec_CompSt] = "extdef__specifier_fundec_compst",
    [ExtDecList__VarDec] = "extdeclist__vardec",
    [ExtDecList__VarDec_COMMA_ExtDecList] = "extdeclist__vardec_comma_extdeclist",
    [Specifier__TYPE] = "specifier__type",
    [Specifier__StructSpecifier] = "specifier__structspecifier",
    [StructSpecifier__STRUCT_ID_LC_DefList_RC] = "structspecifier__struct_id_lc_deflist_rc",
    [StructSpecifier__STRUCT_LC_DefList_RC] = "structspecifier__struct_lc_deflist_rc",
    [StructSpecifier__STRUCT_ID] = "structspecifier__struct_id",
    [VarDec__ID] = "vardec__id",
    [VarDec__VarDec_LB_INT_RB] = "vardec__vardec_lb_int_rb",
    [FunDec__ID_LP_VarList_RP] = "fundec__id_lp_varlist_rp",
    [FunDec__ID_LP_RP] = "fundec__id_lp_rp",
    [VarList__ParamDec_COMMA_VarList] = "varlist__paramdec_comma_varlist",
    [VarList__ParamDec] = "varlist__paramdec",
    [ParamDec__Specifier_VarDec] = "paramdec__specifier_vardec",
    [CompSt__LC_DefList_StmtList_RC] = "compst__lc_deflist_stmtlist_rc",
    [StmtList__Stmt_StmtList] = "stmtlist__stmt_stmtlist",
    [Stmt__Exp_SEMI] = "stmt__exp_semi",
    [Stmt__CompSt] = "stmt__compst",
    [Stmt__RETURN_Exp_SEMI] = "stmt__return_exp_semi",
    [Stmt__IF_LP_Exp_RP_Stmt] = "stmt__if_lp_exp_rp_stmt",
    [Stmt__IF_LP_Exp_RP_Stmt_ELSE_Stmt] = "stmt__if_lp_exp_rp_stmt_else_stmt",
    [Stmt__WHILE_LP_Exp_RP_Stmt] = "stmt__while_lp_exp_rp_stmt",
    [DefList__Def_DefList] = "deflist__def_deflist",
    [Def__Specifier_DecList_SEMI] = "def__specifier_declist_semi",
    [DecList__Dec] = "declist__dec",
    [DecList__Dec_COMMA_DecList] = "declist__dec_comma_declist",
    [Dec__VarDec] = "dec__vardec",
    [Dec__VarDec_ASSIGNOP_Exp] = "dec__vardec_assignop_exp",
    [Exp__Exp_ASSIGNOP_Exp] = "exp__exp_assignop_exp",
    [Exp__Exp_AND_Exp] = "exp__exp_op_exp",
    [Exp__Exp_OR_Exp] = "exp__exp_op_exp",
    [Exp__Exp_RELOP_Exp] = "exp__exp_op_exp",
    [Exp__Exp_PLUS_Exp] = "exp__exp_op_exp",
    [Exp__Exp_MINUS_Exp] = "exp__exp_op_exp",
    [Exp__Exp_STAR_Exp] = "exp__exp_op_exp",
    [Exp__Exp_DIV_Exp] = "exp__exp_op_exp",
    [Exp__LP_Exp_RP] = "exp__lp_exp_rp",
    [Exp__MINUS_Exp] = "exp__minus_exp",
    [Exp__NOT_Exp] = "exp__not_exp",
    [Exp__ID_LP_Args_RP] = "exp__id_lp_args_rp",
    [Exp__ID_LP_RP] = "exp__id_lp_rp",
    [Exp__Exp_LB_Exp_RB] = "exp__exp_lb_exp_rb",
    [Exp__Exp_DOT_ID] = "exp__exp_dot_id",
    [Exp__ID] = "exp__id",
    [Exp__INT] = "exp__int",
    [Exp__FLOAT] = "exp__float",
    [Args__Exp_COMMA_Args] = "args__exp_comma_args",
    [Args__Exp] = "args__exp"
};


#define translate(x)  (((x) == NULL) ? NULL : translater[(x)->rule](x))

#define getChild(x) ((x)->firstChild)
#define getChild2(x) ((x)->firstChild->nextSibling)
#define getChild3(x) ((x)->firstChild->nextSibling->nextSibling)
#define getChild4(x) ((x)->firstChild->nextSibling->nextSibling->nextSibling)
#define getChild5(x) ((x)->firstChild->nextSibling->nextSibling->nextSibling->nextSibling)


typedef InterCodeNode* Code;


void getIR(char* fileName){
    anyStruct = false;
    irHead = translate(root);
    if(anyStruct){
        printf("Cannot translate: Code contains variables or parameters of structure type.\n");
    }
    else{
        FILE* file = stdout;
        traverseIR(file);
        // fclose(file);
    }
}

static InterCodeNode* no_feedback(Node* n){
    return NULL;
}

static InterCodeNode* program__extdeflist(Node* n){
#ifdef _TRA_DEBUG
    printf("%-3d  %s\n", n->line, handlestr[n->rule]);
#endif


    return translate(getChild(n));
}

static InterCodeNode* extdeflist__extdef_extdeflist(Node* n){
#ifdef _TRA_DEBUG
    printf("%-3d  %s\n", n->line, handlestr[n->rule]);
#endif


    return concat(translate(getChild(n)), translate(getChild2(n)));
}

static InterCodeNode* extdef__specifier_fundec_compst(Node* n){
#ifdef _TRA_DEBUG
    printf("%-3d  %s\n", n->line, handlestr[n->rule]);
#endif


    return concat(translate(getChild2(n)), translate(getChild3(n)));
}

static Type* currentFunction = NULL;

static InterCodeNode* fundec__id_lp_varlist_rp(Node* n){
#ifdef _TRA_DEBUG
    printf("%-3d  %s\n", n->line, handlestr[n->rule]);
#endif


    Type* func = n->type;
    currentFunction = func;
    currentFunction->function.dataSize = -4;
    Code code = newInterCodeNodeWithInfo(IR_FUNC, newOperandFunc(func), NULL, NULL);
    Type** paraList = func->function.argv;
    int len = func->function.argc;
    int paraSize = 0;
    for(int i = 0; i < len; ++i){
        //输出时注意check类型是否是结构体或其他不合法类型
        paraSize += computeSizeByByte(paraList[i]);
        paraList[i]->offset = paraSize;
        Code tmp = newInterCodeNodeWithInfo(IR_PARAM, newOperandVar(paraList[i]), NULL, NULL);
        code = concat(code, tmp);
    }
    return code;
}

static InterCodeNode* fundec__id_lp_rp(Node* n){
#ifdef _TRA_DEBUG
    printf("%-3d  %s\n", n->line, handlestr[n->rule]);
#endif


    Type* func = n->type;
    currentFunction = func;
    currentFunction->function.dataSize = -4;
    Code code = newInterCodeNodeWithInfo(IR_FUNC, newOperandFunc(func), NULL, NULL);
    return code;
}

static InterCodeNode* compst__lc_deflist_stmtlist_rc(Node* n){
#ifdef _TRA_DEBUG
    printf("%-3d  %s\n", n->line, handlestr[n->rule]);
#endif


    Node* list = getChild2(n);
    Code code = NULL;
    if(list->tag == _DefList){
        code = translate(list);
        list = list->nextSibling;
    }
    if(list->tag == _StmtList){
        code = concat(code, translate(list));
    }
    return code;
}

static InterCodeNode* deflist__def_deflist(Node* n){
#ifdef _TRA_DEBUG
    printf("%-3d  %s\n", n->line, handlestr[n->rule]);
#endif


    return concat(translate(getChild(n)), translate(getChild2(n)));
}

static InterCodeNode* def__specifier_declist_semi(Node* n){
#ifdef _TRA_DEBUG
    printf("%-3d  %s\n", n->line, handlestr[n->rule]);
#endif


    if(getChild2(n)->tag == _DecList)
        return translate(getChild2(n));
    return NULL;
}

static InterCodeNode* declist__dec(Node* n){
#ifdef _TRA_DEBUG
    printf("%-3d  %s\n", n->line, handlestr[n->rule]);
#endif


    return translate(getChild(n));
}

static InterCodeNode* declist__dec_comma_declist(Node* n){
#ifdef _TRA_DEBUG
    printf("%-3d  %s\n", n->line, handlestr[n->rule]);
#endif


    return concat(translate(getChild(n)), translate(getChild3(n)));
}

static InterCodeNode* dec__vardec(Node* n){
#ifdef _TRA_DEBUG
    printf("%-3d  %s\n", n->line, handlestr[n->rule]);
    showType(n->type);
#endif


    Type* var = n->type;
    Code code = NULL;
    int size = computeSizeByByte(var);
    if(var->myType->typeTag == type_array || var->myType->typeTag == define_struct){
        var->offset = currentFunction->function.dataSize - size + 4;
    }else{
        var->offset = currentFunction->function.dataSize;
    }
    currentFunction->function.dataSize -= size;
    if(!(var->myType == BASIC_INT || var->myType == BASIC_FLOAT)){
        code = newInterCodeNodeWithInfo(IR_DEC, newOperandVar(var), newOperandConstant(size), NULL);
    }
    return code;
}

static InterCodeNode* dec__vardec_assignop_exp(Node* n){
#ifdef _TRA_DEBUG
    printf("%-3d  %s\n", n->line, handlestr[n->rule]);
    showType(n->type);
#endif


    // vardec must not be array or struct
    Type* var = n->type;
    int size = computeSizeByByte(var);
    var->offset = currentFunction->function.dataSize;
    currentFunction->function.dataSize -= size;
    assert(var->myType == BASIC_INT || var->myType == BASIC_FLOAT);
    Node* exp = getChild3(n);
    exp->place = newTemp();
    Code code = translate(exp);
    Operand* value = exp->place;
    code = concat(code, newInterCodeNodeWithInfo(IR_ASSIGN, newOperandVar(n->type), value, NULL));
    return code;
}

static InterCodeNode* exp__exp_assignop_exp(Node* n){
#ifdef _TRA_DEBUG
    printf("%-3d  %s\n", n->line, handlestr[n->rule]);
#endif


    Operand* var = newTemp();
    Node* exp1 = getChild(n);
    Node* exp2 = getChild3(n);
    exp1->arrayAssign = true;
    exp1->place = var;
    Code code = translate(exp1);
    var = exp1->place;

    Operand* value = newTemp();
    exp2->place = value;
    code = concat(code, translate(exp2));
    value = exp2->place;
    if(var->tag == tempVariable){ // exp1 is array
        code = concat(code, newInterCodeNodeWithInfo(IR_WRITE_TO_ADDR, var, value, NULL));
        if(n->place != NULL)
            code = concat(code, newInterCodeNodeWithInfo(IR_ASSIGN, n->place, value, NULL));
    }else if(var->tag == variable){ // exp1 is id
        code = concat(code, newInterCodeNodeWithInfo(IR_ASSIGN, var, value, NULL));
        if(n->place != NULL)
            code = concat(code, newInterCodeNodeWithInfo(IR_ASSIGN, n->place, value, NULL));
    }   
    return code;
}

static InterCodeTag getOp(Node* op){
    if(op->tag == _PLUS) return IR_ADD;
    if(op->tag == _MINUS) return IR_SUB;
    if(op->tag == _DIV) return IR_DIV;
    if(op->tag == _STAR) return IR_MUL;
    return -1;
}

static InterCodeNode* exp__exp_op_exp(Node* n){
#ifdef _TRA_DEBUG
    printf("%-3d  %s\n", n->line, handlestr[n->rule]);
#endif


    if(n->place == NULL) assert(0);//return NULL;
    Operand* t1 = newTemp();
    Operand* t2 = newTemp();
    Node* exp1 = getChild(n); exp1->place = t1;
    Node* exp2 = getChild3(n); exp2->place = t2;
    Code c1 = translate(exp1); t1 = exp1->place;
    Code c2 = translate(exp2); t2 = exp2->place;
    Node* op = getChild2(n);

    Code code = concat(c1, c2);
    code = concat(code, newInterCodeNodeWithInfo(getOp(op), n->place, t1, t2));
    return code;
}

static InterCodeNode* exp__lp_exp_rp(Node* n){
#ifdef _TRA_DEBUG
    printf("%-3d  %s\n", n->line, handlestr[n->rule]);
#endif


    Node* exp = getChild2(n);
    exp->place = n->place;
    return translate(exp);
}

static InterCodeNode* exp__minus_exp(Node* n){
#ifdef _TRA_DEBUG
    printf("%-3d  %s\n", n->line, handlestr[n->rule]);
#endif


    if(n->place == NULL) assert(0);//return NULL;
    Operand* t1 = newTemp();
    Node* exp1 = getChild2(n); exp1->place = t1;
    Code c1 = translate(exp1); t1 = exp1->place;

    Code code = c1;
    code = concat(code, newInterCodeNodeWithInfo(IR_SUB, n->place, newOperandConstant(0), t1));
    return code;
}

static InterCodeNode* exp__cond(Node* n){
#ifdef _TRA_DEBUG
    printf("%-3d  %s\n", n->line, handlestr[n->rule]);
#endif


    if(n->place == NULL) assert(0);//return NULL;
    Operand* label1 = newLabel();
    Operand* label2 = newLabel();
    Code code0 = newInterCodeNodeWithInfo(IR_ASSIGN, n->place, newOperandConstant(0), NULL);
    Code code1 = translate_Cond(n, label1, label2);
    Code code2 = concat(newInterCodeNodeWithInfo(IR_LABEL, label1, NULL, NULL), newInterCodeNodeWithInfo(IR_ASSIGN, n->place, newOperandConstant(1), NULL));
    Code code = concat(code0, code1);
    code = concat(code, code2);
    code = concat(code, newInterCodeNodeWithInfo(IR_LABEL, label2, NULL, NULL));
    return code;
}

static InterCodeNode* exp__id_lp_rp(Node* n){
#ifdef _TRA_DEBUG
    printf("%-3d  %s\n", n->line, handlestr[n->rule]);
#endif


    char* funcName = getChild(n)->text;
    if(strcmp(funcName, "read") == 0) {
        return newInterCodeNodeWithInfo(IR_READ, n->place, NULL, NULL);
    }    
    return newInterCodeNodeWithInfo(IR_CALL_ASSIGN, n->place, newOperandFunc(findSymbolTable(funcName)), NULL);
}

static InterCodeNode* exp__id_lp_args_rp(Node* n){
#ifdef _TRA_DEBUG
    printf("%-3d  %s\n", n->line, handlestr[n->rule]);
#endif


    char* funcName = getChild(n)->text;
    Node* args = getChild3(n);
    Operand** argList = (Operand**)malloc(sizeof(Operand*)*12);
    args->argList = argList;
    args->argCount = 0;
    Code code = translate(args);
    if(strcmp(funcName, "write") == 0){
        return concat(code, newInterCodeNodeWithInfo(IR_WRITE, argList[args->argCount-1], NULL, NULL));
    }
    for(int i = args->argCount-1; i >= 0; --i){
        code = concat(code, newInterCodeNodeWithInfo(IR_ARG, argList[i], NULL, NULL));
    }
    code = concat(code, newInterCodeNodeWithInfo(IR_CALL_ASSIGN, n->place, newOperandFunc(findSymbolTable(funcName)), NULL));
    return code;
}   

static InterCodeNode* args__exp_comma_args(Node* n){
#ifdef _TRA_DEBUG
    printf("%-3d  %s\n", n->line, handlestr[n->rule]);
#endif


    Operand* t1 = newTemp();
    Node* exp = getChild(n);
    exp->place = t1;
    Code code = translate(exp);
    t1 = exp->place;
    n->argList[n->argCount] = t1;
    n->argCount++;

    Node* args = getChild3(n);
    args->argList = n->argList;
    args->argCount = n->argCount;
    code = concat(code, translate(args));
    n->argCount = args->argCount;
    return code;
}

static InterCodeNode* args__exp(Node* n){
#ifdef _TRA_DEBUG
    printf("%-3d  %s\n", n->line, handlestr[n->rule]);
#endif


    Operand* t1 = newTemp();
    Node* exp = getChild(n);
    exp->place = t1;
    Code code = translate(exp);
    t1 = exp->place;
    n->argList[n->argCount] = t1;
    n->argCount++;
    return code;
}



static bool checkPointer(Type* a){
    Type** list = currentFunction->function.argv;
    int len = currentFunction->function.argc;
    for(int i = 0; i < len; ++i){
        if(a == list[i])
            return true;
    }
    return false;
}

static Operand* getVarOperand(Type* a){
    if(checkPointer(a)){
        return newOperandVar(a);
    }else{
        return newOperandAddr(newOperandVar(a));
    }
}


static InterCodeNode* exp__exp_lb_exp_rb(Node* n){
#ifdef _TRA_DEBUG
    printf("%-3d  %s\n", n->line, handlestr[n->rule]);
#endif


    if(n->place == NULL) assert(0);//return NULL;
    n->arrayLevel++;
    Node* exp = getChild(n);
    Code code = NULL;
#ifdef _TRA_DEBUG
printf("what the elment\n");
showType(exp->type->myType);
#endif
    if(exp->rule == Exp__ID){
        // printf("di yi wei\n");

        Operand* tmp = newTemp();
        exp->place = tmp; // useless tmp
        translate(exp);
        n->arraySymbol = exp->arraySymbol;
        Operand* t1 = newTemp();
        Node* index = getChild3(n);
        index->place = t1;
        code = translate(index); t1 = index->place;

        if(n->arrayLevel == 1){
            Operand* offset = newTemp();
            code = concat(code, newInterCodeNodeWithInfo(IR_MUL, offset, t1, newOperandConstant(computeSizeByByte(n->type))));
            if(n->arrayAssign){
                // printf("arrayAssign\n");
                code = concat(code, newInterCodeNodeWithInfo(IR_ADD, n->place, getVarOperand(n->arraySymbol), offset));
            }else{
                // printf("not arrayAssign\n");
                Operand* tmpaddr = newTemp();
                code = concat(code, newInterCodeNodeWithInfo(IR_ADD, tmpaddr, getVarOperand(n->arraySymbol), offset));
                code = concat(code, newInterCodeNodeWithInfo(IR_READ_FROM_ADDR, n->place, tmpaddr, NULL));
            }
        }else{
            // printf("arrayLevel!=1 && di yi wei\n");
            code = concat(code, newInterCodeNodeWithInfo(IR_MUL, n->place, t1, newOperandConstant(computeSizeByByte(n->type))));
        }

    }
    else{
        // printf("not di yi wei\n");

        Operand* t1 = newTemp();
        exp->place = t1;
        exp->arrayLevel = n->arrayLevel;
        code = translate(exp); t1 = exp->place;
        n->arraySymbol = exp->arraySymbol;

        Operand* t2 = newTemp();
        Node* index = getChild3(n);
        index->place = t2;
        code = concat(code, translate(index)); t2 = index->place;
        Operand* t3 = newTemp();
        code = concat(code, newInterCodeNodeWithInfo(IR_MUL, t3, t2, newOperandConstant(computeSizeByByte(n->type))));


        if(n->arrayLevel == 1){
            Operand* t4 = newTemp();
            code = concat(code, newInterCodeNodeWithInfo(IR_ADD, t4, t1, t3));
            if(n->arrayAssign){
                code = concat(code, newInterCodeNodeWithInfo(IR_ADD, n->place, getVarOperand(n->arraySymbol), t4));
            }else{
                Operand* tmpaddr = newTemp();
                code = concat(code, newInterCodeNodeWithInfo(IR_ADD, tmpaddr, getVarOperand(n->arraySymbol), t4));
                code = concat(code, newInterCodeNodeWithInfo(IR_READ_FROM_ADDR, n->place, tmpaddr, NULL));
            }
        }else{
            code = concat(code, newInterCodeNodeWithInfo(IR_ADD, n->place, t1, t3));
        }
    }

    return code;
}

static InterCodeNode* exp__id(Node* n){
#ifdef _TRA_DEBUG
    printf("%-3d  %s\n", n->line, handlestr[n->rule]);
    showType(n->type);
#endif


    if(n->place == NULL) assert(0);//return NULL;
    Type* t = n->type;
    if(t->myType == BASIC_INT || t->myType == BASIC_FLOAT){
        n->place = newOperandVar(t);
    }else if(t->myType->typeTag == define_struct){
        anyStruct = true;
        // assert(0);
    }else if(t->myType->typeTag == type_array){
        n->arraySymbol = t;
        // n->place = newOperandAddr(newOperandVar(t));
        n->place = getVarOperand(t);
    }
    return NULL;
}

static InterCodeNode* exp__exp_dot_id(Node* n){
#ifdef _TRA_DEBUG
    printf("%-3d  %s\n", n->line, handlestr[n->rule]);
#endif


    anyStruct = true;
    return NULL;
}

static InterCodeNode* exp__int(Node* n){
#ifdef _TRA_DEBUG
    printf("%-3d  %s\n", n->line, handlestr[n->rule]);
#endif


    if(n->place == NULL) assert(0);//return NULL;
    n->place = newOperandConstant(getChild(n)->intVal);
    return NULL;
    // Node* c = getChild(n);
    // return newInterCodeNodeWithInfo(IR_ASSIGN, n->place, newOperandConstant(c->intVal), NULL);
}

static InterCodeNode* exp__float(Node* n){
#ifdef _TRA_DEBUG
    printf("%-3d  %s\n", n->line, handlestr[n->rule]);
#endif


    assert(0);
}

static InterCodeNode* stmtlist__stmt_stmtlist(Node* n){
#ifdef _TRA_DEBUG
    printf("%-3d  %s\n", n->line, handlestr[n->rule]);
#endif


    return concat(translate(getChild(n)), translate(getChild2(n)));
}

static InterCodeNode* stmt__exp_semi(Node* n){
#ifdef _TRA_DEBUG
    printf("%-3d  %s\n", n->line, handlestr[n->rule]);
#endif


    return translate(getChild(n));  // exp->place NULL 
}

static InterCodeNode* stmt__compst(Node* n){
#ifdef _TRA_DEBUG
    printf("%-3d  %s\n", n->line, handlestr[n->rule]);
#endif


    return translate(getChild(n));
}

static InterCodeNode* stmt__return_exp_semi(Node* n){
#ifdef _TRA_DEBUG
    printf("%-3d  %s\n", n->line, handlestr[n->rule]);
#endif


    Operand* t1 = newTemp();
    getChild2(n)->place = t1;
    Code code = translate(getChild2(n));
    t1 = getChild2(n)->place;
    code = concat(code, newInterCodeNodeWithInfo(IR_RETURN, t1, NULL, NULL));
    return code;
}

static InterCodeNode* stmt__if_lp_exp_rp_stmt(Node* n){
#ifdef _TRA_DEBUG
    printf("%-3d  %s\n", n->line, handlestr[n->rule]);
#endif


    Operand* label1 = newLabel();
    Operand* label2 = newLabel();
    Code code1 = translate_Cond(getChild3(n), label1, label2);
    Code code2 = translate(getChild5(n));
    Code code = concat(code1, newInterCodeNodeWithInfo(IR_LABEL, label1, NULL, NULL));
    code = concat(code, code2);
    code = concat(code, newInterCodeNodeWithInfo(IR_LABEL, label2, NULL, NULL));
    return code;
}

static InterCodeNode* stmt__if_lp_exp_rp_stmt_else_stmt(Node* n){
#ifdef _TRA_DEBUG
    printf("%-3d  %s\n", n->line, handlestr[n->rule]);
#endif


    Operand* label1 = newLabel();
    Operand* label2 = newLabel();
    Operand* label3 = newLabel();
    Code code1 = translate_Cond(getChild3(n), label1, label2);
    Code code2 = translate(getChild5(n));
    Code code3 = translate(getChild5(n)->nextSibling->nextSibling);
    Code code = concat(code1, newInterCodeNodeWithInfo(IR_LABEL, label1, NULL, NULL));
    code = concat(code, code2);
    code = concat(code, newInterCodeNodeWithInfo(IR_GOTO, label3, NULL, NULL));
    code = concat(code, newInterCodeNodeWithInfo(IR_LABEL, label2, NULL, NULL));
    code = concat(code, code3);
    return concat(code, newInterCodeNodeWithInfo(IR_LABEL, label3, NULL, NULL));
}

static InterCodeNode* stmt__while_lp_exp_rp_stmt(Node* n){
#ifdef _TRA_DEBUG
    printf("%-3d  %s\n", n->line, handlestr[n->rule]);
#endif


    Operand* label1 = newLabel();
    Operand* label2 = newLabel();
    Operand* label3 = newLabel();
    Code code1 = translate_Cond(getChild3(n), label2, label3);
    Code code2 = translate(getChild5(n));
    Code code = concat(newInterCodeNodeWithInfo(IR_LABEL, label1, NULL, NULL), code1);
    code = concat(code, newInterCodeNodeWithInfo(IR_LABEL, label2, NULL, NULL));
    code = concat(code, code2);
    code = concat(code, newInterCodeNodeWithInfo(IR_GOTO, label1, NULL, NULL));
    code = concat(code, newInterCodeNodeWithInfo(IR_LABEL, label3, NULL, NULL));
    return code;
}

static InterCodeNode* translate_Cond(Node* exp, Operand* ltrue, Operand* lfalse){
#ifdef _TRA_DEBUG
    printf("%-3d  translate_Cond %s\n", exp->line, handlestr[exp->rule]);
#endif
    // printf("%d  from translate_Cond\n", exp->line);

    RuleTag tag = exp->rule;
    if(Exp__Exp_RELOP_Exp == tag){ // handle array
        // printf("Exp__Exp_RELOP_Exp\n");
        Operand* t1 = newTemp();
        Operand* t2 = newTemp();
        Node* exp1 = getChild(exp); exp1->place = t1;
        Node* exp2 = getChild3(exp); exp2->place = t2;
        Code code1 = translate(exp1); t1 = exp1->place;
        Code code2 = translate(exp2); t2 = exp2->place;
        Code code = concat(code1, code2);

        Node* relop = getChild2(exp);
        char* op = relop->text;
        InterCode* ic = newInterCode(IR_IF_GOTO, ltrue, t1, t2);
        ic->relop = op;
        code = concat(code, newInterCodeNodeWithIC(ic));
        code = concat(code, newInterCodeNodeWithInfo(IR_GOTO, lfalse, NULL, NULL));
        return code;

    }else if(Exp__NOT_Exp == tag){
        // printf("Exp__NOT_Exp\n");
        return translate_Cond(exp, lfalse, ltrue);

    }else if(Exp__Exp_AND_Exp == tag){
        // printf("Exp__Exp_AND_Exp\n");
        Operand* label1 = newLabel();
        Code code1 = translate_Cond(getChild(exp), label1, lfalse);
        Code code2 = translate_Cond(getChild3(exp), ltrue, lfalse);
        Code code = concat(code1, newInterCodeNodeWithInfo(IR_LABEL, label1, NULL, NULL));
        code = concat(code, code2);
        return code;

    }else if(Exp__Exp_OR_Exp == tag){
        // printf("Exp__Exp_OR_Exp\n");
        Operand* label1 = newLabel();
        Code code1 = translate_Cond(getChild(exp), ltrue, label1);
        Code code2 = translate_Cond(getChild3(exp), ltrue, lfalse);
        Code code = concat(code1, newInterCodeNodeWithInfo(IR_LABEL, label1, NULL, NULL));
        code = concat(code, code2);
        return code;

    }else{ 
        // printf("other case\n");
        Operand* t1 = newTemp();
        exp->place = t1;
        Code code = translate(exp);
        t1 = exp->place;

        char* op = "!=";
        InterCode* ic = newInterCode(IR_IF_GOTO, ltrue, t1, newOperandConstant(0));
        ic->relop = op;
        code = concat(code, newInterCodeNodeWithIC(ic));
        code = concat(code, newInterCodeNodeWithInfo(IR_GOTO, lfalse, NULL, NULL));
        return code;
    }
}


static translaterPoint translater[] = {
    [Program__ExtDefList] = program__extdeflist,
    [ExtDefList__ExtDef_ExtDefList] = extdeflist__extdef_extdeflist,
    [ExtDef__Specifier_ExtDecList_SEMI] = no_feedback,
    [ExtDef__Specifier_SEMI] = no_feedback,
    [ExtDef__Specifier_FunDec_CompSt] = extdef__specifier_fundec_compst,
    [FunDec__ID_LP_VarList_RP] = fundec__id_lp_varlist_rp,
    [FunDec__ID_LP_RP] = fundec__id_lp_rp,
    [CompSt__LC_DefList_StmtList_RC] = compst__lc_deflist_stmtlist_rc,
    [StmtList__Stmt_StmtList] = stmtlist__stmt_stmtlist,
    [Stmt__Exp_SEMI] = stmt__exp_semi,
    [Stmt__CompSt] = stmt__compst,
    [Stmt__RETURN_Exp_SEMI] = stmt__return_exp_semi,
    [Stmt__IF_LP_Exp_RP_Stmt] = stmt__if_lp_exp_rp_stmt,
    [Stmt__IF_LP_Exp_RP_Stmt_ELSE_Stmt] = stmt__if_lp_exp_rp_stmt_else_stmt,
    [Stmt__WHILE_LP_Exp_RP_Stmt] = stmt__while_lp_exp_rp_stmt,
    [DefList__Def_DefList] = deflist__def_deflist,
    [Def__Specifier_DecList_SEMI] = def__specifier_declist_semi,
    [DecList__Dec] = declist__dec,
    [DecList__Dec_COMMA_DecList] = declist__dec_comma_declist,
    [Dec__VarDec] = dec__vardec,
    [Dec__VarDec_ASSIGNOP_Exp] = dec__vardec_assignop_exp,
    [Exp__Exp_ASSIGNOP_Exp] = exp__exp_assignop_exp,
    [Exp__Exp_AND_Exp] = exp__cond,
    [Exp__Exp_OR_Exp] = exp__cond,
    [Exp__Exp_RELOP_Exp] = exp__cond,
    [Exp__NOT_Exp] = exp__cond,
    [Exp__Exp_PLUS_Exp] = exp__exp_op_exp,
    [Exp__Exp_MINUS_Exp] = exp__exp_op_exp,
    [Exp__Exp_STAR_Exp] = exp__exp_op_exp,
    [Exp__Exp_DIV_Exp] = exp__exp_op_exp,
    [Exp__LP_Exp_RP] = exp__lp_exp_rp,
    [Exp__MINUS_Exp] = exp__minus_exp,
    [Exp__ID_LP_Args_RP] = exp__id_lp_args_rp,
    [Exp__ID_LP_RP] = exp__id_lp_rp,
    [Exp__Exp_LB_Exp_RB] = exp__exp_lb_exp_rb,
    [Exp__Exp_DOT_ID] = exp__exp_dot_id,
    [Exp__ID] = exp__id,
    [Exp__INT] = exp__int,
    [Exp__FLOAT] = exp__float,
    [Args__Exp_COMMA_Args] = args__exp_comma_args,
    [Args__Exp] = args__exp,
};



#undef getChild
#undef translate
