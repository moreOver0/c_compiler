#include "common.h"

int semanticErrorCount;
static bool inStruct;

static void program__extdeflist(Node* node);
static void extdeflist__extdef_extdeflist(Node* node);
static void extdef__specifier_extdeclist_semi(Node* node);
static void extdef__specifier_semi(Node* node);
static void extdef__specifier_fundec_compst(Node* node);
static void extdeclist__vardec(Node* node);
static void extdeclist__vardec_comma_extdeclist(Node* node);
static void specifier__type(Node* node);
static void specifier__structspecifier(Node* node);
static void structspecifier__struct_id_lc_deflist_rc(Node* node);
static void structspecifier__struct_lc_deflist_rc(Node* node);
static void structspecifier__struct_id(Node* node);
static void opttag__id(Node* node);
static void tag__id(Node* node);
static void vardec__id(Node* node);
static void vardec__vardec_lb_int_rb(Node* node);
static void fundec__id_lp_varlist_rp(Node* node);
static void fundec__id_lp_rp(Node* node);
static void varlist__paramdec_comma_varlist(Node* node);
static void varlist__paramdec(Node* node);
static void paramdec__specifier_vardec(Node* node);
static void compst__lc_deflist_stmtlist_rc(Node* node);
static void stmtlist__stmt_stmtlist(Node* node);
static void stmt__exp_semi(Node* node);
static void stmt__compst(Node* node);
static void stmt__return_exp_semi(Node* node);
static void stmt__if_lp_exp_rp_stmt(Node* node);
static void stmt__if_lp_exp_rp_stmt_else_stmt(Node* node);
static void stmt__while_lp_exp_rp_stmt(Node* node);
static void deflist__def_deflist(Node* node);
static void def__specifier_declist_semi(Node* node);
static void declist__dec(Node* node);
static void declist__dec_comma_declist(Node* node);
static void dec__vardec(Node* node);
static void dec__vardec_assignop_exp(Node* node);
static void exp__exp_assignop_exp(Node* node);
static void exp__exp_op_exp(Node* node);
static void exp__lp_exp_rp(Node* node);
static void exp__minus_exp(Node* node);
static void exp__not_exp(Node* node);
static void exp__id_lp_args_rp(Node* node);
static void exp__id_lp_rp(Node* node);
static void exp__exp_lb_exp_rb(Node* node);
static void exp__exp_dot_id(Node* node);
static void exp__id(Node* node);
static void exp__int(Node* node);
static void exp__float(Node* node);
static void args__exp_comma_args(Node* node);
static void args__exp(Node* node);


typedef void (*handlerPoint)(Node*);

static handlerPoint handler[] = {
    [Program__ExtDefList] = program__extdeflist,
    [ExtDefList__ExtDef_ExtDefList] = extdeflist__extdef_extdeflist,
    [ExtDef__Specifier_ExtDecList_SEMI] = extdef__specifier_extdeclist_semi,
    [ExtDef__Specifier_SEMI] = extdef__specifier_semi,
    [ExtDef__Specifier_FunDec_CompSt] = extdef__specifier_fundec_compst,
    [ExtDecList__VarDec] = extdeclist__vardec,
    [ExtDecList__VarDec_COMMA_ExtDecList] = extdeclist__vardec_comma_extdeclist,
    [Specifier__TYPE] = specifier__type,
    [Specifier__StructSpecifier] = specifier__structspecifier,
    [StructSpecifier__STRUCT_ID_LC_DefList_RC] = structspecifier__struct_id_lc_deflist_rc,
    [StructSpecifier__STRUCT_LC_DefList_RC] = structspecifier__struct_lc_deflist_rc,
    [StructSpecifier__STRUCT_ID] = structspecifier__struct_id,
    [VarDec__ID] = vardec__id,
    [VarDec__VarDec_LB_INT_RB] = vardec__vardec_lb_int_rb,
    [FunDec__ID_LP_VarList_RP] = fundec__id_lp_varlist_rp,
    [FunDec__ID_LP_RP] = fundec__id_lp_rp,
    [VarList__ParamDec_COMMA_VarList] = varlist__paramdec_comma_varlist,
    [VarList__ParamDec] = varlist__paramdec,
    [ParamDec__Specifier_VarDec] = paramdec__specifier_vardec,
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
    [Exp__Exp_AND_Exp] = exp__exp_op_exp,
    [Exp__Exp_OR_Exp] = exp__exp_op_exp,
    [Exp__Exp_RELOP_Exp] = exp__exp_op_exp,
    [Exp__Exp_PLUS_Exp] = exp__exp_op_exp,
    [Exp__Exp_MINUS_Exp] = exp__exp_op_exp,
    [Exp__Exp_STAR_Exp] = exp__exp_op_exp,
    [Exp__Exp_DIV_Exp] = exp__exp_op_exp,
    [Exp__LP_Exp_RP] = exp__lp_exp_rp,
    [Exp__MINUS_Exp] = exp__minus_exp,
    [Exp__NOT_Exp] = exp__not_exp,
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


void semanticError(int line, char* msg, char* idname){
    semanticErrorCount++;
    printf("[semantic error] line %d: ", line);
    printf(msg, idname);
}

bool checkType(Type* t1, Type* t2){
    if(t1 == NULL || t2 == NULL){
        return true;
    }else if(t1 == t2){
        return true;
    }
    if(t1->typeTag != t2->typeTag)
        return false;

    if(t1->typeTag == type_unknown){
        return checkType(t1->myType, t2->myType);
    }else if(t1->typeTag == type_array){
        return t1->array.size == t2->array.size && checkType(t1->array.element, t2->array.element);
    }else if(t1->typeTag == define_struct){
        if(t1->structure.elementCount != t2->structure.elementCount)
            return false;
        for(int i = 0; i < t1->structure.elementCount; ++i){
            if(!checkType(t1->structure.element[i], t2->structure.element[i])){
                return false;
            }
        }
        return true;
    }else{
        assert(0);
    }
    return false;
}

static bool isLeftValue(Node* node){
    if(node->rule == Exp__ID){
        Type* t = findSymbolTable(node->firstChild->text);
        return t != NULL && t->typeTag == type_unknown;
    }else{
        return node->rule == Exp__Exp_LB_Exp_RB || node->rule == Exp__Exp_DOT_ID;
    }
}

void semanticCheck(){
    semanticErrorCount = 0;
    inStruct = false;
    initSymbolTable();
    program__extdeflist(root);
}


#define handle(x) if((x) != NULL){ handler[(x)->rule](x); }


static void program__extdeflist(Node* node){
    if(node == NULL) return;
    handle(node->firstChild);
}

static void extdeflist__extdef_extdeflist(Node* node){
    if(node == NULL) return;
    Node* extdef = node->firstChild;
    handle(extdef);
    handle(extdef->nextSibling);
}

static void extdef__specifier_extdeclist_semi(Node* node){
    if(node == NULL) return;
    Node* specifier = node->firstChild;
    handle(specifier);
    Node* extdeclist = specifier->nextSibling;
    extdeclist->type = specifier->type;
    handle(extdeclist);
}

static void extdef__specifier_semi(Node* node){
    if(node == NULL) return;
    handle(node->firstChild);
}

static void extdef__specifier_fundec_compst(Node* node){
    if(node == NULL) return;
    Node* specifier = node->firstChild;
    handle(specifier);

    Node* fundec = specifier->nextSibling;
    fundec->type = specifier->type;
    handle(fundec);

    Node* compst = fundec->nextSibling;
    compst->type = specifier->type;
    handle(compst);
}

static void extdeclist__vardec(Node* node){
    if(node == NULL) return;
    Node* vardec = node->firstChild;
    vardec->type = node->type;
    handle(vardec);

    if(!insertSymbolTableType(vardec->type)){
        semanticError(vardec->line, "Duplicated definition of '%s'\n", vardec->text);
    }
}

static void extdeclist__vardec_comma_extdeclist(Node* node){
    if(node == NULL) return;
    Node* vardec = node->firstChild;
    Node* comma = vardec->nextSibling;
    Node* vardeclist = comma->nextSibling;

    vardec->type = node->type;
    handle(vardec);
    if(!insertSymbolTableType(vardec->type)){
        semanticError(vardec->line, "Duplicated definition of '%s'\n", vardec->text);
    }

    vardeclist->type = node->type;
    handle(vardeclist);
}

static void specifier__type(Node* node){
    if(node == NULL) return;
    Node* type = node->firstChild;
    if(strcmp(type->text, "int") == 0){
        type->type = BASIC_INT;
    }else if(strcmp(type->text, "float") == 0){
        type->type = BASIC_FLOAT;
    }else{
        type->type = BASIC_UNKNOWN;
        semanticError(type->line, "Unknown type '%s'\n", type->text);
    }
}

static void specifier__structspecifier(Node* node){
    if(node == NULL) return;
    Node* structspecifier = node->firstChild;
    if(structspecifier != NULL){
        handle(structspecifier);
        node->type = structspecifier->type;
    }
}

static const int maxElement = 12;
static int elementIndex;
Type** structElement;

static void structspecifier__struct_id_lc_deflist_rc(Node* node){
    if(node == NULL) return;
    inStruct = true;

    Node* id = node->firstChild->nextSibling;

    Type* tmp = newType();
    tmp->name = id->text;
    tmp->typeTag = define_struct;

    Node* deflist = id->nextSibling->nextSibling;
    if(deflist->tag == _DefList){
        structElement = (Type**)malloc(sizeof(Type*) * maxElement);
        memset(structElement, 0, sizeof(Type*) * maxElement);
        elementIndex = 0;

        handle(deflist);//将数据成员的指针存到tmp->element中

        tmp->structure.elementCount = elementIndex;
        tmp->structure.element = structElement;
    }else{
        tmp->structure.elementCount = 0;
        tmp->structure.element = NULL;        
    }
    if(!insertSymbolTableType(tmp)){
        semanticError(id->line, "Duplicated definition of '%s'\n", tmp->name);
    }
    node->type = tmp;
    inStruct = false;
}

static void structspecifier__struct_lc_deflist_rc(Node* node){
    if(node == NULL) return;
    inStruct = true;

    Type* tmp = newType();
    tmp->name = "nameless struct";
    tmp->typeTag = define_struct;

    Node* deflist = node->firstChild->nextSibling->nextSibling;
    if(deflist->tag == _DefList){
        structElement = (Type**)malloc(sizeof(Type*) * maxElement);
        memset(structElement, 0, sizeof(Type*) * maxElement);
        elementIndex = 0;

        handle(deflist);//将数据成员的指针存到tmp->element中

        tmp->structure.elementCount = elementIndex;
        tmp->structure.element = structElement;
    }else{
        tmp->structure.elementCount = 0;
        tmp->structure.element = NULL;        
    }
    node->type = tmp;

    inStruct = false;
}

static void structspecifier__struct_id(Node* node){
    if(node == NULL) return;
    Node* id = node->firstChild->nextSibling;
    Symbol* tmp = findSymbolTable(id->text);
    if(tmp != NULL && tmp->typeTag == define_struct){
        semanticError(id->line, "Undefined struct name '%s'\n", id->text);
    }else{
        node->type = tmp;
    }
}

static void vardec__id(Node* node){
    if(node == NULL) return;

    Type* t = node->type;
    Node* id = node->firstChild;
    if(t != NULL){
        Type* me = newType();
        me->name = id->text;
        me->typeTag = type_unknown;
        me->myType = t; // myType => 普通变量的类型
        node->type = me;        
    }else{
        assert(0);
    }
}

static void vardec__vardec_lb_int_rb(Node* node){
    if(node == NULL) return;
    Node* sub = node->firstChild;
    Node* size = sub->nextSibling->nextSibling;
    int s = size->intVal;

    Type* tmp = newType();
    tmp->typeTag = type_array;
    tmp->array.size = s;
    tmp->array.element = node->type;
    // tmp->myType = node->type;

    sub->type = tmp;
    handle(sub);
    node->type = sub->type;
    // node->type->typeTag = type_array;
}

static const int maxArgc = 12;
static int funcArgIndex;
Type** funcArgv;

static void fundec__id_lp_varlist_rp(Node* node){
    if(node == NULL) return;
    Node* id = node->firstChild;
    Node* varlist = id->nextSibling->nextSibling;

    Type* tmp = newType();
    tmp->function.ret = node->type;
    tmp->typeTag = type_func;
    tmp->name = id->text;

    funcArgIndex = 0;
    funcArgv = (Type**)malloc(sizeof(Type*) * maxArgc);
    memset(funcArgv, 0, sizeof(Type*) * maxArgc);
    handle(varlist);
    tmp->function.argc = funcArgIndex;
    tmp->function.argv = funcArgv;
    node->type = tmp;

    if(!insertSymbolTableType(tmp)){
        semanticError(id->line, "Duplicated definition of '%s'\n", tmp->name);
    }
}

static void fundec__id_lp_rp(Node* node){
    if(node == NULL) return;
    Node* id = node->firstChild;

    Type* tmp = newType();
    tmp->function.ret = node->type;
    tmp->typeTag = type_func;
    tmp->name = id->text;
    tmp->function.argc = 0;
    tmp->function.argv = NULL;
    node->type = tmp;

    if(!insertSymbolTableType(tmp)){
        semanticError(id->line, "Duplicated definition of '%s'\n", tmp->name);
    }
}

static void varlist__paramdec_comma_varlist(Node* node){
    if(node == NULL) return;
    Node* paramdec = node->firstChild;
    Node* varlist = paramdec->nextSibling->nextSibling;
    handle(paramdec);
    handle(varlist);
}

static void varlist__paramdec(Node* node){
    if(node == NULL) return;
    Node* paramdec = node->firstChild;    
    handle(paramdec);
}

static void paramdec__specifier_vardec(Node* node){
    if(node == NULL) return;
    Node* specifier = node->firstChild;
    Node* vardec = specifier->nextSibling;

    handle(specifier);
    vardec->type = specifier->type;
    handle(vardec);

    if(!insertSymbolTableType(vardec->type)){
        semanticError(node->line, "Duplicated definition of '%s'\n", vardec->type->name);
    }
    node->type = vardec->type;

    funcArgv[funcArgIndex] = node->type;
    funcArgIndex++;
}

static void compst__lc_deflist_stmtlist_rc(Node* node){
    if(node == NULL) return;
    Node* list = node->firstChild->nextSibling;
    if(list->tag == _DefList){
        handle(list);
        list = list->nextSibling;
    }
    if(list->tag == _StmtList){
        list->type = node->type;
        handle(list);
    }
}

static void stmtlist__stmt_stmtlist(Node* node){
    if(node == NULL) return;
    node->firstChild->type = node->type;
    handle(node->firstChild);
    Node* list = node->firstChild->nextSibling;
    if(list !=  NULL){
        list->type = node->type;
        handle(list);
    }
}

static void stmt__exp_semi(Node* node){
    if(node == NULL) return;
    handle(node->firstChild);
}

static void stmt__compst(Node* node){
    if(node == NULL) return;
    node->firstChild->type = node->type;
    handle(node->firstChild);
}

static void stmt__return_exp_semi(Node* node){
    if(node == NULL) return;
    Node* exp = node->firstChild->nextSibling;
    handle(exp);
    if(!checkType(exp->type, node->type)){
        semanticError(node->line, "Returning type mismatched\n", NULL);
    }
}   

static void stmt__if_lp_exp_rp_stmt(Node* node){
    if(node == NULL) return;
    Node* exp = node->firstChild->nextSibling->nextSibling;
    Node* stmt = exp->nextSibling->nextSibling;
    stmt->type = node->type;
    handle(exp);
    if(!checkType(exp->type, BASIC_INT)){
        semanticError(node->line, "The condition expression must return int\n", NULL);
    }
    handle(stmt);
}

static void stmt__if_lp_exp_rp_stmt_else_stmt(Node* node){
    if(node == NULL) return;
    Node* exp = node->firstChild->nextSibling->nextSibling;
    Node* stmt1 = exp->nextSibling->nextSibling;
    Node* stmt2 = stmt1->nextSibling->nextSibling;
    stmt1->type = node->type;
    stmt2->type = node->type;
    handle(exp);
    if(!checkType(exp->type, BASIC_INT)){
        semanticError(node->line, "The condition expression must return int\n", NULL);
    }
    handle(stmt1);
    handle(stmt2);      
}

static void stmt__while_lp_exp_rp_stmt(Node* node){
    if(node == NULL) return;
    Node* exp = node->firstChild->nextSibling->nextSibling;
    Node* stmt = exp->nextSibling->nextSibling;
    handle(exp);
    if(!checkType(exp->type, BASIC_INT)){
        semanticError(node->line, "The condition expression must return int\n", NULL);
    }
    handle(stmt);
}

static void deflist__def_deflist(Node* node){
    if(node == NULL) return;
    Node* def = node->firstChild;
    Node* deflist = def->nextSibling;
    handle(def);
    handle(deflist);
}

static void def__specifier_declist_semi(Node* node){
    if(node == NULL) return;
    handle(node->firstChild);
    Type* tmp = node->firstChild->type;

    Node* declist = node->firstChild->nextSibling;
    declist->type = tmp;
    handle(declist);
}

static void declist__dec(Node* node){
    if(node == NULL) return;
    Node* dec = node->firstChild;
    dec->type = node->type;
    handle(dec);
}

static void declist__dec_comma_declist(Node* node){
    if(node == NULL) return;
    handle(node->firstChild);
    Node* declist = node->firstChild->nextSibling->nextSibling;
    declist->type = node->type;
    handle(declist);
}

static void dec__vardec(Node* node){
    if(node == NULL) return;
    Node* vardec = node->firstChild;
    vardec->type = node->type;
    handle(vardec);

    if(!insertSymbolTableType(vardec->type)){
        semanticError(vardec->line, "Redefined variable '%s'\n", vardec->text);
    }
    node->type = vardec->type;

    if(inStruct){    // 添加var到struct中
        structElement[elementIndex] = node->type;
        elementIndex++;
    }
}

static void dec__vardec_assignop_exp(Node* node){
    if(node == NULL) return;
    dec__vardec(node);
    if(inStruct){
        semanticError(node->line, "Initialization in the structure definition is not allowed\n", NULL);
    }
    else{
        Node* exp = node->firstChild->nextSibling->nextSibling;
        handle(exp);
        if(!checkType(exp->type, node->type)){
            semanticError(node->line, "Type mismatch\n", NULL);
        }
    }
}

static void exp__exp_assignop_exp(Node* node){
    if(node == NULL) return;
    Node* exp1 = node->firstChild;
    Node* exp2 = exp1->nextSibling->nextSibling;
    handle(exp1);
    handle(exp2);
    if(exp1->type != NULL && exp2->type != NULL){
        if(!checkType(exp1->type, exp2->type)){
            semanticError(node->line, "Type mismatch\n", NULL);
        }else if(!isLeftValue(exp1)){
            semanticError(node->line, "The left-hand side of an assignment must be a variable\n", NULL);
        }else{
            node->type = exp1->type;
        }
    }
}

static void exp__exp_op_exp(Node* node){
    if(node == NULL) return;
    Node* exp1 = node->firstChild;
    Node* exp2 = exp1->nextSibling->nextSibling;
    handle(exp1);
    handle(exp2);
    if(exp1->type != NULL && exp2->type != NULL){
        if(!checkType(exp1->type, exp2->type)){
            semanticError(node->line, "Type mismatch\n", NULL);
        }else if(!checkType(exp1->type, BASIC_FLOAT) && !checkType(exp1->type, BASIC_INT)){
            semanticError(node->line, "Type is not allowed in operation '%s'\n", exp1->nextSibling->text);
        }else{
            node->type = exp1->type;    
        }        
    }
}

static void exp__lp_exp_rp(Node* node){
    if(node == NULL) return;
    Node* exp = node->firstChild->nextSibling;
    handle(exp);
    node->type = exp->type;
}

static void exp__minus_exp(Node* node){
    if(node == NULL) return;
    Node* exp = node->firstChild->nextSibling;
    handle(exp);
    Type* t = exp->type;
    if(t != NULL){
        if(!(checkType(t, BASIC_INT) || checkType(t, BASIC_FLOAT))){
            semanticError(node->line, "'%s' can only cast on basic type\n", node->firstChild->text);
        }else{
            node->type = exp->type; 
        }
    }
}

static void exp__not_exp(Node* node){
    if(node == NULL) return;
    Node* exp = node->firstChild->nextSibling;
    handle(exp);
    Type* t = exp->type;
    if(t != NULL){
        if(!checkType(t, BASIC_INT)){
            semanticError(node->line, "'%s' can only cast on int type\n", node->firstChild->text);
        }else{
            node->type = exp->type; 
        }
    }
}

static Type** stdArgv;
static int argIndex;
static int argCount;

static void exp__id_lp_args_rp(Node* node){
    if(node == NULL) return;
    Node* id = node->firstChild;
    Node* args = id->nextSibling->nextSibling;
    Type* func = findSymbolTable(id->text);
    if(func == NULL){
        semanticError(node->line, "Undefined function '%s'\n", id->text);
    }else if(func->typeTag != type_func){
        semanticError(node->line, "'%s' is not a function\n", id->text);
    }else{
        stdArgv = func->function.argv;
        argCount = func->function.argc;
        argIndex = 0;
        handle(args);
        node->type = func->function.ret;
    }
}

static void exp__id_lp_rp(Node* node){
    if(node == NULL) return;
    Node* id = node->firstChild;
    Type* func = findSymbolTable(id->text);
    if(func == NULL){
        semanticError(node->line, "Undefined function '%s'\n", id->text);
    }else if(func->typeTag != type_func){
        semanticError(node->line, "'%s' is not a function\n", id->text);
    }else{
        node->type = func->function.ret;
    }
}

static void exp__exp_lb_exp_rb(Node* node){
    if(node == NULL) return;
    Node* exp1 = node->firstChild;
    Node* exp2 = exp1->nextSibling->nextSibling;
    handle(exp1);handle(exp2);
    if(exp2->type != NULL && exp2->type != BASIC_INT){
        semanticError(node->line, "Index should be integer\n", NULL);
    }
    if(exp1->type != NULL){
        if(exp1->type->typeTag != type_array){
            semanticError(node->line, "Not an array\n", NULL);
        }else{
            node->type = exp1->type->array.element;
        }
    }
}

static void exp__exp_dot_id(Node* node){
    if(node == NULL) return;
    Node* exp = node->firstChild;
    Node* id = exp->nextSibling->nextSibling;
    if(exp->type == NULL || exp->type->myType == NULL || exp->type->myType->typeTag != define_struct){
        semanticError(node->line, "Not a structure instance\n", NULL);
    }else{
        Type* t = exp->type;
        int cnt = t->structure.elementCount;
        bool get = false;
        for(int i = 0 ; i < cnt; ++i){
            if(strcmp(id->text, t->structure.element[i]->name) == 0){
                node->type = t->structure.element[i];
                return;
            }
        }
        semanticError(node->line, "No '%s' field in structure\n", id->text);
    }
}

static void exp__id(Node* node){
    if(node == NULL) return;
    Node* id = node->firstChild;
    Type* t = findSymbolTable(id->text);
    if(t == NULL){
        semanticError(node->line, "Undefined '%s'\n", id->text);
    }else if(t->typeTag == define_struct){
        semanticError(node->line, "'%s' is name of structure\n", id->text);
    }else{
        if(t->typeTag == type_unknown){
            node->type = t->myType;
        }else{
            assert(0);
        }   
    }
}

static void exp__int(Node* node){
    if(node == NULL) return;
    node->type = BASIC_INT;
}

static void exp__float(Node* node){
    if(node == NULL) return;
    node->type = BASIC_FLOAT;
}

static void args__exp_comma_args(Node* node){
    if(node == NULL) return;
    Node* exp = node->firstChild;
    Node* args = exp->nextSibling->nextSibling;
    handle(exp);
    Type* t = exp->type;
    if(argIndex >= argCount){
        semanticError(node->line, "Number of arguments mismatched\n", NULL);
    }else if(!checkType(t, stdArgv[argIndex])){
        semanticError(node->line, "Type of argument mismatched\n", NULL);
    }
    argIndex++;
    handle(args);
}

static void args__exp(Node* node){
    if(node == NULL) return;
    Node* exp = node->firstChild;
    handle(exp);
    Type* t = exp->type;
    if(argIndex >= argCount){
        semanticError(node->line, "Number of arguments mismatched\n", NULL);
    }else if(!checkType(t, stdArgv[argIndex])){
        semanticError(node->line, "Type of argument mismatched\n", NULL);
    }
    argIndex++;
    if(argCount != argIndex){
        semanticError(node->line, "Number of arguments mismatched\n", NULL);        
    }
}

#undef handle





