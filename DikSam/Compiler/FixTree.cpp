#include "StdAfx.h"
#include "Debug.h"
#include "Memory.h"
#include "Util.h"
#include "Error.h"
#include "Create.h"
#include "FixTree.h"

FixTree::FixTree(Debug& debug, Memory& memory, Util& util, Error& error, Create& create)
    : m_Debug(debug)
    , m_Memory(memory)
    , m_Util(util)
    , m_Error(error)
    , m_Create(create)
{

}

FixTree::~FixTree()
{

}

void FixTree::Fix(DKC_Compiler *pCompiler)
{

}

Expression* FixTree::FixExpression(Block *pBlock, Expression *pExpression)
{
    if (nullptr == pExpression)
    {
        return nullptr;
    }

    switch (pExpression->kind)
    {
    case BOOLEAN_EXPRESSION :
        pExpression->type = m_Util.AllocTypeSpecifier(DVM_BOOLEAN_TYPE);
        break;

    case INT_EXPRESSION :
        pExpression->type = m_Util.AllocTypeSpecifier(DVM_INT_TYPE);
        break;

    case DOUBLE_EXPRESSION :
        pExpression->type = m_Util.AllocTypeSpecifier(DVM_DOUBLE_TYPE);
        break;

    case STRING_EXPRESSION :
        pExpression->type = m_Util.AllocTypeSpecifier(DVM_STRING_TYPE);
        break;

    case IDENTIFIER_EXPRESSION :
        pExpression = FixIdentifierExpression(pBlock, pExpression);
        break;

    case COMMA_EXPRESSION :
        pExpression = FixCommaExpression(pBlock, pExpression);
        break;

    case ASSIGN_EXPRESSION :
        pExpression = FixAssignExpression(pBlock, pExpression);
        break;

    case ADD_EXPRESSION :
    case SUB_EXPRESSION :
    case MUL_EXPRESSION :
    case DIV_EXPRESSION :
    case MOD_EXPRESSION :
        pExpression = FixMathBinaryExpression(pBlock, pExpression);
        break;

    case EQ_EXPRESSION :
    case NE_EXPRESSION :
    case GT_EXPRESSION :
    case GE_EXPRESSION :
    case LT_EXPRESSION :
    case LE_EXPRESSION :
        pExpression = FixCompareExpression(pBlock, pExpression);
        break;

    case LOGICAL_AND_EXPRESSION :
    case LOGICAL_OR_EXPRESSION :
        pExpression = FixLogicalAndOrExpression(pBlock, pExpression);
        break;

    case MINUS_EXPRESSION :
        pExpression = FixMinusExpression(pBlock, pExpression);
        break;

    case LOGICAL_NOT_EXPRESSION :
        pExpression = FixLogicalNotExpression(pBlock, pExpression);
        break;

    case FUNCTION_CALL_EXPRESSION :
        pExpression = FixFunctionCallExpression(pBlock, pExpression);
        break;

    case INCREMENT_EXPRESSION :
    case DECREMENT_EXPRESSION :
        pExpression = FixIncDecExpression(pBlock, pExpression);
        break;

    default :
        FIXTREE_DBG_Assert(0, ("bad case. kind..", pExpression->kind));
    }

    return pExpression;
}

Expression* FixTree::FixIdentifierExpression(Block *pBlock, Expression *pExpression)
{
    Declaration *pDeclaration = m_Util.SearchDeclaration(pExpression->u.identifier.name, pBlock);

    if (pDeclaration)
    {
        pExpression->type = pDeclaration->type;
        pExpression->u.identifier.is_function = DVM_FALSE;
        pExpression->u.identifier.u.declaration = pDeclaration;
        
        return pExpression;
    }

    FunctionDefinition *pFunctionDefinition = m_Util.SearchFunction(pExpression->u.identifier.name);
    if (nullptr == pFunctionDefinition)
    {
        m_Error.CompileError(pExpression->line_number,
            IDENTIFIER_NOT_FOUND_ERR,
            STRING_MESSAGE_ARGUMENT, "name", pExpression->u.identifier.name,
            MESSAGE_ARGUMENT_END);
    }

    pExpression->type = pFunctionDefinition->type;
    pExpression->u.identifier.is_function = DVM_TRUE;
    pExpression->u.identifier.u.function = pFunctionDefinition;

    return pExpression;
}

Expression* FixTree::FixCommaExpression(Block *pBlock, Expression *pExpression)
{
    return nullptr;
}

Expression* FixTree::FixAssignExpression(Block *pBlock, Expression *pExpression)
{
    return nullptr;
}

Expression* FixTree::FixMathBinaryExpression(Block *pBlock, Expression *pExpression)
{
    return nullptr;
}

Expression* FixTree::FixCompareExpression(Block *pBlock, Expression *pExpression)
{
    return nullptr;
}

Expression* FixTree::FixLogicalAndOrExpression(Block *pBlock, Expression *pExpression)
{
    return nullptr;
}

Expression* FixTree::FixMinusExpression(Block *pBlock, Expression *pExpression)
{
    return nullptr;
}

Expression* FixTree::FixLogicalNotExpression(Block *pBlock, Expression *pExpression)
{
    return nullptr;
}

Expression* FixTree::FixFunctionCallExpression(Block *pBlock, Expression *pExpression)
{
    return nullptr;
}

Expression* FixTree::FixIncDecExpression(Block *pBlock, Expression *pExpression)
{
    return nullptr;
}

Expression* FixTree::EvalMathExpressionInt(Expression *pExpression, int left, int right)
{
    switch (pExpression->kind)
    {
    case ADD_EXPRESSION :
        pExpression->u.int_value = left + right;
        break;

    case SUB_EXPRESSION :
        pExpression->u.int_value = left - right;
        break;

    case MUL_EXPRESSION :
        pExpression->u.int_value = left * right;
        break;

    case DIV_EXPRESSION :
        pExpression->u.int_value = left / right;
        break;

    case MOD_EXPRESSION :
        pExpression->u.int_value = left % right;
        break;

    default :
        FIXTREE_DBG_Assert(0, ("pExpression->kind..", pExpression->kind));
    }

    pExpression->type = m_Util.AllocTypeSpecifier(DVM_INT_TYPE);
    pExpression->kind = INT_EXPRESSION;

    return pExpression;
}

Expression* FixTree::EvalMathExpressionDouble(Expression *pExpression, double left, double right)
{
    switch (pExpression->kind)
    {
    case ADD_EXPRESSION :
        pExpression->u.double_value = left + right;
        break;

    case SUB_EXPRESSION :
        pExpression->u.double_value = left - right;
        break;

    case MUL_EXPRESSION :
        pExpression->u.double_value = left * right;
        break;

    case DIV_EXPRESSION :
        pExpression->u.double_value = left / right;
        break;

    case MOD_EXPRESSION :
        pExpression->u.double_value = std::fmod(left, right);
        break;

    default :
        FIXTREE_DBG_Assert(0, ("pExpression->kind..", pExpression->kind));
    }

    pExpression->type = m_Util.AllocTypeSpecifier(DVM_DOUBLE_TYPE);
    pExpression->kind = DOUBLE_EXPRESSION;

    return pExpression;
}

Expression* FixTree::EvalMathExpression(Block *pBlock, Expression *pExpression)
{
    if (INT_EXPRESSION == pExpression->u.binary_expression.left->kind
        && INT_EXPRESSION == pExpression->u.binary_expression.right->kind)
    {
        pExpression = EvalMathExpressionInt(pExpression,
            pExpression->u.binary_expression.left->u.int_value,
            pExpression->u.binary_expression.right->u.int_value);
    }
    else if (DOUBLE_EXPRESSION == pExpression->u.binary_expression.left->kind
        && DOUBLE_EXPRESSION == pExpression->u.binary_expression.right->kind)
    {
        pExpression = EvalMathExpressionDouble(pExpression,
            pExpression->u.binary_expression.left->u.double_value,
            pExpression->u.binary_expression.right->u.double_value);
    }
    else if (INT_EXPRESSION == pExpression->u.binary_expression.left->kind
        && DOUBLE_EXPRESSION == pExpression->u.binary_expression.right->kind)
    {
        pExpression = EvalMathExpressionDouble(pExpression,
            pExpression->u.binary_expression.left->u.int_value,
            pExpression->u.binary_expression.right->u.double_value);
    }
    else if (DOUBLE_EXPRESSION == pExpression->u.binary_expression.left->kind
        && INT_EXPRESSION == pExpression->u.binary_expression.right->kind)
    {
        pExpression = EvalMathExpressionDouble(pExpression,
            pExpression->u.binary_expression.left->u.double_value,
            pExpression->u.binary_expression.right->u.int_value);
    }
    else if (STRING_EXPRESSION == pExpression->u.binary_expression.left->kind
        && ADD_EXPRESSION == pExpression->kind)
    {
        pExpression = ChainString(pExpression);
    }

    return pExpression;
}

Expression* FixTree::EvalCompareExpressionBoolean(Expression *pExpression, DVM_Boolean left, DVM_Boolean right)
{
    switch (pExpression->kind)
    {
    case EQ_EXPRESSION :
        pExpression->u.boolean_value = (left == right ? DVM_TRUE : DVM_FALSE);
        break;

    case NE_EXPRESSION :
        pExpression->u.boolean_value = (left != right ? DVM_TRUE : DVM_FALSE);
        break;
    }

    pExpression->type = m_Util.AllocTypeSpecifier(DVM_BOOLEAN_TYPE);
    pExpression->kind = BOOLEAN_EXPRESSION;

    return pExpression;
}

Expression* FixTree::EvalCompareExpressionInt(Expression *pExpression, int left, int right)
{
    switch (pExpression->kind)
    {
    case EQ_EXPRESSION :
        pExpression->u.boolean_value = (left == right ? DVM_TRUE : DVM_FALSE);
        break;

    case NE_EXPRESSION :
        pExpression->u.boolean_value = (left != right ? DVM_TRUE : DVM_FALSE);
        break;

    case GT_EXPRESSION :
        pExpression->u.boolean_value = (left > right ? DVM_TRUE : DVM_FALSE);
        break;

    case GE_EXPRESSION :
        pExpression->u.boolean_value = (left >= right ? DVM_TRUE : DVM_FALSE);
        break;

    case LT_EXPRESSION :
        pExpression->u.boolean_value = (left < right ? DVM_TRUE : DVM_FALSE);
        break;

    case LE_EXPRESSION :
        pExpression->u.boolean_value = (left <= right ? DVM_TRUE : DVM_FALSE);
        break;

    default :
        FIXTREE_DBG_Assert(0, ("pExpression->kind..", pExpression));
    }

    pExpression->type = m_Util.AllocTypeSpecifier(DVM_BOOLEAN_TYPE);
    pExpression->kind = BOOLEAN_EXPRESSION;

    return pExpression;
}

Expression* FixTree::EvalCompareExpressionDouble(Expression *pExpression, double left, double right)
{
    switch (pExpression->kind)
    {
    case EQ_EXPRESSION :
        pExpression->u.boolean_value = (double(std::abs(left - right)) < eps ? DVM_TRUE : DVM_FALSE);
        break;

    case NE_EXPRESSION :
        pExpression->u.boolean_value = (double(std::abs(left - right)) > eps ? DVM_TRUE : DVM_FALSE);
        break;

    case GT_EXPRESSION :
        pExpression->u.boolean_value = (left - right > eps ? DVM_TRUE : DVM_FALSE);
        break;

    case GE_EXPRESSION :
        pExpression->u.boolean_value = (left - right >= eps ? DVM_TRUE : DVM_FALSE);
        break;

    case LT_EXPRESSION :
        pExpression->u.boolean_value = (right - left > eps ? DVM_TRUE : DVM_FALSE);
        break;

    case LE_EXPRESSION :
        pExpression->u.boolean_value = (right - left >= eps ? DVM_TRUE : DVM_FALSE);
        break;

    default :
        FIXTREE_DBG_Assert(0, ("pExpression->kind..", pExpression));
    }

    pExpression->type = m_Util.AllocTypeSpecifier(DVM_BOOLEAN_TYPE);
    pExpression->kind = BOOLEAN_EXPRESSION;

    return pExpression;
}

Expression* FixTree::EvalCompareExpressionString(Expression *pExpression, DVM_Char *left, DVM_Char *right)
{
    switch (pExpression->kind)
    {
    case EQ_EXPRESSION :
        pExpression->u.boolean_value = (std::basic_string<DVM_Char>(left) == right ? DVM_TRUE : DVM_FALSE);
        break;

    case NE_EXPRESSION :
        pExpression->u.boolean_value = (std::basic_string<DVM_Char>(left) != right ? DVM_TRUE : DVM_FALSE);
        break;

    case GT_EXPRESSION :
        pExpression->u.boolean_value = (std::basic_string<DVM_Char>(left) > right ? DVM_TRUE : DVM_FALSE);
        break;

    case GE_EXPRESSION :
        pExpression->u.boolean_value = (std::basic_string<DVM_Char>(left) >= right ? DVM_TRUE : DVM_FALSE);
        break;

    case LT_EXPRESSION :
        pExpression->u.boolean_value = (std::basic_string<DVM_Char>(left) < right ? DVM_TRUE : DVM_FALSE);
        break;

    case LE_EXPRESSION :
        pExpression->u.boolean_value = (std::basic_string<DVM_Char>(left) <= right ? DVM_TRUE : DVM_FALSE);
        break;

    default :
        FIXTREE_DBG_Assert(0, ("pExpression->kind..", pExpression));
    }

    m_Memory.Free(left);
    m_Memory.Free(right);

    pExpression->type = m_Util.AllocTypeSpecifier(DVM_BOOLEAN_TYPE);
    pExpression->kind = BOOLEAN_EXPRESSION;

    return pExpression;
}

Expression* FixTree::EvalCompareExpression(Expression *pExpression)
{
    if (BOOLEAN_EXPRESSION == pExpression->u.binary_expression.left->kind
        && BOOLEAN_EXPRESSION == pExpression->u.binary_expression.right->kind)
    {
        pExpression = EvalCompareExpressionBoolean(pExpression,
            pExpression->u.binary_expression.left->u.boolean_value,
            pExpression->u.binary_expression.right->u.boolean_value);
    }
    else if (INT_EXPRESSION == pExpression->u.binary_expression.left->kind
        && INT_EXPRESSION == pExpression->u.binary_expression.right->kind)
    {
        pExpression = EvalCompareExpressionInt(pExpression,
            pExpression->u.binary_expression.left->u.int_value,
            pExpression->u.binary_expression.right->u.int_value);
    }
    else if (DOUBLE_EXPRESSION == pExpression->u.binary_expression.left->kind
        && DOUBLE_EXPRESSION == pExpression->u.binary_expression.right->kind)
    {
        pExpression = EvalCompareExpressionDouble(pExpression,
            pExpression->u.binary_expression.left->u.double_value,
            pExpression->u.binary_expression.right->u.double_value);
    }
    else if (INT_EXPRESSION == pExpression->u.binary_expression.left->kind
        && DOUBLE_EXPRESSION == pExpression->u.binary_expression.right->kind)
    {
        pExpression = EvalCompareExpressionDouble(pExpression,
            pExpression->u.binary_expression.left->u.int_value,
            pExpression->u.binary_expression.right->u.double_value);
    }
    else if (DOUBLE_EXPRESSION == pExpression->u.binary_expression.left->kind
        && INT_EXPRESSION == pExpression->u.binary_expression.right->kind)
    {
        pExpression = EvalCompareExpressionDouble(pExpression,
            pExpression->u.binary_expression.left->u.double_value,
            pExpression->u.binary_expression.right->u.int_value);
    }
    else if (STRING_EXPRESSION == pExpression->u.binary_expression.left->kind
        && STRING_EXPRESSION == pExpression->u.binary_expression.right->kind)
    {
        pExpression = EvalCompareExpressionString(pExpression,
            pExpression->u.binary_expression.left->u.string_value,
            pExpression->u.binary_expression.right->u.string_value);
    }

    return pExpression;
}

Expression* FixTree::AllocCastExpression(CastType enType, Expression *pOperand)
{
    Expression *pCastExpression = m_Create.AllocExpression(CAST_EXPRESSION);

    pCastExpression->line_number = pOperand->line_number;
    pCastExpression->u.cast.type = enType;
    pCastExpression->u.cast.operand = pOperand;

    switch (enType)
    {
    case INT_TO_DOUBLE_CAST :
        pCastExpression->type = m_Util.AllocTypeSpecifier(DVM_DOUBLE_TYPE);
        break;

    case DOUBLE_TO_INT_CAST :
        pCastExpression->type = m_Util.AllocTypeSpecifier(DVM_INT_TYPE);
        break;

    case BOOLEAN_TO_STRING_CAST :
    case INT_TO_STRING_CAST :
    case DOUBLE_TO_STRING_CAST :
        pCastExpression->type = m_Util.AllocTypeSpecifier(DVM_STRING_TYPE);
        break;
    }

    return pCastExpression;
}

Expression* FixTree::CreateAssignCast(Expression *pSrc, TypeSpecifier *pDest)
{
    if (pSrc->type->derive != nullptr || pDest->derive != nullptr)
    {
        m_Error.CompileError(pSrc->line_number, DERIVE_TYPE_CAST_ERR, MESSAGE_ARGUMENT_END);
    }

    if (pSrc->type->basic_type == pDest->basic_type)
    {
        return pSrc;
    }

    if (DVM_INT_TYPE == pSrc->type->basic_type && DVM_DOUBLE_TYPE == pDest->basic_type)
    {
        Expression *pCastExpression = AllocCastExpression(INT_TO_DOUBLE_CAST, pSrc);
        return pCastExpression;
    }
    else if (DVM_DOUBLE_TYPE == pSrc->type->basic_type && DVM_INT_TYPE == pDest->basic_type)
    {
        Expression *pCastExpression = AllocCastExpression(DOUBLE_TO_INT_CAST, pSrc);
        return pCastExpression;
    }
    else
    {
        CastMismatchError(pSrc->line_number, pSrc->type->basic_type, pDest->basic_type);
    }

    return nullptr;
}

Expression* FixTree::ChainString(Expression *pExpression)
{
    DVM_Char *pLeftStr = pExpression->u.binary_expression.left->u.string_value;
    DVM_Char *pRightStr = m_Util.ExpressionToString(pExpression->u.binary_expression.right);

    if (nullptr == pRightStr)
    {
        return pExpression;
    }

    int iLen = std::basic_string<DVM_Char>(pLeftStr).length()
             + std::basic_string<DVM_Char>(pRightStr).length();

    DVM_Char *pNewStr = (DVM_Char*)FIXTREE_MEM_Malloc(sizeof(DVM_Char)* (iLen + 1));
    wcscpy_s(pNewStr, iLen + 1, (std::basic_string<DVM_Char>(pLeftStr) + std::basic_string<DVM_Char>(pRightStr)).c_str());

    m_Memory.Free(pLeftStr);
    m_Memory.Free(pRightStr);

    pExpression->kind = STRING_EXPRESSION;
    pExpression->type = m_Util.AllocTypeSpecifier(DVM_STRING_TYPE);
    pExpression->u.string_value = pNewStr;

    return pExpression;
}

Expression* FixTree::CastBinaryExpression(Expression *pExpression)
{
    if (IsInt(pExpression->u.binary_expression.left->type) && IsDouble(pExpression->u.binary_expression.right->type))
    {
        Expression *pCastExpression = AllocCastExpression(INT_TO_DOUBLE_CAST, pExpression->u.binary_expression.left);
        pExpression->u.binary_expression.left = pCastExpression;
    }
    else if (IsDouble(pExpression->u.binary_expression.left->type) && IsInt(pExpression->u.binary_expression.right->type))
    {
        Expression *pCastExpression = AllocCastExpression(INT_TO_DOUBLE_CAST, pExpression->u.binary_expression.right);
        pExpression->u.binary_expression.right = pCastExpression;
    }
    else if (IsString(pExpression->u.binary_expression.left->type) && IsBoolean(pExpression->u.binary_expression.right->type))
    {
        Expression *pCastExpression = AllocCastExpression(BOOLEAN_TO_STRING_CAST, pExpression->u.binary_expression.right);
        pExpression->u.binary_expression.right = pCastExpression;
    }
    else if (IsString(pExpression->u.binary_expression.left->type) && IsInt(pExpression->u.binary_expression.right->type))
    {
        Expression *pCastExpression = AllocCastExpression(INT_TO_STRING_CAST, pExpression->u.binary_expression.right);
        pExpression->u.binary_expression.right = pCastExpression;
    }
    else if (IsString(pExpression->u.binary_expression.left->type) && IsDouble(pExpression->u.binary_expression.right->type))
    {
        Expression *pCastExpression = AllocCastExpression(DOUBLE_TO_STRING_CAST, pExpression->u.binary_expression.right);
        pExpression->u.binary_expression.right = pCastExpression;
    }

    return pExpression;
}

void FixTree::CheckArgument(Block *pBlock, FunctionDefinition *pFunctionDefinition, Expression *pExpression)
{

}

void FixTree::CastMismatchError(int iLine, DVM_BasicType enSrc, DVM_BasicType enDest)
{
    char *pSrcName = m_Util.GetBasicTypeName(enSrc);
    char *pDestName = m_Util.GetBasicTypeName(enDest);

    m_Error.CompileError(iLine,
        CAST_MISMATCH_ERR,
        STRING_MESSAGE_ARGUMENT, "src", pSrcName,
        STRING_MESSAGE_ARGUMENT, "dest", pDestName,
        MESSAGE_ARGUMENT_END);
}