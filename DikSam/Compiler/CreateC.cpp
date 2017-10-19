#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

extern int  g_iCurrentThreadIndex;
extern char* yytext;

int yyerror(char const *str)
{
    DikSam::GetClassObject(g_iCurrentThreadIndex)->GetError()->CompileError(
        DikSam::GetClassObject(g_iCurrentThreadIndex)->GetInterface()->GetCurrentCompiler()->current_line_number,
        PARSE_ERR, STRING_MESSAGE_ARGUMENT, "token", yytext, MESSAGE_ARGUMENT_END);

    return 0;
}

PackageName* dkc_create_package_name(char *identifier)
{
    return DikSam::GetClassObject(g_iCurrentThreadIndex)->GetCreate()->CreatePackageName(identifier);
}

PackageName* dkc_chain_package_name(PackageName *list, char *identifier)
{
    return DikSam::GetClassObject(g_iCurrentThreadIndex)->GetCreate()->ChainPackageName(list, identifier);
}

RequireList* dkc_create_require_list(PackageName *package_name)
{
    return DikSam::GetClassObject(g_iCurrentThreadIndex)->GetCreate()->CreateRequireList(package_name);
}

RequireList* dkc_chain_require_list(RequireList *list, RequireList *add)
{
    return DikSam::GetClassObject(g_iCurrentThreadIndex)->GetCreate()->ChainRequireList(list, add);
}

RenameList* dkc_create_rename_list(PackageName *package_name, char *identifier)
{
    return DikSam::GetClassObject(g_iCurrentThreadIndex)->GetCreate()->CreateRenameList(package_name, identifier);
}

RenameList* dkc_chain_rename_list(RenameList *list, RenameList *add)
{
    return DikSam::GetClassObject(g_iCurrentThreadIndex)->GetCreate()->ChainRenameList(list, add);
}

void dkc_set_require_and_rename_list(RequireList *require_list, RenameList *rename_list)
{
    DikSam::GetClassObject(g_iCurrentThreadIndex)->GetCreate()->SetRequireAndRenameList(require_list, rename_list);
}

FunctionDefinition* dkc_create_function_definition(TypeSpecifier *type, char *identifier, ParameterList *parameter_list, Block *block)
{
    return DikSam::GetClassObject(g_iCurrentThreadIndex)->GetCreate()->CreateFunctionDefinition(type, identifier, parameter_list, block);
}

void dkc_function_define(TypeSpecifier *type, char *identifier, ParameterList *parameter_list, Block *block)
{
    DikSam::GetClassObject(g_iCurrentThreadIndex)->GetCreate()->FunctionDefine(type, identifier, parameter_list, block);
}

ParameterList* dkc_create_parameter(TypeSpecifier *type, char *identifier)
{
    return DikSam::GetClassObject(g_iCurrentThreadIndex)->GetCreate()->CreateParameter(type, identifier);
}

ParameterList* dkc_chain_parameter(ParameterList *list, TypeSpecifier *type, char *identifier)
{
    return DikSam::GetClassObject(g_iCurrentThreadIndex)->GetCreate()->ChainParameter(list, type, identifier);
}

ArgumentList* dkc_create_argument_list(Expression *expression)
{
    return DikSam::GetClassObject(g_iCurrentThreadIndex)->GetCreate()->CreateArgumentList(expression);
}

ExpressionList* dkc_create_expression_list(Expression *expression)
{
    return DikSam::GetClassObject(g_iCurrentThreadIndex)->GetCreate()->CreateExpressionList(expression);
}

ExpressionList* dkc_chain_expression_list(ExpressionList *list, Expression *expression)
{
    return DikSam::GetClassObject(g_iCurrentThreadIndex)->GetCreate()->ChainExpressionList(list, expression);
}

ArgumentList* dkc_chain_argument_list(ArgumentList *list, Expression *expr)
{
    return DikSam::GetClassObject(g_iCurrentThreadIndex)->GetCreate()->ChainArgumentList(list, expr);
}

StatementList* dkc_create_statement_list(Statement *statement)
{
    return DikSam::GetClassObject(g_iCurrentThreadIndex)->GetCreate()->CreateStatementList(statement);
}

StatementList* dkc_chain_statement_list(StatementList *list, Statement *statement)
{
    return DikSam::GetClassObject(g_iCurrentThreadIndex)->GetCreate()->ChainStatementList(list, statement);
}

TypeSpecifier* dkc_create_type_specifier(DVM_BasicType basic_type)
{
    return DikSam::GetClassObject(g_iCurrentThreadIndex)->GetCreate()->CreateTypeSpecifier(basic_type);
}

TypeSpecifier* dkc_create_class_type_specifier(char *identifier)
{
    return DikSam::GetClassObject(g_iCurrentThreadIndex)->GetCreate()->CreateClassTypeSpecifier(identifier);
}

TypeSpecifier* dkc_create_array_type_specifier(TypeSpecifier *base)
{
    return DikSam::GetClassObject(g_iCurrentThreadIndex)->GetCreate()->CreateArrayTypeSpecifier(base);
}

Expression* dkc_alloc_expression(ExpressionKind kind)
{
    return DikSam::GetClassObject(g_iCurrentThreadIndex)->GetCreate()->AllocExpression(kind);
}

Expression* dkc_create_comma_expression(Expression *left, Expression *right)
{
    return DikSam::GetClassObject(g_iCurrentThreadIndex)->GetCreate()->CreateCommaExpression(left, right);
}

Expression* dkc_create_assign_expression(Expression *left, AssignmentOperator op, Expression *operand)
{
    return DikSam::GetClassObject(g_iCurrentThreadIndex)->GetCreate()->CreateAssignExpression(left, op, operand);
}

Expression* dkc_create_binary_expression(ExpressionKind op, Expression *left, Expression *right)
{
    return DikSam::GetClassObject(g_iCurrentThreadIndex)->GetCreate()->CreateBinaryExpression(op, left, right);
}

Expression* dkc_create_minus_expression(Expression *operand)
{
    return DikSam::GetClassObject(g_iCurrentThreadIndex)->GetCreate()->CreateMinusExpression(operand);
}

Expression* dkc_create_logical_not_expression(Expression *operand)
{
    return DikSam::GetClassObject(g_iCurrentThreadIndex)->GetCreate()->CreateLogicalNotExpression(operand);
}

Expression* dkc_create_incdec_expression(Expression *operand, ExpressionKind inc_or_dec)
{
    return DikSam::GetClassObject(g_iCurrentThreadIndex)->GetCreate()->CreateIncDecExpression(operand, inc_or_dec);
}

Expression* dkc_create_instanceof_expression(Expression *operand, TypeSpecifier *type)
{
    return DikSam::GetClassObject(g_iCurrentThreadIndex)->GetCreate()->CreateInstanceofExpression(operand, type);
}

Expression* dkc_create_identifier_expression(char *identifier)
{
    return DikSam::GetClassObject(g_iCurrentThreadIndex)->GetCreate()->CreateIdentifierExpression(identifier);
}

Expression* dkc_create_function_call_expression(Expression *function, ArgumentList *argument)
{
    return DikSam::GetClassObject(g_iCurrentThreadIndex)->GetCreate()->CreateFunctionCallExpression(function, argument);
}

Expression* dkc_create_down_cast_expression(Expression *operand, TypeSpecifier *type)
{
    return DikSam::GetClassObject(g_iCurrentThreadIndex)->GetCreate()->CreateDownCastExpression(operand, type);
}

Expression* dkc_create_member_expression(Expression *expression, char *member_name)
{
    return DikSam::GetClassObject(g_iCurrentThreadIndex)->GetCreate()->CreateMemberExpression(expression, member_name);
}

Expression* dkc_create_boolean_expression(DVM_Boolean value)
{
    return DikSam::GetClassObject(g_iCurrentThreadIndex)->GetCreate()->CreateBooleanExpression(value);
}

Expression* dkc_create_null_expression(void)
{
    return DikSam::GetClassObject(g_iCurrentThreadIndex)->GetCreate()->CreateNullExpression();
}

Expression* dkc_create_new_expression(char *class_name, char *method_name, ArgumentList *argument)
{
    return DikSam::GetClassObject(g_iCurrentThreadIndex)->GetCreate()->CreateNewExpression(class_name, method_name, argument);
}

Expression* dkc_create_index_expression(Expression *array, Expression *index)
{
    return DikSam::GetClassObject(g_iCurrentThreadIndex)->GetCreate()->CreateIndexExpression(array, index);
}

Expression* dkc_create_array_literal_expression(ExpressionList *list)
{
    return DikSam::GetClassObject(g_iCurrentThreadIndex)->GetCreate()->CreateArrayLiteralExpression(list);
}

Expression* dkc_create_basic_array_creation(DVM_BasicType basic_type, ArrayDimension *dim_expr_list, ArrayDimension *dim_list)
{
    return DikSam::GetClassObject(g_iCurrentThreadIndex)->GetCreate()->CreateBasicArrayCreation(basic_type, dim_expr_list, dim_list);
}

Expression* dkc_create_class_array_creation(TypeSpecifier *type, ArrayDimension *dim_expr_list, ArrayDimension *dim_list)
{
    return DikSam::GetClassObject(g_iCurrentThreadIndex)->GetCreate()->CreateClassArrayCreation(type, dim_expr_list, dim_list);
}

Expression* dkc_create_this_expression(void)
{
    return DikSam::GetClassObject(g_iCurrentThreadIndex)->GetCreate()->CreateThisExpression();
}

Expression* dkc_create_super_expression(void)
{
    return DikSam::GetClassObject(g_iCurrentThreadIndex)->GetCreate()->CreateSuperExpression();
}

ArrayDimension* dkc_create_array_dimension(Expression *expr)
{
    return DikSam::GetClassObject(g_iCurrentThreadIndex)->GetCreate()->CreateArrayDimension(expr);
}

ArrayDimension* dkc_chain_array_dimension(ArrayDimension *list, ArrayDimension *dim)
{
    return DikSam::GetClassObject(g_iCurrentThreadIndex)->GetCreate()->ChainArrayDimension(list, dim);
}

Statement* dkc_alloc_statement(StatementType type)
{
    return DikSam::GetClassObject(g_iCurrentThreadIndex)->GetCreate()->AllocStatement(type);
}

Statement* dkc_create_if_statement(Expression *condition, Block *then_block, Elsif *elsif_list, Block *else_block)
{
    return DikSam::GetClassObject(g_iCurrentThreadIndex)->GetCreate()->CreateIfStatement(condition, then_block, elsif_list, else_block);
}

Elsif* dkc_chain_elsif_list(Elsif *list, Elsif *add)
{
    return DikSam::GetClassObject(g_iCurrentThreadIndex)->GetCreate()->ChainElsifList(list, add);
}

Elsif* dkc_create_elsif(Expression *expr, Block *block)
{
    return DikSam::GetClassObject(g_iCurrentThreadIndex)->GetCreate()->CreateElsif(expr, block);
}

Statement* dkc_create_while_statement(char *label, Expression *condition, Block *block)
{
    return DikSam::GetClassObject(g_iCurrentThreadIndex)->GetCreate()->CreateWhileStatement(label, condition, block);
}

Statement* dkc_create_do_while_statement(char *label, Block *block, Expression *condition)
{
    return DikSam::GetClassObject(g_iCurrentThreadIndex)->GetCreate()->CreateDoWhileStatement(label, block, condition);
}

Statement* dkc_create_for_statement(char *label, Expression *init, Expression *cond, Expression *post, Block *block)
{
    return DikSam::GetClassObject(g_iCurrentThreadIndex)->GetCreate()->CreateForStatement(label, init, cond, post, block);
}

Block* dkc_alloc_block()
{
    return DikSam::GetClassObject(g_iCurrentThreadIndex)->GetCreate()->AllocBlock();
}

Block* dkc_open_block(void)
{
    return DikSam::GetClassObject(g_iCurrentThreadIndex)->GetCreate()->OpenBlock();
}

Block* dkc_close_block(Block *block, StatementList *statement_list)
{
    return DikSam::GetClassObject(g_iCurrentThreadIndex)->GetCreate()->CloseBlock(block, statement_list);
}

Statement* dkc_create_expression_statement(Expression *expression)
{
    return DikSam::GetClassObject(g_iCurrentThreadIndex)->GetCreate()->CreateExpressionStatement(expression);
}

Statement* dkc_create_return_statement(Expression *expression)
{
    return DikSam::GetClassObject(g_iCurrentThreadIndex)->GetCreate()->CreateReturnStatement(expression);
}

Statement* dkc_create_break_statement(char *label)
{
    return DikSam::GetClassObject(g_iCurrentThreadIndex)->GetCreate()->CreateBreakStatement(label);
}

Statement* dkc_create_continue_statement(char *label)
{
    return DikSam::GetClassObject(g_iCurrentThreadIndex)->GetCreate()->CreateContinueStatement(label);
}

Statement* dkc_create_try_statement(Block *try_block, CatchClause *catch_clause, Block *finally_block)
{
    return DikSam::GetClassObject(g_iCurrentThreadIndex)->GetCreate()->CreateTryStatement(try_block, catch_clause, finally_block);
}

CatchClause* dkc_create_catch_clause(TypeSpecifier *type, char *variable_name, Block *block)
{
    return DikSam::GetClassObject(g_iCurrentThreadIndex)->GetCreate()->CreateCatchClause(type, variable_name, block);
}

CatchClause* dkc_start_catch_clause(void)
{
    return DikSam::GetClassObject(g_iCurrentThreadIndex)->GetCreate()->StartCatchClause();
}

CatchClause* dkc_end_catch_clause(CatchClause *catch_clause, TypeSpecifier *type, char *variable_name, Block *block)
{
    return DikSam::GetClassObject(g_iCurrentThreadIndex)->GetCreate()->EndCatchClause(catch_clause, type, variable_name, block);
}

CatchClause* dkc_chain_catch_list(CatchClause *list, CatchClause *add)
{
    return DikSam::GetClassObject(g_iCurrentThreadIndex)->GetCreate()->ChainCatchList(list, add);
}

Statement* dkc_create_throw_statement(Expression *expression)
{
    return DikSam::GetClassObject(g_iCurrentThreadIndex)->GetCreate()->CreateThrowStatement(expression);
}

Statement* dkc_create_declaration_statement(TypeSpecifier *type, char *identifier, Expression *initializer)
{
    return DikSam::GetClassObject(g_iCurrentThreadIndex)->GetCreate()->CreateDeclarationStatement(type, identifier, initializer);
}

void dkc_start_class_definition(ClassOrMemberModifierList *modifier, DVM_ClassOrInterface class_or_interface, char *identifier, ExtendsList *extends)
{
    DikSam::GetClassObject(g_iCurrentThreadIndex)->GetCreate()->StartClassDefinition(modifier, class_or_interface, identifier, extends);
}

void dkc_class_define(MemberDeclaration *member_list)
{
    DikSam::GetClassObject(g_iCurrentThreadIndex)->GetCreate()->ClassDefine(member_list);
}

ExtendsList* dkc_create_extends_list(char *identifier)
{
    return DikSam::GetClassObject(g_iCurrentThreadIndex)->GetCreate()->CreateExtendsList(identifier);
}

ExtendsList* dkc_chain_extends_list(ExtendsList *list, char *add)
{
    return DikSam::GetClassObject(g_iCurrentThreadIndex)->GetCreate()->ChainExtendsList(list, add);
}

ClassOrMemberModifierList dkc_create_class_or_member_modifier(ClassOrMemberModifierKind modifier)
{
    return DikSam::GetClassObject(g_iCurrentThreadIndex)->GetCreate()->CreateClassOrMemberModifier(modifier);
}

ClassOrMemberModifierList dkc_chain_class_or_member_modifier(ClassOrMemberModifierList list, ClassOrMemberModifierList add)
{
    return DikSam::GetClassObject(g_iCurrentThreadIndex)->GetCreate()->ChainClassOrMemberModifier(list, add);
}

MemberDeclaration* dkc_chain_member_declaration(MemberDeclaration *list, MemberDeclaration *add)
{
    return DikSam::GetClassObject(g_iCurrentThreadIndex)->GetCreate()->ChainMemberDeclaration(list, add);
}

MemberDeclaration* dkc_create_method_member(ClassOrMemberModifierList *modifier, FunctionDefinition *function_definition, DVM_Boolean is_constructor)
{
    return DikSam::GetClassObject(g_iCurrentThreadIndex)->GetCreate()->CreateMethodMember(modifier, function_definition, is_constructor);
}

FunctionDefinition* dkc_method_function_define(TypeSpecifier *type, char *identifier, ParameterList *parameter_list, Block *block)
{
    return DikSam::GetClassObject(g_iCurrentThreadIndex)->GetCreate()->MethodFunctionDefine(type, identifier, parameter_list, block);
}

FunctionDefinition* dkc_constructor_function_define(char *identifier, ParameterList *parameter_list, Block *block)
{
    return DikSam::GetClassObject(g_iCurrentThreadIndex)->GetCreate()->ConstructorFunctionDefine(identifier, parameter_list, block);
}

MemberDeclaration* dkc_create_field_member(ClassOrMemberModifierList *modifier, TypeSpecifier *type, char *name)
{
    return DikSam::GetClassObject(g_iCurrentThreadIndex)->GetCreate()->CreateFieldMember(modifier, type, name);
}

#ifdef __cplusplus
}
#endif // __cplusplus

#define YY_NO_UNISTD_H
#include "lex.yy.c"
#include "y.tab.c"

#ifdef __cplusplus
extern "C"
{
#endif

void ResetLex(void)
{
    yylex_destroy();
}

#ifdef __cplusplus
}
#endif
