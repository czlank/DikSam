#pragma once

#include "DikSamc.h"

class Debug;
class Memory;
class Util;
class Error;
class Create;

class FixTree
{
#define FIXTREE_DBG_Panic(arg)              (m_Debug.Panic(__FILE__, __LINE__, arg))
#define FIXTREE_DBG_Assert(expression, arg) ((expression) ? (void)(0) : (m_Debug.Assert(__FILE__, __LINE__, #expression, arg)))
#define FIXTREE_MEM_Malloc(size)            (m_Memory.Malloc(__FILE__, __LINE__, size))
#define FIXTREE_MEM_Realloc(ptr, size)      (m_Memory.Realloc(__FILE__, __LINE__, ptr, size))

public:
    FixTree(Debug& debug, Memory& memory, Util& util, Error& error, Create& create);
    ~FixTree();

    void Fix(DKC_Compiler *pCompiler);

private:
    Expression* FixIdentifierExpression(Block *pBlock, Expression *pExpression);
    Expression* FixCommaExpression(Block *pBlock, Expression *pExpression);
    Expression* FixAssignExpression(Block *pBlock, Expression *pExpression);
    Expression* FixMathBinaryExpression(Block *pBlock, Expression *pExpression);

    Expression* EvalMathExpressionInt(Expression *pExpression, int left, int right);
    Expression* EvalMathExpressionDouble(Expression *pExpression, double left, double right);
    Expression* EvalMathExpression(Block *pBlock, Expression *pExpression);
    Expression* EvalCompareExpressionBoolean(Expression *pExpression, DVM_Boolean left, DVM_Boolean right);
    Expression* EvalCompareExpressionInt(Expression *pExpression, int left, int right);
    Expression* EvalCompareExpressionDouble(Expression *pExpression, double left, double right);

    Expression* AllocCastExpression(CastType enType, Expression *pOperand);
    Expression* CreateAssignCast(Expression *pSrc, TypeSpecifier *pDest);
    Expression* ChainString(Expression *pExpression);
    Expression* CastBinaryExpression(Expression *pExpression);

    void CastMismatchError(int iLine, DVM_BasicType enSrc, DVM_BasicType enDest);

    inline bool IsInt(TypeSpecifier *pType) { return (DVM_INT_TYPE == pType->basic_type && nullptr == pType->derive); }
    inline bool IsDouble(TypeSpecifier *pType) { return (DVM_DOUBLE_TYPE == pType->basic_type && nullptr == pType->derive); }
    inline bool IsBoolean(TypeSpecifier *pType) { return (DVM_BOOLEAN_TYPE == pType->basic_type && nullptr == pType->derive); }
    inline bool IsString(TypeSpecifier *pType) { return (DVM_STRING_TYPE == pType->basic_type && nullptr == pType->derive); }

private:
    Debug       &m_Debug;
    Memory      &m_Memory;
    Util        &m_Util;
    Error       &m_Error;
    Create      &m_Create;
};