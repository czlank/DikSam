#include "StdAfx.h"
#include "Debug.h"
#include "Memory.h"
#include "Util.h"
#include "Error.h"
#include "Create.h"
#include "Interface.h"
#include "FixTree.h"

FixTree::FixTree(Debug& debug, Memory& memory, Util& util, Error& error, Create& create, Interface& refInterface)
    : m_Debug(debug)
    , m_Memory(memory)
    , m_Util(util)
    , m_Error(error)
    , m_Create(create)
    , m_Interface(refInterface)
{

}

FixTree::~FixTree()
{

}

void FixTree::operator () (DKC_Compiler *pCompiler)
{
    FixStatementList(nullptr, pCompiler->statement_list, nullptr);

    FunctionDefinition *pFunctionDefinition = pCompiler->function_list;
    for (; pFunctionDefinition; pFunctionDefinition = pFunctionDefinition->next)
    {
        if (nullptr == pFunctionDefinition->block)
            continue;

        AddParameterAsDeclaration(pFunctionDefinition);
        FixStatementList(pFunctionDefinition->block, pFunctionDefinition->block->statement_list, pFunctionDefinition);
        AddReturnFunction(pFunctionDefinition);
    }

    DeclarationList *pDeclarationList = pCompiler->declaration_list;
    int iVariableCount = 0;

    for (; pDeclarationList; pDeclarationList = pDeclarationList->next)
    {
        pDeclarationList->declaration->variable_index = iVariableCount++;
    }
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

void FixTree::FixStatement(Block *pBlock, Statement *pStatement, FunctionDefinition *pFunctionDefinition)
{
    switch (pStatement->type)
    {
    case EXPRESSION_STATEMENT :
        FixExpression(pBlock, pStatement->u.expression_s);
        break;

    case IF_STATEMENT :
        FixIfStatement(pBlock, &pStatement->u.if_s, pFunctionDefinition);
        break;

    case WHILE_STATEMENT :
        FixExpression(pBlock, pStatement->u.while_s.condition);
        FixStatementList(pStatement->u.while_s.block, pStatement->u.while_s.block->statement_list, pFunctionDefinition);
        break;

    case FOR_STATEMENT :
        FixExpression(pBlock, pStatement->u.for_s.init);
        FixExpression(pBlock, pStatement->u.for_s.condition);
        FixExpression(pBlock, pStatement->u.for_s.post);
        FixStatementList(pStatement->u.for_s.block, pStatement->u.for_s.block->statement_list, pFunctionDefinition);
        break;

    case RETURN_STATEMENT :
        FixReturnStatement(pBlock, &pStatement->u.return_s, pFunctionDefinition);
        break;

    case BREAK_STATEMENT :
        break;

    case CONTINUE_STATEMENT :
        break;

    case TRY_STATEMENT :
        break;

    case THROW_STATEMENT :
        FixExpression(pBlock, pStatement->u.throw_s.exception);
        break;

    case DECLARATION_STATEMENT :
        AddDeclaration(pBlock, pStatement->u.declaration_s, pFunctionDefinition, pStatement->line_number);
        FixExpression(pBlock, pStatement->u.declaration_s->initializer);

        if (pStatement->u.declaration_s->initializer)
        {
            pStatement->u.declaration_s->initializer = CreateAssignCast(pStatement->u.declaration_s->initializer, pStatement->u.declaration_s->type);
        }

        break;

    default :
        FIXTREE_DBG_Assert(0, ("bad case. type..", pStatement->type));
    }
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
    pExpression->u.comma.left = FixExpression(pBlock, pExpression->u.comma.left);
    pExpression->u.comma.right = FixExpression(pBlock, pExpression->u.comma.right);
    pExpression->type = pExpression->u.comma.right->type;

    return pExpression;
}

Expression* FixTree::FixAssignExpression(Block *pBlock, Expression *pExpression)
{
    if (pExpression->u.assign_expression.left->kind != IDENTIFIER_EXPRESSION)
    {
        m_Error.CompileError(pExpression->u.assign_expression.left->line_number,
            NOT_LVALUE_ERR, MESSAGE_ARGUMENT_END);
    }

    pExpression->u.assign_expression.left = FixExpression(pBlock, pExpression->u.assign_expression.left);
    Expression *pOperand = FixExpression(pBlock, pExpression->u.assign_expression.operand);
    pExpression->u.assign_expression.operand = CreateAssignCast(pOperand, pExpression->u.assign_expression.left->type);
    pExpression->type = pExpression->u.assign_expression.left->type;

    return pExpression;
}

Expression* FixTree::FixMathBinaryExpression(Block *pBlock, Expression *pExpression)
{
    pExpression->u.binary_expression.left = FixExpression(pBlock, pExpression->u.binary_expression.left);
    pExpression->u.binary_expression.right = FixExpression(pBlock, pExpression->u.binary_expression.right);

    pExpression = EvalMathExpression(pBlock, pExpression);
    if (INT_EXPRESSION == pExpression->kind || DOUBLE_EXPRESSION == pExpression->kind || STRING_EXPRESSION == pExpression->kind)
    {
        return pExpression;
    }

    pExpression = CastBinaryExpression(pExpression);

    if (IsInt(pExpression->u.binary_expression.left->type) && IsInt(pExpression->u.binary_expression.right->type))
    {
        pExpression->type = m_Util.AllocTypeSpecifier(DVM_INT_TYPE);
    }
    else if (IsDouble(pExpression->u.binary_expression.left->type) && IsDouble(pExpression->u.binary_expression.right->type))
    {
        pExpression->type = m_Util.AllocTypeSpecifier(DVM_DOUBLE_TYPE);
    }
    else if (IsString(pExpression->u.binary_expression.left->type) && IsString(pExpression->u.binary_expression.right->type))
    {
        pExpression->type = m_Util.AllocTypeSpecifier(DVM_STRING_TYPE);
    }
    else
    {
        m_Error.CompileError(pExpression->line_number,
            MATH_TYPE_MISMATCH_ERR, MESSAGE_ARGUMENT_END);
    }

    return pExpression;
}

Expression* FixTree::FixCompareExpression(Block *pBlock, Expression *pExpression)
{
    pExpression->u.binary_expression.left = FixExpression(pBlock, pExpression->u.binary_expression.left);
    pExpression->u.binary_expression.right = FixExpression(pBlock, pExpression->u.binary_expression.right);
    
    pExpression = EvalCompareExpression(pExpression);
    if (BOOLEAN_EXPRESSION == pExpression->kind)
    {
        return pExpression;
    }

    pExpression = CastBinaryExpression(pExpression);

    if ((pExpression->u.binary_expression.left->type->basic_type != pExpression->u.binary_expression.right->type->basic_type)
        || pExpression->u.binary_expression.left->type->derive
        || pExpression->u.binary_expression.right->type->derive)
    {
        m_Error.CompileError(pExpression->line_number,
            COMPARE_TYPE_MISMATCH_ERR, MESSAGE_ARGUMENT_END);
    }

    pExpression->type = m_Util.AllocTypeSpecifier(DVM_BOOLEAN_TYPE);

    return pExpression;
}

Expression* FixTree::FixLogicalAndOrExpression(Block *pBlock, Expression *pExpression)
{
    pExpression->u.binary_expression.left = FixExpression(pBlock, pExpression->u.binary_expression.left);
    pExpression->u.binary_expression.right = FixExpression(pBlock, pExpression->u.binary_expression.right);

    if (IsBoolean(pExpression->u.binary_expression.left->type) && IsBoolean(pExpression->u.binary_expression.right->type))
    {
        pExpression->type = m_Util.AllocTypeSpecifier(DVM_BOOLEAN_TYPE);
    }
    else
    {
        m_Error.CompileError(pExpression->line_number,
            LOGICAL_TYPE_MISMATCH_ERR, MESSAGE_ARGUMENT_END);
    }

    return pExpression;
}

Expression* FixTree::FixMinusExpression(Block *pBlock, Expression *pExpression)
{
    pExpression->u.minus_expression = FixExpression(pBlock, pExpression->u.minus_expression);

    if (!IsInt(pExpression->u.minus_expression->type) && !IsDouble(pExpression->u.minus_expression->type))
    {
        m_Error.CompileError(pExpression->line_number,
            MINUS_TYPE_MISMATCH_ERR, MESSAGE_ARGUMENT_END);
    }

    pExpression->type = pExpression->u.minus_expression->type;

    if (INT_EXPRESSION == pExpression->u.minus_expression->kind)
    {
        pExpression->kind = INT_EXPRESSION;
        pExpression->u.int_value = -pExpression->u.minus_expression->u.int_value;
    }
    else if (DOUBLE_EXPRESSION == pExpression->u.minus_expression->kind)
    {
        pExpression->kind = DOUBLE_EXPRESSION;
        pExpression->u.double_value = -pExpression->u.minus_expression->u.double_value;
    }

    return pExpression;
}

Expression* FixTree::FixLogicalNotExpression(Block *pBlock, Expression *pExpression)
{
    pExpression->u.logical_not = FixExpression(pBlock, pExpression->u.logical_not);

    if (BOOLEAN_EXPRESSION == pExpression->u.logical_not->kind)
    {
        pExpression->kind = BOOLEAN_EXPRESSION;
        pExpression->u.boolean_value = pExpression->u.logical_not->u.boolean_value == DVM_TRUE ? DVM_FALSE : DVM_TRUE;

        return pExpression;
    }

    if (!IsBoolean(pExpression->u.logical_not->type))
    {
        m_Error.CompileError(pExpression->line_number,
            LOGICAL_NOT_TYPE_MISMATCH_ERR, MESSAGE_ARGUMENT_END);
    }

    pExpression->type = pExpression->u.logical_not->type;

    return pExpression;
}

Expression* FixTree::FixFunctionCallExpression(Block *pBlock, Expression *pExpression)
{
    Expression *pFunctionExpression = FixExpression(pBlock, pExpression->u.function_call_expression.function);

    if (pFunctionExpression->kind != IDENTIFIER_EXPRESSION)
    {
        m_Error.CompileError(pExpression->line_number,
            FUNCTION_NOT_IDENTIFIER_ERR, MESSAGE_ARGUMENT_END);
    }

    FunctionDefinition *pFunctionDefinition = m_Util.SearchFunction(pFunctionExpression->u.identifier.name);
    if (nullptr == pFunctionDefinition)
    {
        m_Error.CompileError(pExpression->line_number,
            FUNCTION_NOT_FOUND_ERR,
            STRING_MESSAGE_ARGUMENT, "name", pFunctionExpression->u.identifier.name,
            MESSAGE_ARGUMENT_END);
    }

    CheckArgument(pBlock, pFunctionDefinition, pExpression);

    pExpression->type = m_Util.AllocTypeSpecifier(pFunctionDefinition->type->basic_type);
    pExpression->type->derive = pFunctionDefinition->type->derive;

    return pExpression;
}

Expression* FixTree::FixIncDecExpression(Block *pBlock, Expression *pExpression)
{
    pExpression->u.inc_dec.operand = FixExpression(pBlock, pExpression->u.inc_dec.operand);

    if (!IsInt(pExpression->u.inc_dec.operand->type))
    {
        m_Error.CompileError(pExpression->line_number,
            INC_DEC_TYPE_MISMATCH_ERR, MESSAGE_ARGUMENT_END);
    }

    pExpression->type = pExpression->u.inc_dec.operand->type;

    return pExpression;
}

void FixTree::FixIfStatement(Block *pBlock, IfStatement *pIfStatement, FunctionDefinition *pFunctionDefinition)
{
    FixExpression(pBlock, pIfStatement->condition);
    FixStatementList(pIfStatement->then_block, pIfStatement->then_block->statement_list, pFunctionDefinition);

    for (Elsif *pos = pIfStatement->elsif_list; pos; pos = pos->next)
    {
        FixExpression(pBlock, pos->condition);

        if (pos->block)
        {
            FixStatementList(pos->block, pos->block->statement_list, pFunctionDefinition);
        }
    }

    if (pIfStatement->else_block)
    {
        FixStatementList(pIfStatement->else_block, pIfStatement->else_block->statement_list, pFunctionDefinition);
    }
}

void FixTree::FixReturnStatement(Block *pBlock, ReturnStatement *pReturnStatement, FunctionDefinition *pFunctionDefinition)
{
    Expression *pReturnValue = FixExpression(pBlock, pReturnStatement->return_value);

    if (nullptr == pReturnValue)
    {
        FIXTREE_DBG_Assert(nullptr == pFunctionDefinition->type->derive, ("closure is not supported."));

        switch (pFunctionDefinition->type->basic_type)
        {
        case DVM_BOOLEAN_TYPE :
            pReturnValue = m_Create.AllocExpression(BOOLEAN_EXPRESSION);
            pReturnValue->u.boolean_value = DVM_FALSE;
            break;

        case DVM_INT_TYPE :
            pReturnValue = m_Create.AllocExpression(INT_EXPRESSION);
            pReturnValue->u.int_value = 0;
            break;

        case DVM_DOUBLE_TYPE :
            pReturnValue = m_Create.AllocExpression(DOUBLE_EXPRESSION);
            pReturnValue->u.double_value = 0.0;
            break;

        case DVM_STRING_TYPE :
            pReturnValue = m_Create.AllocExpression(STRING_EXPRESSION);
            pReturnValue->u.string_value = L"";
            break;
        }

        pReturnStatement->return_value = pReturnValue;
        return;
    }

    Expression *pCastExpression = CreateAssignCast(pReturnStatement->return_value, pFunctionDefinition->type);
    pReturnStatement->return_value = pCastExpression;
}

void FixTree::FixStatementList(Block *pBlock, StatementList *pStatementList, FunctionDefinition *pFunctionDefinition)
{
    for (StatementList *pos = pStatementList; pos; pos = pos->next)
    {
        FixStatement(pBlock, pos->statement, pFunctionDefinition);
    }
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

    DVM_Char *pNewStr = (DVM_Char*)STRING_UTIL_Malloc(sizeof(DVM_Char)* (iLen + 1));
    wcscpy_s(pNewStr, iLen + 1, (std::basic_string<DVM_Char>(pLeftStr) + std::basic_string<DVM_Char>(pRightStr)).c_str());

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
    ParameterList *pParameter = pFunctionDefinition->parameter;
    ArgumentList *pArgument = pExpression->u.function_call_expression.argument;

    for (; pParameter && pArgument; pParameter = pParameter->next, pArgument = pArgument->next)
    {
        pArgument->expression = FixExpression(pBlock, pArgument->expression);
        pArgument->expression = CreateAssignCast(pArgument->expression, pParameter->type);
    }

    if (pParameter || pArgument)
    {
        m_Error.CompileError(pExpression->line_number,
            ARGUMENT_COUNT_MISMATCH_ERR, MESSAGE_ARGUMENT_END);
    }
}

void FixTree::AddLocalVariable(FunctionDefinition *pFunctionDefinition, Declaration *pDeclaration)
{
    pFunctionDefinition->local_variable = (Declaration**)FIXTREE_MEM_Realloc(pFunctionDefinition->local_variable,
        sizeof(Declaration*) * (pFunctionDefinition->local_variable_count + 1));

    pFunctionDefinition->local_variable[pFunctionDefinition->local_variable_count] = pDeclaration;
    pDeclaration->variable_index = pFunctionDefinition->local_variable_count++;
}

void FixTree::AddDeclaration(Block *pBlock, Declaration *pDeclaration, FunctionDefinition *pFunctionDefinition, int iLine)
{
    if (m_Util.SearchDeclaration(pDeclaration->name, pBlock))
    {
        m_Error.CompileError(iLine,
            VARIABLE_MULTIPLE_DEFINE_ERR,
            STRING_MESSAGE_ARGUMENT, "name", pDeclaration->name,
            MESSAGE_ARGUMENT_END);
    }

    if (pBlock)
    {
        pBlock->declaration_list = m_Create.ChainDeclaration(pBlock->declaration_list, pDeclaration);
        AddLocalVariable(pFunctionDefinition, pDeclaration);
        pDeclaration->is_local = DVM_TRUE;
    }
    else
    {
        DKC_Compiler *pCompiler = m_Interface.GetCompiler();
        pCompiler->declaration_list = m_Create.ChainDeclaration(pCompiler->declaration_list, pDeclaration);
        pDeclaration->is_local = DVM_FALSE;
    }
}

void FixTree::AddParameterAsDeclaration(FunctionDefinition *pFunctionDefinition)
{
    for (ParameterList *pParam = pFunctionDefinition->parameter; pParam; pParam = pParam->next)
    {
        if (m_Util.SearchDeclaration(pParam->name, pFunctionDefinition->block))
        {
            m_Error.CompileError(pParam->line_number,
                PARAMETER_MULTIPLE_DEFINE_ERR,
                STRING_MESSAGE_ARGUMENT, "name", pParam->name,
                MESSAGE_ARGUMENT_END);
        }

        Declaration *pDeclaration = m_Create.AllocDeclaration(pParam->type, pParam->name);
        AddDeclaration(pFunctionDefinition->block, pDeclaration, pFunctionDefinition, pParam->line_number);
    }
}

void FixTree::AddReturnFunction(FunctionDefinition *pFunctionDefinition)
{
    StatementList **ppLastStatementList = nullptr;

    if (nullptr == pFunctionDefinition->block->statement_list)
    {
        ppLastStatementList = &pFunctionDefinition->block->statement_list;
    }
    else
    {
        StatementList *pLastStatement = pFunctionDefinition->block->statement_list;
        for (; pLastStatement->next; pLastStatement = pLastStatement->next)
            ;

        if (RETURN_STATEMENT == pLastStatement->statement->type)
        {
            return;
        }

        ppLastStatementList = &pLastStatement->next;
    }

    Statement *pRetStatement = m_Create.CreateReturnStatement(nullptr);
    FixReturnStatement(pFunctionDefinition->block, &pRetStatement->u.return_s, pFunctionDefinition);
    *ppLastStatementList = m_Create.CreateStatementList(pRetStatement);
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