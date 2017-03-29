#pragma once

#include "Debug.h"
#include "Error.h"
#include "Util.h"
#include "DikSamc.h"

class Create
{
#define CREATE_UTIL_Malloc(size)            (m_Util.Malloc(__FILE__, __LINE__, size))
#define CREATE_DBG_Assert(expression, arg)  ((expression) ? (void)(0) : (m_Debug.Assert(__FILE__, __LINE__, #expression, arg)))

public:
    Create(Debug& debug, Error& error, Util& util);
    ~Create();

    // 声明
    Declaration* AllocDeclaration(TypeSpecifier *pType, char *lpstrIdentifier);
    DeclarationList* ChainDeclaration(DeclarationList *pList, Declaration *pDecl);
    // 函数定义
    void FunctionDefine(DVM_BasicType enType, char *lpstrIdentifier, ParameterList *pParameterList, Block *pBlock);

    // 形参表
    ParameterList* CreateParameter(DVM_BasicType enType, char *lpstrIdentifier);
    ParameterList* ChainParameter(ParameterList *pList, DVM_BasicType enType, char *lpstrIdentifier);
    // 实参表
    ArgumentList* CreateArgumentList(Expression *pExpression);
    ArgumentList* ChainArgumentList(ArgumentList *pList, Expression *pExpression);

    // 语句表
    StatementList* CreateStatementList(Statement *pStatement);
    StatementList* ChainStatementList(StatementList *pList, Statement *pStatement);

    // 表达式
    Expression* AllocExpression(ExpressionKind enKind);
    Expression* CreateCommaExpression(Expression *pLeft, Expression *pRight);
    Expression* CreateAssignExpression(Expression *pLeft, AssignmentOperator enOperator, Expression *pOperand);
    Expression* CreateBinaryExpression(ExpressionKind enKind, Expression *pLeft, Expression *pRight);
    Expression* CreateMinusExpression(Expression *pOperand);
    Expression* CreateLogicalNotExpression(Expression *pOperand);
    Expression* CreateIncDecExpression(Expression *pOperand, ExpressionKind enKind);
    Expression* CreateIdentifierExpression(char *lpstrIdentifier);
    Expression* CreateFunctionCallExpression(Expression *pFunction, ArgumentList *pArgument);
    Expression* CreateBooleanExpression(DVM_Boolean enValue);

    // 语句
    Statement* CreateIfStatement(Expression *pCondition, Block *pThenBlock, Elsif *pElsifList, Block *pElseBlock);
    Elsif*     ChainElsifList(Elsif *pList, Elsif *pAdd);
    Elsif*     CreateElsif(Expression *pExpression, Block *pBlock);
    Statement* CreateWhileStatement(char *lpstrLabel, Expression *pConfition, Block *pBlock);
    Statement* CreateForStatement(char *lpstrLabel, Expression *pInit, Expression *pCondition, Expression *pPost, Block *pBlock);
    Block*     OpenBlock();
    Block*     CloseBlock(Block *pBlock, StatementList *pStatementList);

private:
    FunctionDefinition* CreateFunctionDefinition(DVM_BasicType enType, char *lpstrIdentifier, ParameterList *pParameterList, Block *pBlock);
    Statement* AllocStatement(StatementType enType);

private:
    Debug&  m_Debug;
    Error&  m_Error;
    Util&   m_Util;
};