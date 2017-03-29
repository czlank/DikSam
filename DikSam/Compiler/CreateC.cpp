#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus

extern int  g_iCurrentThreadIndex;
extern char* yytext;

int yyerror(char const *str)
{
    DikSam::GetClassObject(g_iCurrentThreadIndex)->GetError()->CompileError(
        DikSam::GetClassObject(g_iCurrentThreadIndex)->GetUtil()->GetCompiler()->current_line_number,
        PARSE_ERR, STRING_MESSAGE_ARGUMENT, "token", yytext, MESSAGE_ARGUMENT_END);

    return 0;
}

void dkc_function_define(DVM_BasicType type, char *identifier, ParameterList *parameter_list, Block *block)
{
    DikSam::GetClassObject(g_iCurrentThreadIndex)->GetCreate()->FunctionDefine(type, identifier, parameter_list, block);
}

ParameterList* dkc_create_parameter(DVM_BasicType type, char *identifier)
{
    return DikSam::GetClassObject(g_iCurrentThreadIndex)->GetCreate()->CreateParameter(type, identifier);
}

ParameterList* dkc_chain_parameter(ParameterList *list, DVM_BasicType type, char *identifier)
{
    return DikSam::GetClassObject(g_iCurrentThreadIndex)->GetCreate()->ChainParameter(list, type, identifier);
}

ArgumentList* dkc_create_argument_list(Expression *expression)
{
    return DikSam::GetClassObject(g_iCurrentThreadIndex)->GetCreate()->CreateArgumentList(expression);
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

Expression* dkc_create_identifier_expression(char *identifier)
{
    return DikSam::GetClassObject(g_iCurrentThreadIndex)->GetCreate()->CreateIdentifierExpression(identifier);
}

Expression* dkc_create_function_call_expression(Expression *function, ArgumentList *argument)
{
    return DikSam::GetClassObject(g_iCurrentThreadIndex)->GetCreate()->CreateFunctionCallExpression(function, argument);
}

Expression* dkc_create_boolean_expression(DVM_Boolean value)
{
    return DikSam::GetClassObject(g_iCurrentThreadIndex)->GetCreate()->CreateBooleanExpression(value);
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

Statement* dkc_create_for_statement(char *label, Expression *init, Expression *cond, Expression *post, Block *block)
{
    return DikSam::GetClassObject(g_iCurrentThreadIndex)->GetCreate()->CreateForStatement(label, init, cond, post, block);
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

Statement* dkc_create_try_statement(Block *try_block, char *exception, Block *catch_block, Block *finally_block)
{
    return DikSam::GetClassObject(g_iCurrentThreadIndex)->GetCreate()->CreateTryStatement(try_block, exception, catch_block, finally_block);
}

Statement* dkc_create_throw_statement(Expression *expression)
{
    return DikSam::GetClassObject(g_iCurrentThreadIndex)->GetCreate()->CreateThrowStatement(expression);
}

Statement* dkc_create_declaration_statement(DVM_BasicType type, char *identifier, Expression *initializer)
{
    return DikSam::GetClassObject(g_iCurrentThreadIndex)->GetCreate()->CreateDeclarationStatement(type, identifier, initializer);
}

#ifdef __cplusplus
}
#endif // __cplusplus

#define YY_NO_UNISTD_H
#include "lex.yy.c"
#include "y.tab.c"
