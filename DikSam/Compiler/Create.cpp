#include "StdAfx.h"
#include "Create.h"
#include "DikSam.h"
#include "Debug.h"
#include "Error.h"
#include "Util.h"
#include "Interface.h"

#include "CreateC.cpp"

Create::Create(Debug& debug, Error& error, Util& util, Interface& rInterface)
    : m_Debug(debug)
    , m_Error(error)
    , m_Util(util)
    , m_Interface(rInterface)
{

}

Create::~Create()
{

}

Declaration* Create::AllocDeclaration(TypeSpecifier *pType, char *lpstrIdentifier)
{
    Declaration *pDecl = (Declaration*)CREATE_UTIL_Malloc(sizeof(Declaration));

    pDecl->name = lpstrIdentifier;
    pDecl->type = pType;
    pDecl->variable_index = -1;

    return pDecl;
}

DeclarationList* Create::ChainDeclaration(DeclarationList *pList, Declaration *pDecl)
{
    DeclarationList *pNewItem = (DeclarationList*)CREATE_UTIL_Malloc(sizeof(DeclarationList));
    
    pNewItem->declaration = pDecl;
    pNewItem->next = nullptr;

    if (nullptr == pList)
    {
        return pNewItem;
    }

    DeclarationList *pos = pList;
    for (; pos->next != nullptr; pos = pos->next)
        ;
    pos->next = pNewItem;

    return pList;
}

void Create::FunctionDefine(TypeSpecifier *pType, char *lpstrIdentifier, ParameterList *pParameterList, Block *pBlock)
{
    if (m_Util.SearchFunction(lpstrIdentifier) || m_Util.SearchDeclaration(lpstrIdentifier, nullptr))
    {
        m_Error.CompileError(m_Interface.GetCompiler()->current_line_number,
            FUNCTION_MULTIPLE_DEFINE_ERR,
            STRING_MESSAGE_ARGUMENT, "name", lpstrIdentifier,
            MESSAGE_ARGUMENT_END);
    }

    FunctionDefinition  *pFD = CreateFunctionDefinition(enType, lpstrIdentifier, pParameterList, pBlock);

    if (pBlock)
    {
        pBlock->type = FUNCTION_BLOCK;
        pBlock->parent.function.function = pFD;
    }

    DKC_Compiler *pCompiler = m_Interface.GetCompiler();

    if (pCompiler->function_list)
    {
        FunctionDefinition *pos = pCompiler->function_list;
        for (; pos->next; pos = pos->next)
            ;
        pos->next = pFD;
    }
    else
    {
        pCompiler->function_list = pFD;
    }
}

ParameterList* Create::CreateParameter(TypeSpecifier *pType, char *lpstrIdentifier)
{
    ParameterList *p = (ParameterList*)CREATE_UTIL_Malloc(sizeof(ParameterList));

    p->name = lpstrIdentifier;
    p->type = pType;
    p->line_number = m_Interface.GetCompiler()->current_line_number;
    p->next = nullptr;

    return p;
}

ParameterList* Create::ChainParameter(ParameterList *pList, TypeSpecifier *pType, char *lpstrIdentifier)
{
    ParameterList *pos = pList;

    for (; pos->next; pos = pos->next)
        ;
    pos->next = CreateParameter(pType, lpstrIdentifier);

    return pList;
}

ArgumentList* Create::CreateArgumentList(Expression *pExpression)
{
    ArgumentList *pArgumentList = (ArgumentList*)CREATE_UTIL_Malloc(sizeof(ArgumentList));

    pArgumentList->expression = pExpression;
    pArgumentList->next = nullptr;

    return pArgumentList;
}

ArgumentList* Create::ChainArgumentList(ArgumentList *pList, Expression *pExpression)
{
    ArgumentList *pos = pList;

    for (; pos->next; pos = pos->next)
        ;
    pos->next = CreateArgumentList(pExpression);

    return pList;
}

ExpressionList* Create::CreateExpressionList(Expression *pExpression)
{
    ExpressionList *pExpressionList = (ExpressionList*)CREATE_UTIL_Malloc(sizeof(ExpressionList));

    pExpressionList->expression = pExpression;
    pExpressionList->next = nullptr;

    return pExpressionList;
}

ExpressionList* Create::ChainExpressionList(ExpressionList *pList, Expression *pExpression)
{
    ExpressionList *pos;

    for (pos = pList; pos->next; pos = pos->next)
        ;
    pos->next = CreateExpressionList(pExpression);

    return pList;
}

StatementList* Create::CreateStatementList(Statement *pStatement)
{
    StatementList *pStatementList = (StatementList*)CREATE_UTIL_Malloc(sizeof(StatementList));

    pStatementList->statement = pStatement;
    pStatementList->next = nullptr;

    return pStatementList;
}

StatementList* Create::ChainStatementList(StatementList *pList, Statement *pStatement)
{
    if (nullptr == pList)
        return CreateStatementList(pStatement);

    StatementList *pos = pList;

    for (; pos->next; pos = pos->next)
        ;
    pos->next = CreateStatementList(pStatement);

    return pList;
}

TypeSpecifier* Create::CreateTypeSpecifier(DVM_BasicType enType)
{
    TypeSpecifier *pTypeSpecifier = (TypeSpecifier*)CREATE_UTIL_Malloc(sizeof(TypeSpecifier));

    pTypeSpecifier->basic_type = enType;
    pTypeSpecifier->derive = nullptr;

    return pTypeSpecifier;
}

TypeSpecifier* Create::CreateArrayTypeSpecifier(TypeSpecifier *pBase)
{
    TypeDerive *pTypeDerive = m_Util.AllocTypeDerive(ARRAY_DERIVE);

    if (nullptr == pBase->derive)
    {
        pBase->derive = pTypeDerive;
    }
    else
    {
        TypeDerive *pos;
        for (pos = pBase->derive; pos->next != nullptr; pos = pos->next)
            ;
        pos->next = pTypeDerive;
    }

    return pBase;
}

Expression* Create::AllocExpression(ExpressionKind enKind)
{
    Expression *pExpression = (Expression*)CREATE_UTIL_Malloc(sizeof(Expression));

    pExpression->type = nullptr;
    pExpression->kind = enKind;
    pExpression->line_number = m_Interface.GetCompiler()->current_line_number;

    return pExpression;
}

Expression* Create::CreateCommaExpression(Expression *pLeft, Expression *pRight)
{
    if (pRight->kind != ASSIGN_EXPRESSION)
    {
        m_Error.CompileError(m_Interface.GetCompiler()->current_line_number, NOT_LVALUE_ERR, MESSAGE_ARGUMENT_END);
    }

    Expression *pExpression = AllocExpression(COMMA_EXPRESSION);
    pExpression->u.comma.left = pLeft;

    Statement *pStatement = (Statement*)CREATE_UTIL_Malloc(sizeof(Statement));
    pStatement->type = DECLARATION_STATEMENT;
    pStatement->line_number = pLeft->line_number;
    
    Declaration *pDeclaration = (Declaration*)CREATE_UTIL_Malloc(sizeof(Declaration));
    pDeclaration->name = pRight->u.assign_expression.left->u.identifier.name;
    pDeclaration->type = nullptr;
    pDeclaration->initializer = pRight->u.assign_expression.operand;
    pDeclaration->variable_index = -1;
    pStatement->u.declaration_s = pDeclaration;

    pExpression->u.comma.right = pStatement;

    return pExpression;
}

Expression* Create::CreateAssignExpression(Expression *pLeft, AssignmentOperator enOperator, Expression *pOperand)
{
    Expression *pExpression = AllocExpression(ASSIGN_EXPRESSION);

    pExpression->u.assign_expression.left = pLeft;
    pExpression->u.assign_expression.op = enOperator;
    pExpression->u.assign_expression.operand = pOperand;

    return pExpression;
}

Expression* Create::CreateBinaryExpression(ExpressionKind enKind, Expression *pLeft, Expression *pRight)
{
    Expression *pExpression = AllocExpression(enKind);

    pExpression->u.binary_expression.left = pLeft;
    pExpression->u.binary_expression.right = pRight;

    return pExpression;
}

Expression* Create::CreateMinusExpression(Expression *pOperand)
{
    Expression *pExpression = AllocExpression(MINUS_EXPRESSION);

    pExpression->u.minus_expression = pOperand;

    return pExpression;
}

Expression* Create::CreateLogicalNotExpression(Expression *pOperand)
{
    Expression *pExpression = AllocExpression(LOGICAL_NOT_EXPRESSION);

    pExpression->u.logical_not = pOperand;

    return pExpression;
}

Expression* Create::CreateIncDecExpression(Expression *pOperand, ExpressionKind enKind)
{
    Expression *pExpression = AllocExpression(enKind);

    pExpression->u.inc_dec.operand = pOperand;

    return pExpression;
}

Expression* Create::CreateIdentifierExpression(char *lpstrIdentifier)
{
    Expression *pExpression = AllocExpression(IDENTIFIER_EXPRESSION);

    pExpression->u.identifier.name = lpstrIdentifier;

    return pExpression;
}

Expression* Create::CreateFunctionCallExpression(Expression *pFunction, ArgumentList *pArgument)
{
    Expression *pExpression = AllocExpression(FUNCTION_CALL_EXPRESSION);

    pExpression->u.function_call_expression.function = pFunction;
    pExpression->u.function_call_expression.argument = pArgument;

    return pExpression;
}

Expression* Create::CreateBooleanExpression(DVM_Boolean enValue)
{
    Expression *pExpression = AllocExpression(BOOLEAN_EXPRESSION);

    pExpression->u.boolean_value = enValue;

    return pExpression;
}

Expression* Create::CreateNullExpression(void)
{

}

Expression* Create::CreateIndexExpression(Expression *pArrayExpression, Expression* pIndex)
{
    Expression *pExpression = AllocExpression(INDEX_EXPRESSION);

    pExpression->u.index_expression.array = pArrayExpression;
    pExpression->u.index_expression.index = pIndex;

    return pArrayExpression;
}

Expression* Create::CreateArrayLiteralExpression(ExpressionList *pList)
{

}

Expression* Create::CreateArrayCreation(DVM_BasicType enType, ArrayDimension *pArrayDimensionExpressionList, ArrayDimension *pArrayDimension)
{

}

ArrayDimension* Create::CreateArrayDimension(Expression *pExpression)
{

}

ArrayDimension* Create::ChainArrayDimension(ArrayDimension *pList, ArrayDimension *pArrayDimension)
{

}

Statement* Create::CreateIfStatement(Expression *pCondition, Block *pThenBlock, Elsif *pElsifList, Block *pElseBlock)
{
    Statement *pStatement = AllocStatement(IF_STATEMENT);

    pStatement->u.if_s.condition = pCondition;
    pStatement->u.if_s.then_block = pThenBlock;
    pStatement->u.if_s.elsif_list = pElsifList;
    pStatement->u.if_s.else_block = pElseBlock;

    return pStatement;
}

Elsif* Create::ChainElsifList(Elsif *pList, Elsif *pAdd)
{
    Elsif *pos = pList;

    for (; pos->next; pos = pos->next)
        ;
    pos->next = pAdd;

    return pList;
}

Elsif* Create::CreateElsif(Expression *pExpression, Block *pBlock)
{
    Elsif *pElsif = (Elsif*)CREATE_UTIL_Malloc(sizeof(Elsif));

    pElsif->condition = pExpression;
    pElsif->block = pBlock;
    pElsif->next = nullptr;

    return pElsif;
}

Statement* Create::CreateWhileStatement(char *lpstrLabel, Expression *pConfition, Block *pBlock)
{
    Statement *pStatement = AllocStatement(WHILE_STATEMENT);

    pStatement->u.while_s.label = lpstrLabel;
    pStatement->u.while_s.condition = pConfition;
    pStatement->u.while_s.block = pBlock;
    pBlock->type = WHILE_STATEMENT_BLOCK;
    pBlock->parent.statement.statement = pStatement;

    return pStatement;
}

Statement* Create::CreateForStatement(char *lpstrLabel, Expression *pInit, Expression *pCondition, Expression *pPost, Block *pBlock)
{
    Statement *pStatement = AllocStatement(FOR_STATEMENT);

    pStatement->u.for_s.label = lpstrLabel;
    pStatement->u.for_s.init = pInit;
    pStatement->u.for_s.condition = pCondition;
    pStatement->u.for_s.post = pPost;
    pStatement->u.for_s.block = pBlock;
    pBlock->type = FOR_STATEMENT_BLOCK;
    pBlock->parent.statement.statement = pStatement;

    return pStatement;
}

Block* Create::OpenBlock()
{
    Block *pBlock = (Block*)CREATE_UTIL_Malloc(sizeof(Block));

    pBlock->type = UNDEFINED_BLOCK;
    pBlock->outer_block = m_Interface.GetCompiler()->current_block;
    pBlock->declaration_list = nullptr;
    m_Interface.GetCompiler()->current_block = pBlock;

    return pBlock;
}

Block* Create::CloseBlock(Block *pBlock, StatementList *pStatementList)
{
    CREATE_DBG_Assert(pBlock == m_Interface.GetCompiler()->current_block, ("block mismatch."));

    pBlock->statement_list = pStatementList;
    m_Interface.GetCompiler()->current_block = pBlock->outer_block;

    return pBlock;
}

Statement* Create::CreateExpressionStatement(Expression *pExpression)
{
    Statement *pStatement = AllocStatement(EXPRESSION_STATEMENT);

    pStatement->u.expression_s = pExpression;

    return pStatement;
}

Statement* Create::CreateReturnStatement(Expression *pExpression)
{
    Statement *pStatement = AllocStatement(RETURN_STATEMENT);

    pStatement->u.return_s.return_value = pExpression;

    return pStatement;
}

Statement* Create::CreateBreakStatement(char *lpstrLabel)
{
    Statement *pStatement = AllocStatement(BREAK_STATEMENT);

    pStatement->u.break_s.label = lpstrLabel;

    return pStatement;
}

Statement* Create::CreateContinueStatement(char *lpstrLabel)
{
    Statement *pStatement = AllocStatement(CONTINUE_STATEMENT);

    pStatement->u.continue_s.label = lpstrLabel;

    return pStatement;
}

Statement* Create::CreateTryStatement(Block *pTryBlock, char *lpstrException, Block *pCatchBlock, Block *pFinallyBlock)
{
    Statement *pStatement = AllocStatement(TRY_STATEMENT);

    pStatement->u.try_s.try_block = pTryBlock;
    pStatement->u.try_s.catch_block = pCatchBlock;
    pStatement->u.try_s.exception = lpstrException;
    pStatement->u.try_s.finally_block = pFinallyBlock;

    return pStatement;
}

Statement* Create::CreateThrowStatement(Expression *pExpression)
{
    Statement *pStatement = AllocStatement(THROW_STATEMENT);

    pStatement->u.throw_s.exception = pExpression;

    return pStatement;
}

Statement* Create::CreateDeclarationStatement(TypeSpecifier *pType, char *lpstrIdentifier, Expression *pInitializer)
{
    Statement *pStatement = AllocStatement(DECLARATION_STATEMENT);
    Declaration *pDeclaration = AllocDeclaration(m_Util.AllocTypeSpecifier(enType), lpstrIdentifier);

    pDeclaration->initializer = pInitializer;
    pStatement->u.declaration_s = pDeclaration;

    return pStatement;
}

FunctionDefinition* Create::CreateFunctionDefinition(TypeSpecifier *pType, char *lpstrIdentifier, ParameterList *pParameterList, Block *pBlock)
{
    DKC_Compiler *pCompiler = m_Interface.GetCompiler();
    FunctionDefinition *pFD = (FunctionDefinition*)CREATE_UTIL_Malloc(sizeof(FunctionDefinition));

    pFD->type = pType;
    pFD->name = lpstrIdentifier;
    pFD->parameter = pParameterList;
    pFD->block = pBlock;
    pFD->index = pCompiler->function_count++;
    pFD->local_variable_count = 0;
    pFD->local_variable = nullptr;
    pFD->end_line_number = pCompiler->current_line_number;
    pFD->next = nullptr;

    return pFD;
}

Statement* Create::AllocStatement(StatementType enType)
{
    Statement *pStatement = (Statement*)CREATE_UTIL_Malloc(sizeof(Statement));

    pStatement->type = enType;
    pStatement->line_number = m_Interface.GetCompiler()->current_line_number;

    return pStatement;
}