#pragma once

#include "DikSamc.h"

class Debug;
class Error;
class Util;
class Interface;

class Create
{
#define CREATE_UTIL_Malloc(size)            (m_Util.Malloc(__FILE__, __LINE__, size))
#define CREATE_DBG_Assert(expression, arg)  ((expression) ? (void)(0) : (m_Debug.Assert(__FILE__, __LINE__, #expression, arg)))

public:
    Create(Debug& debug, Error& error, Util& util, Interface& rInterface);
    ~Create();

    // ����
    Declaration* AllocDeclaration(TypeSpecifier *pType, char *lpstrIdentifier);
    DeclarationList* ChainDeclaration(DeclarationList *pList, Declaration *pDecl);

    // ��������
    void FunctionDefine(TypeSpecifier *pType, char *lpstrIdentifier, ParameterList *pParameterList, Block *pBlock);

    // �βα�
    ParameterList* CreateParameter(TypeSpecifier *pType, char *lpstrIdentifier);
    ParameterList* ChainParameter(ParameterList *pList, TypeSpecifier *pType, char *lpstrIdentifier);
    // ʵ�α�
    ArgumentList* CreateArgumentList(Expression *pExpression);
    ArgumentList* ChainArgumentList(ArgumentList *pList, Expression *pExpression);

    // ���ʽ��
    ExpressionList* CreateExpressionList(Expression *pExpression);
    ExpressionList* ChainExpressionList(ExpressionList *pList, Expression *pExpression);

    // ����
    StatementList* CreateStatementList(Statement *pStatement);
    StatementList* ChainStatementList(StatementList *pList, Statement *pStatement);

    // ����
    TypeSpecifier* CreateTypeSpecifier(DVM_BasicType enType);
    TypeSpecifier* CreateArrayTypeSpecifier(TypeSpecifier *pBase);

    // ���ʽ
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
    Expression* CreateNullExpression(void);
    Expression* CreateIndexExpression(Expression *pArrayExpression, Expression* pIndex);
    Expression* CreateArrayLiteralExpression(ExpressionList *pList);
    Expression* CreateArrayCreation(DVM_BasicType enType, ArrayDimension *pArrayDimensionExpressionList, ArrayDimension *pArrayDimension);

    // ����
    ArrayDimension* CreateArrayDimension(Expression *pExpression);
    ArrayDimension* ChainArrayDimension(ArrayDimension *pList, ArrayDimension *pArrayDimension);

    // ���
    Statement* CreateIfStatement(Expression *pCondition, Block *pThenBlock, Elsif *pElsifList, Block *pElseBlock);
    Elsif*     ChainElsifList(Elsif *pList, Elsif *pAdd);
    Elsif*     CreateElsif(Expression *pExpression, Block *pBlock);
    Statement* CreateWhileStatement(char *lpstrLabel, Expression *pConfition, Block *pBlock);
    Statement* CreateForStatement(char *lpstrLabel, Expression *pInit, Expression *pCondition, Expression *pPost, Block *pBlock);
    Block*     OpenBlock();
    Block*     CloseBlock(Block *pBlock, StatementList *pStatementList);
    Statement* CreateExpressionStatement(Expression *pExpression);
    Statement* CreateReturnStatement(Expression *pExpression);
    Statement* CreateBreakStatement(char *lpstrLabel);
    Statement* CreateContinueStatement(char *lpstrLabel);
    Statement* CreateTryStatement(Block *pTryBlock, char *lpstrException, Block *pCatchBlock, Block *pFinallyBlock);
    Statement* CreateThrowStatement(Expression *pExpression);
    Statement* CreateDeclarationStatement(TypeSpecifier *pType, char *lpstrIdentifier, Expression *pInitializer);

private:
    FunctionDefinition* CreateFunctionDefinition(TypeSpecifier *pType, char *lpstrIdentifier, ParameterList *pParameterList, Block *pBlock);
    Statement* AllocStatement(StatementType enType);

private:
    Debug       &m_Debug;
    Error       &m_Error;
    Util        &m_Util;
    Interface   &m_Interface;
};