#include "StdAfx.h"
#include "Debug.h"
#include "Memory.h"
#include "Util.h"
#include "Error.h"
#include "Create.h"
#include "Interface.h"
#include "FixTree.h"

#ifdef DBG_assert
#undef DBG_assert
#endif
#define DBG_assert(expression, arg) ((expression) ? (void)(0) : (m_Debug.Assert(__FILE__, __LINE__, #expression, arg)))

#ifdef DBG_panic
#undef DBG_panic
#endif
#define DBG_panic(arg)                      (m_Debug.Panic(__FILE__, __LINE__, arg))

#ifdef MEM_malloc
#undef MEM_malloc
#endif
#define MEM_malloc(size)                    (m_Memory.Malloc(__FILE__, __LINE__, size))

#ifdef MEM_realloc
#undef MEM_realloc
#endif
#define MEM_realloc(ptr, size)              (m_Memory.Realloc(__FILE__, __LINE__, ptr, size))

#ifdef MEM_free
#undef MEM_free
#endif
#define MEM_free(ptr)                       (m_Memory.Free(ptr))

#ifdef MEM_strdup
#undef MEM_strdup
#endif
#define MEM_strdup(ptr)                     (m_Memory.StrDUP(__FILE__, __LINE__, ptr))

#ifdef dkc_malloc
#undef dkc_malloc
#endif
#define dkc_malloc(size)                    (m_Util.Malloc(__FILE__, __LINE__, size))

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
    FixClassList(pCompiler);

    for (FunctionDefinition *pos = pCompiler->function_list; pos; pos = pos->next)
    {
        ReservFunctionIndex(pCompiler, pos);
    }

    FixStatementList(nullptr, pCompiler->statement_list, nullptr);

    for (FunctionDefinition *pos = pCompiler->function_list; pos; pos = pos->next)
    {
        if (nullptr == pos->class_definition)
        {
            FixFunction(pos);
        }
    }

    int iVariableCount = 0;
    for (DeclarationList *pDeclarationList = pCompiler->declaration_list; pDeclarationList; pDeclarationList = pDeclarationList->next)
    {
        pDeclarationList->declaration->variable_index = iVariableCount++;
    }
}

Expression* FixTree::FixExpression(Block *pBlock, Expression *pExpression, Expression *pParentExpression)
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

    case MEMBER_EXPRESSION :
        pExpression = FixMemberExpression(pBlock, pExpression);
        break;

    case NULL_EXPRESSION :
        pExpression->type = m_Util.AllocTypeSpecifier(DVM_NULL_TYPE);
        break;

    case THIS_EXPRESSION :
        pExpression = FixThisExpression(pExpression);
        break;

    case SUPER_EXPRESSION :
        pExpression = FixSuperExpression(pExpression, pParentExpression);
        break;

    case ARRAY_LITERAL_EXPRESSION :
        pExpression = FixArrayLiteralExpression(pBlock, pExpression);
        break;

    case INDEX_EXPRESSION :
        pExpression = FixIndexExpression(pBlock, pExpression);
        break;

    case INCREMENT_EXPRESSION :
    case DECREMENT_EXPRESSION :
        pExpression = FixIncDecExpression(pBlock, pExpression);
        break;

    case INSTANCEOF_EXPRESSION :
        pExpression = FixInstanceofExpression(pBlock, pExpression);
        break;

    case DOWN_CAST_EXPRESSION :
        pExpression = FixDownCastExpression(pBlock, pExpression);
        break;

    case CAST_EXPRESSION :
        break;

    case UP_CAST_EXPRESSION :
        break;

    case NEW_EXPRESSION :
        pExpression = FixNewExpression(pBlock, pExpression);
        break;

    case ARRAY_CREATION_EXPRESSION :
        pExpression = FixArrayCreationExpression(pBlock, pExpression);
        break;

    default :
        DBG_assert(0, ("bad case. kind..", pExpression->kind));
    }

    FixTypeSpecifier(pExpression->type);

    return pExpression;
}

void FixTree::FixStatement(Block *pBlock, Statement *pStatement, FunctionDefinition *pFunctionDefinition)
{
    switch (pStatement->type)
    {
    case EXPRESSION_STATEMENT :
        pStatement->u.expression_s = FixExpression(pBlock, pStatement->u.expression_s, nullptr);
        break;

    case IF_STATEMENT :
        FixIfStatement(pBlock, &pStatement->u.if_s, pFunctionDefinition);
        break;

    case WHILE_STATEMENT :
        FixWhileStatement(pBlock, &pStatement->u.while_s, pFunctionDefinition);
        break;

    case DO_WHILE_STATEMENT :
        FixDoWhileStatement(pBlock, &pStatement->u.do_while_s, pFunctionDefinition);
        break;

    case FOR_STATEMENT :
        FixForStatement(pBlock, &pStatement->u.for_s, pFunctionDefinition);
        break;

    case RETURN_STATEMENT :
        FixReturnStatement(pBlock, pStatement, pFunctionDefinition);
        break;

    case BREAK_STATEMENT :
        break;

    case CONTINUE_STATEMENT :
        break;

    case TRY_STATEMENT :
        break;

    case THROW_STATEMENT :
        FixExpression(pBlock, pStatement->u.throw_s.exception, nullptr);
        break;

    case DECLARATION_STATEMENT :
        AddDeclaration(pBlock, pStatement->u.declaration_s, pFunctionDefinition, pStatement->line_number, false);
        FixTypeSpecifier(pStatement->u.declaration_s->type);

        do 
        {
            Expression *pExp = pStatement->u.declaration_s->initializer;
            std::stack<Declaration*> stkDeclaration;

            while (pExp && COMMA_EXPRESSION == pExp->kind)
            {
                stkDeclaration.push(pExp->u.comma.right->u.declaration_s);
                pExp = pExp->u.comma.left;
            }

            while (!stkDeclaration.empty())
            {
                Declaration *pDeclaration = stkDeclaration.top();

                AddDeclaration(pBlock, pDeclaration, pFunctionDefinition, pStatement->line_number, false);
                stkDeclaration.pop();
            }
        } while (0);

        FixExpression(pBlock, pStatement->u.declaration_s->initializer, nullptr);

        if (pStatement->u.declaration_s->initializer)
        {
            pStatement->u.declaration_s->initializer = FixExpression(pBlock, pStatement->u.declaration_s->initializer, nullptr);
            pStatement->u.declaration_s->initializer = CreateAssignCast(pStatement->u.declaration_s->initializer, pStatement->u.declaration_s->type);
        }

        break;

    default :
        DBG_assert(0, ("bad case. type..", pStatement->type));
    }
}

Expression* FixTree::FixIdentifierExpression(Block *pBlock, Expression *pExpression)
{
    Declaration *pDeclaration = m_Util.SearchDeclaration(pExpression->u.identifier.name, pBlock);

    if (pDeclaration)
    {
        pExpression->type = pDeclaration->type;
        pExpression->u.identifier.kind = VARIABLE_IDENTIFIER;
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

    pExpression->type = CreateFunctionDeriveType(pFunctionDefinition);
    pExpression->u.identifier.kind = FUNCTION_IDENTIFIER;
    pExpression->u.identifier.u.function.function_definition = pFunctionDefinition;
    pExpression->u.identifier.u.function.function_index = ReservFunctionIndex(m_Interface.GetCurrentCompiler(), pFunctionDefinition);
    
    FixTypeSpecifier(pExpression->type);

    return pExpression;
}

Expression* FixTree::FixCommaExpression(Block *pBlock, Expression *pExpression)
{
    pExpression->u.comma.left = FixExpression(pBlock, pExpression->u.comma.left, pExpression);
    pExpression->u.comma.right->u.declaration_s->initializer = FixExpression(pBlock, pExpression->u.comma.right->u.declaration_s->initializer, pExpression);
    pExpression->u.comma.right->u.declaration_s->type = m_Util.AllocTypeSpecifier(pExpression->u.comma.right->u.declaration_s->initializer->type->basic_type);
    pExpression->type = pExpression->u.comma.right->u.declaration_s->type;

    return pExpression;
}

Expression* FixTree::FixAssignExpression(Block *pBlock, Expression *pExpression)
{
    if (pExpression->u.assign_expression.left->kind != IDENTIFIER_EXPRESSION
        && pExpression->u.assign_expression.left->kind != INDEX_EXPRESSION
        && pExpression->u.assign_expression.left->kind != MEMBER_EXPRESSION)
    {
        m_Error.CompileError(pExpression->u.assign_expression.left->line_number,
            NOT_LVALUE_ERR, MESSAGE_ARGUMENT_END);
    }

    pExpression->u.assign_expression.left = FixExpression(pBlock, pExpression->u.assign_expression.left, pExpression);
    Expression *pOperand = FixExpression(pBlock, pExpression->u.assign_expression.operand, pExpression);
    pExpression->u.assign_expression.operand = CreateAssignCast(pOperand, pExpression->u.assign_expression.left->type);
    pExpression->type = pExpression->u.assign_expression.left->type;

    return pExpression;
}

Expression* FixTree::FixMathBinaryExpression(Block *pBlock, Expression *pExpression)
{
    pExpression->u.binary_expression.left = FixExpression(pBlock, pExpression->u.binary_expression.left, pExpression);
    pExpression->u.binary_expression.right = FixExpression(pBlock, pExpression->u.binary_expression.right, pExpression);

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
    else if (ADD_EXPRESSION == pExpression->kind
        && (IsString(pExpression->u.binary_expression.left->type) && IsString(pExpression->u.binary_expression.right->type)
            || IsString(pExpression->u.binary_expression.left->type) && NULL_EXPRESSION == pExpression->u.binary_expression.right->kind))
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
    pExpression->u.binary_expression.left = FixExpression(pBlock, pExpression->u.binary_expression.left, pExpression);
    pExpression->u.binary_expression.right = FixExpression(pBlock, pExpression->u.binary_expression.right, pExpression);
    
    pExpression = EvalCompareExpression(pExpression);
    if (BOOLEAN_EXPRESSION == pExpression->kind)
    {
        return pExpression;
    }

    pExpression = CastBinaryExpression(pExpression);

    if (!(m_Util.CompareType(pExpression->u.binary_expression.left->type, pExpression->u.binary_expression.right->type)
        || (IsObject(pExpression->u.binary_expression.left->type) && NULL_EXPRESSION == pExpression->u.binary_expression.right->kind)
        || (IsObject(pExpression->u.binary_expression.right->type) && NULL_EXPRESSION == pExpression->u.binary_expression.left->kind)))
    {
        m_Error.CompileError(pExpression->line_number, COMPARE_TYPE_MISMATCH_ERR, MESSAGE_ARGUMENT_END);
    }

    pExpression->type = m_Util.AllocTypeSpecifier(DVM_BOOLEAN_TYPE);

    return pExpression;
}

Expression* FixTree::FixLogicalAndOrExpression(Block *pBlock, Expression *pExpression)
{
    pExpression->u.binary_expression.left = FixExpression(pBlock, pExpression->u.binary_expression.left, pExpression);
    pExpression->u.binary_expression.right = FixExpression(pBlock, pExpression->u.binary_expression.right, pExpression);

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
    pExpression->u.minus_expression = FixExpression(pBlock, pExpression->u.minus_expression, pExpression);

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
    pExpression->u.logical_not = FixExpression(pBlock, pExpression->u.logical_not, pExpression);

    if (BOOLEAN_EXPRESSION == pExpression->u.logical_not->kind)
    {
        pExpression->kind = BOOLEAN_EXPRESSION;
        pExpression->u.boolean_value = pExpression->u.logical_not->u.boolean_value == DVM_TRUE ? DVM_FALSE : DVM_TRUE;
        pExpression->type = m_Util.AllocTypeSpecifier(DVM_BOOLEAN_TYPE);

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
    TypeSpecifier arrayBase;
    TypeSpecifier *pArrayBase = nullptr;

    DKC_Compiler *pCompiler = m_Interface.GetCurrentCompiler();

    Expression *pFunctionExpression = FixExpression(pBlock, pExpression->u.function_call_expression.function, pExpression);
    pExpression->u.function_call_expression.function = pFunctionExpression;

    FunctionDefinition *pFunctionDefinition = nullptr;

    if (IDENTIFIER_EXPRESSION == pFunctionExpression->kind)
    {
        pFunctionDefinition = m_Util.SearchFunction(pFunctionExpression->u.identifier.name);
    }
    else if (MEMBER_EXPRESSION == pFunctionExpression->kind)
    {
        if (IsArray(pFunctionExpression->u.member_expression.expression->type))
        {
            pFunctionDefinition = &pCompiler->array_method[pFunctionExpression->u.member_expression.method_index];

            arrayBase = *pFunctionExpression->u.member_expression.expression->type;
            arrayBase.derive = pFunctionExpression->u.member_expression.expression->type->derive->next;
            pArrayBase = &arrayBase;
        }
        else if (IsString(pFunctionExpression->u.member_expression.expression->type))
        {
            pFunctionDefinition = &pCompiler->string_method[pFunctionExpression->u.member_expression.method_index];
        }
        else
        {
            if (FIELD_MEMBER == pFunctionExpression->u.member_expression.declaration->kind)
            {
                m_Error.CompileError(pExpression->line_number, FIELD_CAN_NOT_CALL_ERR,
                    STRING_MESSAGE_ARGUMENT, "member_name", pFunctionExpression->u.member_expression.declaration->u.field.name,
                    MESSAGE_ARGUMENT_END);
            }

            if (pFunctionExpression->u.member_expression.declaration->u.method.is_constructor)
            {
                Expression *pObj = pFunctionExpression->u.member_expression.expression;

                if (pObj->kind != SUPER_EXPRESSION && pObj->kind != THIS_EXPRESSION)
                {
                    m_Error.CompileError(pExpression->line_number, CONSTRUCTOR_CALLED_ERR,
                        STRING_MESSAGE_ARGUMENT, "member_name", pFunctionExpression->u.member_expression.declaration->u.field.name,
                        MESSAGE_ARGUMENT_END);
                }
            }

            pFunctionDefinition = pFunctionExpression->u.member_expression.declaration->u.method.function_definition;
        }
    }

    if (nullptr == pFunctionDefinition)
    {
        m_Error.CompileError(pExpression->line_number, FUNCTION_NOT_FOUND_ERR,
            STRING_MESSAGE_ARGUMENT, "name", pFunctionExpression->u.identifier.name,
            MESSAGE_ARGUMENT_END);
    }

    TypeSpecifier *pFuncType = pFunctionDefinition->type;
    ParameterList *pFuncParam = pFunctionDefinition->parameter;

    CheckArgument(pBlock, pExpression->line_number, pFuncParam, pExpression->u.function_call_expression.argument, pArrayBase);
    pExpression->type = m_Util.AllocTypeSpecifier(pFuncType->basic_type);
    *pExpression->type = *pFuncType;
    pExpression->type->derive = pFuncType->derive;

    if (DVM_CLASS_TYPE == pFuncType->basic_type)
    {
        pExpression->type->class_ref.identifier = pFuncType->class_ref.identifier;
        FixTypeSpecifier(pExpression->type);
    }

    return pExpression;
}

Expression* FixTree::FixIncDecExpression(Block *pBlock, Expression *pExpression)
{
    pExpression->u.inc_dec.operand = FixExpression(pBlock, pExpression->u.inc_dec.operand, pExpression);

    if (!IsInt(pExpression->u.inc_dec.operand->type))
    {
        m_Error.CompileError(pExpression->line_number,
            INC_DEC_TYPE_MISMATCH_ERR, MESSAGE_ARGUMENT_END);
    }

    pExpression->type = pExpression->u.inc_dec.operand->type;

    return pExpression;
}

Expression* FixTree::FixArrayLiteralExpression(Block *pBlock, Expression *pExpression)
{
    ExpressionList *pExpressionList = pExpression->u.array_literal;

    if (nullptr == pExpressionList)
    {
        m_Error.CompileError(pExpression->line_number, ARRAY_LITERAL_EMPTY_ERR, MESSAGE_ARGUMENT_END);
    }

    pExpressionList->expression = FixExpression(pBlock, pExpressionList->expression, pExpression);

    TypeSpecifier *pTypeSpecifier = pExpressionList->expression->type;

    for (ExpressionList *pos = pExpressionList->next; pos; pos = pos->next)
    {
        pos->expression = FixExpression(pBlock, pos->expression, pExpression);
        pos->expression = CreateAssignCast(pos->expression, pTypeSpecifier);
    }

    pExpression->type = m_Util.AllocTypeSpecifier(pTypeSpecifier->basic_type);
    *pExpression->type = *pTypeSpecifier;
    pExpression->type->derive = m_Util.AllocTypeDerive(ARRAY_DERIVE);
    pExpression->type->derive->next = pTypeSpecifier->derive;

    return pExpression;
}

Expression* FixTree::FixIndexExpression(Block *pBlock, Expression *pExpression)
{
    IndexExpression *pIndexExpression = &pExpression->u.index_expression;

    pIndexExpression->array = FixExpression(pBlock, pIndexExpression->array, pExpression);
    pIndexExpression->index = FixExpression(pBlock, pIndexExpression->index, pExpression);

    if (pIndexExpression->array->type->derive && ARRAY_DERIVE == pIndexExpression->array->type->derive->tag)
    {
        pExpression->type = m_Util.AllocTypeSpecifier(pIndexExpression->array->type);
        pExpression->type->derive = pIndexExpression->array->type->derive->next;
    }
    else
    {
        m_Error.CompileError(pExpression->line_number, INDEX_LEFT_OPERAND_NOT_ARRAY_ERR, MESSAGE_ARGUMENT_END);
    }

    if (!IsInt(pIndexExpression->index->type))
    {
        m_Error.CompileError(pExpression->line_number, INDEX_NOT_INT_ERR, MESSAGE_ARGUMENT_END);
    }

    return pExpression;
}

Expression* FixTree::FixArrayCreationExpression(Block *pBlock, Expression *pExpression)
{
    TypeDerive *pHeadTypeDerive = nullptr;

    FixTypeSpecifier(pExpression->u.array_creation.type);

    for (ArrayDimension *pos = pExpression->u.array_creation.dimension; pos; pos = pos->next)
    {
        if (pos->expression)
        {
            pos->expression = FixExpression(pBlock, pos->expression, pExpression);
            if (!IsInt(pos->expression->type))
            {
                m_Error.CompileError(pExpression->line_number, ARRAY_SIZE_NOT_INT_ERR, MESSAGE_ARGUMENT_END);
            }
        }

        TypeDerive *pTypeDerive = m_Util.AllocTypeDerive(ARRAY_DERIVE);
        pTypeDerive->next = pHeadTypeDerive;
        pHeadTypeDerive = pTypeDerive;
    }

    pExpression->type = m_Util.AllocTypeSpecifier(pExpression->u.array_creation.type);
    pExpression->type->derive = pHeadTypeDerive;

    return pExpression;
}

Expression* FixTree::FixClassMemberExpression(Expression *pExpression, Expression *pObj, char *lpstrMemberName)
{
    FixTypeSpecifier(pObj->type);

    MemberDeclaration *pMemberDeclaration = m_Util.SearchMember(pObj->type->class_ref.class_definition, lpstrMemberName);

    if (nullptr == pMemberDeclaration)
    {
        m_Error.CompileError(pExpression->line_number, MEMBER_NOT_FOUND_ERR,
            STRING_MESSAGE_ARGUMENT, "class_name", pObj->type->class_ref.class_definition->name,
            STRING_MESSAGE_ARGUMENT, "member_name", lpstrMemberName,
            MESSAGE_ARGUMENT_END);
    }

    CheckMemberAccessibility(pObj->line_number, pObj->type->class_ref.class_definition, pMemberDeclaration, lpstrMemberName);
    
    pExpression->u.member_expression.declaration = pMemberDeclaration;

    switch (pMemberDeclaration->kind)
    {
    case METHOD_MEMBER :
        {
            ClassDefinition *pTargetInterface = nullptr;
            int iInterfaceIndex;

            pExpression->type = CreateFunctionDeriveType(pMemberDeclaration->u.method.function_definition);

            if (DVM_CLASS_DEFINITION == pObj->type->class_ref.class_definition->class_or_interface
                && IsInterfaceMethod(pObj->type->class_ref.class_definition, pMemberDeclaration, &pTargetInterface, &iInterfaceIndex))
            {
                pExpression->u.member_expression.expression = CreateUpCast(pObj, pTargetInterface, iInterfaceIndex);
            }
        }
        break;

    case FIELD_MEMBER :
        if (SUPER_EXPRESSION == pObj->kind)
        {
            m_Error.CompileError(pExpression->line_number, FIELD_OF_SUPER_REFERENCED_ERR, MESSAGE_ARGUMENT_END);
        }

        pExpression->type = pMemberDeclaration->u.field.type;
    }

    return pExpression;
}

Expression* FixTree::FixArrayMethodExpression(Expression *pExpression, Expression *pObj, char *lpstrMemberName)
{
    DKC_Compiler *pCompiler = m_Interface.GetCurrentCompiler();

    int i = 0;
    for (; i < pCompiler->array_method_count; i++)
    {
        if (std::string(pCompiler->array_method[i].name) == lpstrMemberName)
            break;
    }

    if (i == pCompiler->array_method_count)
    {
        m_Error.CompileError(pExpression->line_number, ARRAY_METHOD_NOT_FOUND_ERR,
            STRING_MESSAGE_ARGUMENT, "name", lpstrMemberName,
            MESSAGE_ARGUMENT_END);
    }

    FunctionDefinition *pFunctionDefinition = &pCompiler->array_method[i];

    pExpression->u.member_expression.method_index = i;
    pExpression->type = CreateFunctionDeriveType(pFunctionDefinition);

    return pExpression;
}

Expression* FixTree::FixStringMethodExpression(Expression *pExpression, Expression *pObj, char *lpstrMemberName)
{
    DKC_Compiler *pCompiler = m_Interface.GetCurrentCompiler();

    int i = 0;
    for (; i < pCompiler->string_method_count; i++)
    {
        if (std::string(pCompiler->string_method[i].name) == lpstrMemberName)
            break;
    }

    if (i == pCompiler->string_method_count)
    {
        m_Error.CompileError(pExpression->line_number, STRING_METHOD_NOT_FOUND_ERR,
            STRING_MESSAGE_ARGUMENT, "name", lpstrMemberName,
            MESSAGE_ARGUMENT_END);
    }

    FunctionDefinition *pFunctionDefinition = &pCompiler->string_method[i];

    pExpression->u.member_expression.method_index = i;
    pExpression->type = CreateFunctionDeriveType(pFunctionDefinition);

    return pExpression;
}

Expression* FixTree::FixMemberExpression(Block *pBlock, Expression *pExpression)
{
    Expression *pObj = pExpression->u.member_expression.expression = FixExpression(pBlock, pExpression->u.member_expression.expression, pExpression);

    if (IsClassObject(pObj->type))
    {
        return FixClassMemberExpression(pExpression, pObj, pExpression->u.member_expression.member_name);
    }
    else if (IsArray(pObj->type))
    {
        return FixArrayMethodExpression(pExpression, pObj, pExpression->u.member_expression.member_name);
    }
    else if (IsString(pObj->type))
    {
        return FixStringMethodExpression(pExpression, pObj, pExpression->u.member_expression.member_name);
    }
    else
    {
        m_Error.CompileError(pExpression->line_number, MEMBER_EXPRESSION_TYPE_ERR, MESSAGE_ARGUMENT_END);
    }

    return nullptr;
}

Expression* FixTree::FixThisExpression(Expression *pExpression)
{
    ClassDefinition *pClassDefinition = m_Interface.GetCurrentCompiler()->current_class_definition;

    if (nullptr == pClassDefinition)
    {
        m_Error.CompileError(pExpression->line_number, THIS_OUT_OF_CLASS_ERR, MESSAGE_ARGUMENT_END);
    }

    TypeSpecifier *pTypeSpecifier = m_Util.AllocTypeSpecifier(DVM_CLASS_TYPE);

    pTypeSpecifier->class_ref.identifier = pClassDefinition->name;
    pTypeSpecifier->class_ref.class_definition = pClassDefinition;
    pExpression->type = pTypeSpecifier;

    return pExpression;
}

Expression* FixTree::FixSuperExpression(Expression *pExpression, Expression *pParent)
{
    ClassDefinition *pClassDefinition = m_Interface.GetCurrentCompiler()->current_class_definition;

    if (nullptr == pClassDefinition)
    {
        m_Error.CompileError(pExpression->line_number, SUPER_OUT_OF_CLASS_ERR, MESSAGE_ARGUMENT_END);
    }

    if (nullptr == pClassDefinition->super_class)
    {
        m_Error.CompileError(pExpression->line_number, HASNT_SUPER_CLASS_ERR, MESSAGE_ARGUMENT_END);
    }

    if (nullptr == pParent || pParent->kind != MEMBER_EXPRESSION)
    {
        m_Error.CompileError(pParent ? pParent->line_number : -1, SUPER_NOT_IN_MEMBER_EXPRESSION_ERR, MESSAGE_ARGUMENT_END);
    }

    TypeSpecifier *pTypeSpecifier = m_Util.AllocTypeSpecifier(DVM_CLASS_TYPE);

    pTypeSpecifier->class_ref.identifier = pClassDefinition->super_class->name;
    pTypeSpecifier->class_ref.class_definition = pClassDefinition->super_class;
    pExpression->type = pTypeSpecifier;

    return pExpression;
}

Expression* FixTree::FixInstanceofExpression(Block *pBlock, Expression *pExpression)
{
    pExpression->u.instanceof.operand = FixExpression(pBlock, pExpression->u.instanceof.operand, pExpression);
    FixTypeSpecifier(pExpression->u.instanceof.type);

    Expression *pOperand = pExpression->u.instanceof.operand;
    TypeSpecifier *pTarget = pExpression->u.instanceof.type;

    if (!IsObject(pOperand->type))
    {
        m_Error.CompileError(pExpression->line_number, INSTANCEOF_OPERAND_NOT_REFERENCE_ERR, MESSAGE_ARGUMENT_END);
    }

    if (!IsObject(pTarget))
    {
        m_Error.CompileError(pExpression->line_number, INSTANCEOF_TYPE_NOT_REFERENCE_ERR, MESSAGE_ARGUMENT_END);
    }

    if (!IsClassObject(pOperand->type) || !IsClassObject(pTarget))
    {
        m_Error.CompileError(pExpression->line_number, INSTANCEOF_FOR_NOT_CLASS_ERR, MESSAGE_ARGUMENT_END);
    }

    if (m_Util.CompareType(pOperand->type, pTarget))
    {
        m_Error.CompileError(pExpression->line_number, INSTANCEOF_MUST_RETURN_TRUE_ERR, MESSAGE_ARGUMENT_END);
    }

    if (IsSuperClass(pOperand->type->class_ref.class_definition, pTarget->class_ref.class_definition, nullptr, nullptr))
    {
        m_Error.CompileError(pExpression->line_number, INSTANCEOF_MUST_RETURN_TRUE_ERR, MESSAGE_ARGUMENT_END);
    }

    if (DVM_CLASS_DEFINITION == pTarget->class_ref.class_definition->class_or_interface
        && !IsSuperClass(pTarget->class_ref.class_definition, pOperand->type->class_ref.class_definition, nullptr, nullptr))
    {
        m_Error.CompileError(pExpression->line_number, INSTANCEOF_MUST_RETURN_FALSE_ERR, MESSAGE_ARGUMENT_END);
    }

    pExpression->type = m_Util.AllocTypeSpecifier(DVM_BOOLEAN_TYPE);

    return pExpression;
}

Expression* FixTree::FixDownCastExpression(Block *pBlock, Expression *pExpression)
{
    pExpression->u.down_cast.operand = FixExpression(pBlock, pExpression->u.down_cast.operand, pExpression);
    FixTypeSpecifier(pExpression->u.down_cast.type);

    TypeSpecifier *pOriginType = pExpression->u.down_cast.type;
    TypeSpecifier *pTargetType = pExpression->u.down_cast.type;

    if (!IsClassObject(pOriginType))
    {
        m_Error.CompileError(pExpression->line_number, DOWN_CAST_OPERAND_IS_NOT_CLASS_ERR, MESSAGE_ARGUMENT_END);
    }

    if (!IsClassObject(pTargetType))
    {
        m_Error.CompileError(pExpression->line_number, DOWN_CAST_TARGET_IS_NOT_CLASS_ERR, MESSAGE_ARGUMENT_END);
    }

    if (m_Util.CompareType(pOriginType, pTargetType))
    {
        m_Error.CompileError(pExpression->line_number, DOWN_CAST_DO_NOTHING_ERR, MESSAGE_ARGUMENT_END);
    }

    if (DVM_CLASS_DEFINITION == pTargetType->class_ref.class_definition->class_or_interface
        && !IsSuperClass(pTargetType->class_ref.class_definition, pOriginType->class_ref.class_definition, nullptr, nullptr))
    {
        m_Error.CompileError(pExpression->line_number, DOWN_CAST_TO_BAD_CLASS_ERR, MESSAGE_ARGUMENT_END);
    }

    pExpression->type = pTargetType;

    return pExpression;
}

Expression* FixTree::FixNewExpression(Block *pBlock, Expression *pExpression)
{
    pExpression->u.new_e.class_definition = SearchAndAddClass(pExpression->line_number, pExpression->u.new_e.class_name, &pExpression->u.new_e.class_index);

    if (DVM_TRUE == pExpression->u.new_e.class_definition->is_abstract)
    {
        m_Error.CompileError(pExpression->line_number, NEW_ABSTRACT_CLASS_ERR,
            STRING_MESSAGE_ARGUMENT, "name", pExpression->u.new_e.class_name,
            MESSAGE_ARGUMENT_END);
    }

    if (!pExpression->u.new_e.method_name)
    {
        pExpression->u.new_e.method_name = DEFAULT_CONSTRUCTOR_NAME;
    }

    MemberDeclaration *pMember = m_Util.SearchMember(pExpression->u.new_e.class_definition, pExpression->u.new_e.method_name);

    if (nullptr == pMember)
    {
        m_Error.CompileError(pExpression->line_number, MEMBER_NOT_FOUND_ERR,
            STRING_MESSAGE_ARGUMENT, "class_name", pExpression->u.new_e.class_name,
            STRING_MESSAGE_ARGUMENT, "member_name", pExpression->u.new_e.method_name,
            MESSAGE_ARGUMENT_END);
    }

    if (pMember->kind != METHOD_MEMBER)
    {
        m_Error.CompileError(pExpression->line_number, CONSTRUCTOR_IS_NOT_METHOD_ERR,
            STRING_MESSAGE_ARGUMENT, "member_name", pExpression->u.new_e.method_name,
            MESSAGE_ARGUMENT_END);
    }

    if (pMember->u.method.is_constructor != DVM_TRUE)
    {
        m_Error.CompileError(pExpression->line_number, NOT_CONSTRUCTOR_ERR,
            STRING_MESSAGE_ARGUMENT, "member_name", pExpression->u.new_e.method_name,
            MESSAGE_ARGUMENT_END);
    }

    CheckMemberAccessibility(pExpression->line_number, pExpression->u.new_e.class_definition, pMember, pExpression->u.new_e.method_name);

    DBG_assert(nullptr == pMember->u.method.function_definition->type->derive
        && DVM_VOID_TYPE == pMember->u.method.function_definition->type->basic_type,
        ("constructor is not void."));

    CheckArgument(pBlock, pExpression->line_number, pMember->u.method.function_definition->parameter, pExpression->u.new_e.argument, nullptr);

    pExpression->u.new_e.method_declaration = pMember;

    TypeSpecifier *pTypeSpecifier = m_Util.AllocTypeSpecifier(DVM_CLASS_TYPE);
    pTypeSpecifier->class_ref.identifier = pExpression->u.new_e.class_definition->name;
    pTypeSpecifier->class_ref.class_definition = pExpression->u.new_e.class_definition;
    pExpression->type = pTypeSpecifier;

    return pExpression;
}

void FixTree::FixParameterList(ParameterList *pParameterList)
{
    for (ParameterList *pos = pParameterList; pos; pos = pos->next)
    {
        FixTypeSpecifier(pos->type);
    }
}

void FixTree::FixTypeSpecifier(TypeSpecifier *pTypeSpecifier)
{
    DKC_Compiler *pCompiler = m_Interface.GetCurrentCompiler();

    for (TypeDerive *pos = pTypeSpecifier->derive; pos; pos = pos->next)
    {
        if (FUNCTION_DERIVE == pos->tag)
        {
            FixParameterList(pos->u.function_d.parameter_list);
        }
    }

    if (DVM_CLASS_TYPE == pTypeSpecifier->basic_type && nullptr == pTypeSpecifier->class_ref.class_definition)
    {
        ClassDefinition *pClassDefinition = m_Util.SearchClass(pTypeSpecifier->class_ref.identifier);

        if (pClassDefinition)
        {
            if (!m_Util.ComparePackageName(pClassDefinition->package_name, pCompiler->package_name)
                && pClassDefinition->access_modifier != DVM_PUBLIC_ACCESS)
            {
                m_Error.CompileError(pTypeSpecifier->line_number,
                    PACKAGE_CLASS_ACCESS_ERR,
                    STRING_MESSAGE_ARGUMENT, "class_name", pClassDefinition->name,
                    MESSAGE_ARGUMENT_END);
            }

            pTypeSpecifier->class_ref.class_definition = pClassDefinition;
            pTypeSpecifier->class_ref.class_index = AddClass(pClassDefinition);
        }

        m_Error.CompileError(pTypeSpecifier->line_number,
            TYPE_NAME_NOT_FOUND_ERR,
            STRING_MESSAGE_ARGUMENT, "name", pTypeSpecifier->class_ref.identifier,
            MESSAGE_ARGUMENT_END);
    }
}

void FixTree::FixIfStatement(Block *pBlock, IfStatement *pIfStatement, FunctionDefinition *pFunctionDefinition)
{
    pIfStatement->condition = FixExpression(pBlock, pIfStatement->condition, nullptr);

    if (!IsBoolean(pIfStatement->condition->type))
    {
        m_Error.CompileError(pIfStatement->condition->line_number, IF_CONDITION_NOT_BOOLEAN_ERR, MESSAGE_ARGUMENT_END);
    }

    FixStatementList(pIfStatement->then_block, pIfStatement->then_block->statement_list, pFunctionDefinition);

    for (Elsif *pos = pIfStatement->elsif_list; pos; pos = pos->next)
    {
        pos->condition = FixExpression(pBlock, pos->condition, nullptr);

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

void FixTree::FixWhileStatement(Block *pBlock, WhileStatement *pWhileStatement, FunctionDefinition *pFunctionDefinition)
{
    pWhileStatement->condition = FixExpression(pBlock, pWhileStatement->condition, nullptr);

    if (!IsBoolean(pWhileStatement->condition->type))
    {
        m_Error.CompileError(pWhileStatement->condition->line_number, WHILE_CONDITION_NOT_BOOLEAN_ERR, MESSAGE_ARGUMENT_END);
    }

    FixStatementList(pWhileStatement->block, pWhileStatement->block->statement_list, pFunctionDefinition);
}

void FixTree::FixDoWhileStatement(Block *pBlock, DoWhileStatement *pDoWhileStatement, FunctionDefinition *pFunctionDefinition)
{
    pDoWhileStatement->condition = FixExpression(pBlock, pDoWhileStatement->condition, nullptr);

    if (!IsBoolean(pDoWhileStatement->condition->type))
    {
        m_Error.CompileError(pDoWhileStatement->condition->line_number, DO_WHILE_CONDITION_NOT_BOOLEAN_ERR, MESSAGE_ARGUMENT_END);
    }

    FixStatementList(pDoWhileStatement->block, pDoWhileStatement->block->statement_list, pFunctionDefinition);
}

void FixTree::FixForStatement(Block *pBlock, ForStatement *pForStatement, FunctionDefinition *pFunctionDefinition)
{
    pForStatement->init = FixExpression(pBlock, pForStatement->init, nullptr);
    pForStatement->condition = FixExpression(pBlock, pForStatement->condition, nullptr);

    if (pForStatement->condition != nullptr && !IsBoolean(pForStatement->condition->type))
    {
        m_Error.CompileError(pForStatement->condition->line_number, FOR_CONDITION_NOT_BOOLEAN_ERR, MESSAGE_ARGUMENT_END);
    }

    pForStatement->post = FixExpression(pBlock, pForStatement->post, nullptr);
    FixStatementList(pForStatement->block, pForStatement->block->statement_list, pFunctionDefinition);
}

void FixTree::FixReturnStatement(Block *pBlock, Statement *pStatement, FunctionDefinition *pFunctionDefinition)
{
    Expression *pReturnValue = FixExpression(pBlock, pStatement->u.return_s.return_value, nullptr);

    if (nullptr == pFunctionDefinition->type->derive && DVM_VOID_TYPE == pFunctionDefinition->type->basic_type && pReturnValue)
    {
        m_Error.CompileError(pStatement->line_number, RETURN_IN_VOID_FUNCTION_ERR, MESSAGE_ARGUMENT_END);
    }

    if (nullptr == pReturnValue)
    {
        if (pFunctionDefinition->type->derive)
        {
            if (ARRAY_DERIVE == pFunctionDefinition->type->derive->tag)
            {
                pReturnValue = m_Create.AllocExpression(NULL_EXPRESSION);
            }
            else
            {
                DBG_assert(0, ("pFunctionDefinition->type->derive..", pFunctionDefinition->type->derive));
            }
        }
        else
        {
            switch (pFunctionDefinition->type->basic_type)
            {
            case DVM_VOID_TYPE :
                pReturnValue = m_Create.AllocExpression(INT_EXPRESSION);
                pReturnValue->u.int_value = 0;
                break;

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
            case DVM_CLASS_TYPE :
                pReturnValue = m_Create.AllocExpression(NULL_EXPRESSION);
                break;

            case DVM_NULL_TYPE :
            case DVM_BASE_TYPE :
            default :
                DBG_assert(0, ("basic_type..", pFunctionDefinition->type->basic_type));
            }
        }

        pStatement->u.return_s.return_value = pReturnValue;
        return;
    }

    Expression *pCastExpression = CreateAssignCast(pStatement->u.return_s.return_value, pFunctionDefinition->type);
    pStatement->u.return_s.return_value = pCastExpression;
}

void FixTree::FixStatementList(Block *pBlock, StatementList *pStatementList, FunctionDefinition *pFunctionDefinition)
{
    for (StatementList *pos = pStatementList; pos; pos = pos->next)
    {
        FixStatement(pBlock, pos->statement, pFunctionDefinition);
    }
}

void FixTree::FixFunction(FunctionDefinition *pFunctionDefinition)
{
    AddParameterAsDeclaration(pFunctionDefinition);
    FixTypeSpecifier(pFunctionDefinition->type);

    if (pFunctionDefinition->block)
    {
        FixStatementList(pFunctionDefinition->block, pFunctionDefinition->block->statement_list, pFunctionDefinition);
        AddReturnFunction(pFunctionDefinition);
    }
}

void FixTree::FixExtends(ClassDefinition *pClassDefinition)
{
    if (DVM_INTERFACE_DEFINITION == pClassDefinition->class_or_interface && pClassDefinition->extends)
    {
        m_Error.CompileError(pClassDefinition->line_number, INTERFACE_INHERIT_ERR, MESSAGE_ARGUMENT_END);
    }

    pClassDefinition->interface_list = nullptr;

    for (ExtendsList *pos = pClassDefinition->extends; pos; pos = pos->next)
    {
        ClassDefinition *pSuper = SearchAndAddClass(pClassDefinition->line_number, pos->identifier, nullptr);

        pos->class_definition = pSuper;

        ExtendsList *pNewExtendsList = (ExtendsList*)dkc_malloc(sizeof(ExtendsList));
        ExtendsList *pLastExtendsList = nullptr;

        *pNewExtendsList = *pos;
        pNewExtendsList->next = nullptr;

        if (DVM_CLASS_DEFINITION == pSuper->class_or_interface)
        {
            if (pClassDefinition->super_class)
            {
                m_Error.CompileError(pClassDefinition->line_number, MULTIPLE_INHERITANCE_ERR,
                    STRING_MESSAGE_ARGUMENT, "name", pSuper->name,
                    MESSAGE_ARGUMENT_END);
            }

            if (DVM_FALSE == pSuper->is_abstract)
            {
                m_Error.CompileError(pClassDefinition->line_number, INHERIT_CONCRETE_CLASS_ERR,
                    STRING_MESSAGE_ARGUMENT, "name", pSuper->name,
                    MESSAGE_ARGUMENT_END);
            }

            pClassDefinition->super_class = pSuper;
        }
        else
        {
            DBG_assert(DVM_INTERFACE_DEFINITION == pSuper->class_or_interface, ("super..", pSuper->class_or_interface));

            if (nullptr == pClassDefinition->interface_list)
            {
                pClassDefinition->interface_list = pNewExtendsList;
            }
            else
            {
                pLastExtendsList->next = pNewExtendsList;
            }

            pLastExtendsList = pNewExtendsList;
        }
    }
}

void FixTree::FixClassList(DKC_Compiler *pCompiler)
{
    for (ClassDefinition *pos = pCompiler->class_definition_list; pos; pos = pos->next)
    {
        AddClass(pos);
        FixExtends(pos);
    }

    for (ClassDefinition *pos = pCompiler->class_definition_list; pos; pos = pos->next)
    {
        AddSuperInterface(pos);
    }

    for (ClassDefinition *pos = pCompiler->class_definition_list; pos; pos = pos->next)
    {
        if (pos->class_or_interface != DVM_CLASS_DEFINITION)
            continue;

        pCompiler->current_class_definition = pos;
        AddDefaultConstructor(pos);
        pCompiler->current_class_definition = nullptr;
    }

    for (ClassDefinition *pos = pCompiler->class_definition_list; pos; pos = pos->next)
    {
        int iFieldIndex;
        int iMethodIndex;
        char *lpstrAbstractMethodName = nullptr;

        pCompiler->current_class_definition = pos;

        GetSuperFieldMethodCount(pos, &iFieldIndex, &iMethodIndex);

        for (MemberDeclaration *pMemberPos = pos->member; pMemberPos; pMemberPos = pMemberPos->next)
        {
            if (METHOD_MEMBER == pMemberPos->kind)
            {
                FixFunction(pMemberPos->u.method.function_definition);

                MemberDeclaration *pSuperMember = SearchMemberInSuper(pos, pMemberPos->u.method.function_definition->name);

                if (pSuperMember)
                {
                    if (pSuperMember->kind != METHOD_MEMBER)
                    {
                        m_Error.CompileError(pMemberPos->line_number, FIELD_OVERRIDED_ERR,
                            STRING_MESSAGE_ARGUMENT, "name", pSuperMember->u.field.name,
                            MESSAGE_ARGUMENT_END);
                    }

                    if (DVM_FALSE == pSuperMember->u.method.is_virtual)
                    {
                        m_Error.CompileError(pMemberPos->line_number, NON_VIRTUAL_METHOD_OVERRIDED_ERR,
                            STRING_MESSAGE_ARGUMENT, "name", pMemberPos->u.method.function_definition->name,
                            MESSAGE_ARGUMENT_END);
                    }

                    if (DVM_FALSE == pMemberPos->u.method.is_override)
                    {
                        m_Error.CompileError(pMemberPos->line_number, NEED_OVERRIDE_ERR,
                            STRING_MESSAGE_ARGUMENT, "name", pMemberPos->u.method.function_definition->name,
                            MESSAGE_ARGUMENT_END);
                    }

                    CheckMethodOverride(pSuperMember, pMemberPos);

                    pMemberPos->u.method.method_index = pSuperMember->u.method.method_index;
                }
                else
                {
                    pMemberPos->u.method.method_index = iMethodIndex;
                    iMethodIndex++;
                }

                if (DVM_TRUE == pMemberPos->u.method.is_abstract)
                {
                    lpstrAbstractMethodName = pMemberPos->u.method.function_definition->name;
                }
            }
            else
            {
                DBG_assert(FIELD_MEMBER == pMemberPos->kind, ("pMemberPos->kind..", pMemberPos->kind));

                FixTypeSpecifier(pMemberPos->u.field.type);

                MemberDeclaration *pSuperMember = SearchMemberInSuper(pos, pMemberPos->u.field.name);

                if (pSuperMember)
                {
                    m_Error.CompileError(pMemberPos->line_number, FIELD_NAME_DUPLICATE_ERR,
                        STRING_MESSAGE_ARGUMENT, "name", pMemberPos->u.field.name,
                        MESSAGE_ARGUMENT_END);
                }
                else
                {
                    pMemberPos->u.field.field_index = iFieldIndex;
                    iFieldIndex++;
                }
            }
        }

        if (lpstrAbstractMethodName && DVM_FALSE == pos->is_abstract)
        {
            m_Error.CompileError(pos->line_number, ABSTRACT_METHOD_IN_CONCRETE_CLASS_ERR,
                STRING_MESSAGE_ARGUMENT, "method_name", lpstrAbstractMethodName,
                MESSAGE_ARGUMENT_END);
        }

        pCompiler->current_class_definition = nullptr;
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
        if (0 == right)
        {
            m_Error.CompileError(pExpression->line_number, DIVISION_BY_ZERO_ERR, MESSAGE_ARGUMENT_END);
        }

        pExpression->u.int_value = left / right;
        break;

    case MOD_EXPRESSION :
        pExpression->u.int_value = left % right;
        break;

    default :
        DBG_assert(0, ("pExpression->kind..", pExpression->kind));
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
        DBG_assert(0, ("pExpression->kind..", pExpression->kind));
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
        DBG_assert(0, ("pExpression->kind..", pExpression));
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
        DBG_assert(0, ("pExpression->kind..", pExpression));
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
        DBG_assert(0, ("pExpression->kind..", pExpression));
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
    else if (NULL_EXPRESSION == pExpression->u.binary_expression.left->kind
        && NULL_EXPRESSION == pExpression->u.binary_expression.right->kind)
    {
        pExpression->kind = BOOLEAN_EXPRESSION;
        pExpression->type = m_Util.AllocTypeSpecifier(DVM_BOOLEAN_TYPE);
        pExpression->u.boolean_value = DVM_TRUE;
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

    default :
        DBG_panic(("cast_type..", enType));
    }

    return pCastExpression;
}

Expression* FixTree::CreateUpCast(Expression *pSrc, ClassDefinition *pDestInterface, int iInterfaceIndex)
{
    TypeSpecifier *pTypeSpecifier = m_Util.AllocTypeSpecifier(DVM_CLASS_TYPE);

    pTypeSpecifier->class_ref.identifier = pDestInterface->name;
    pTypeSpecifier->class_ref.class_definition = pDestInterface;
    pTypeSpecifier->class_ref.class_index = iInterfaceIndex;

    Expression *pExpression = m_Create.AllocExpression(UP_CAST_EXPRESSION);
    
    pExpression->type = pTypeSpecifier;
    pExpression->u.up_cast.interface_definition = pDestInterface;
    pExpression->u.up_cast.operand = pSrc;
    pExpression->u.up_cast.interface_index = iInterfaceIndex;

    return pExpression;
}

Expression* FixTree::CreateAssignCast(Expression *pSrc, TypeSpecifier *pDest)
{
    if (m_Util.CompareType(pSrc->type, pDest))
    {
        return pSrc;
    }

    if (IsObject(pDest) && DVM_NULL_TYPE == pSrc->type->basic_type)
    {
        DBG_assert(nullptr == pSrc->type->derive, ("pSrc derive != nullptr"));
        return pSrc;
    }

    if (IsClassObject(pSrc->type) && IsClassObject(pDest))
    {
        bool bIsInterface;
        int iInterfaceIndex;

        if (IsSuperClass(pSrc->type->class_ref.class_definition, pDest->class_ref.class_definition, &bIsInterface, &iInterfaceIndex))
        {
            if (bIsInterface)
            {
                return CreateUpCast(pSrc, pDest->class_ref.class_definition, iInterfaceIndex);
            }

            return pSrc;
        }
        else
        {
            CastMismatchError(pSrc->line_number, pSrc->type, pDest);
        }
    }

    if (IsInt(pSrc->type) && IsDouble(pDest))
    {
        return AllocCastExpression(INT_TO_DOUBLE_CAST, pSrc);
    }
    else if (IsDouble(pSrc->type) && IsInt(pDest))
    {
        return AllocCastExpression(DOUBLE_TO_INT_CAST, pSrc);
    }
    else if (IsString(pDest))
    {
        Expression *pCastExpression = CreateToStringCast(pSrc);

        if (pCastExpression)
        {
            return pCastExpression;
        }
    }

    CastMismatchError(pSrc->line_number, pSrc->type, pDest);

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

    int iLen = std::basic_string<DVM_Char>(pLeftStr).length() + std::basic_string<DVM_Char>(pRightStr).length();

    DVM_Char *pNewStr = (DVM_Char*)MEM_malloc(sizeof(DVM_Char) * (iLen + 1));
    wcscpy_s(pNewStr, iLen + 1, (std::basic_string<DVM_Char>(pLeftStr) + std::basic_string<DVM_Char>(pRightStr)).c_str());

    MEM_free(pLeftStr);
    MEM_free(pRightStr);

    pExpression->kind = STRING_EXPRESSION;
    pExpression->type = m_Util.AllocTypeSpecifier(DVM_STRING_TYPE);
    pExpression->u.string_value = pNewStr;

    return pExpression;
}

Expression* FixTree::CastBinaryExpression(Expression *pExpression)
{
    Expression *pCastExpression = nullptr;

    if (IsInt(pExpression->u.binary_expression.left->type) && IsDouble(pExpression->u.binary_expression.right->type))
    {
        pCastExpression = AllocCastExpression(INT_TO_DOUBLE_CAST, pExpression->u.binary_expression.left);
        pExpression->u.binary_expression.left = pCastExpression;
    }
    else if (IsDouble(pExpression->u.binary_expression.left->type) && IsInt(pExpression->u.binary_expression.right->type))
    {
        pCastExpression = AllocCastExpression(INT_TO_DOUBLE_CAST, pExpression->u.binary_expression.right);
        pExpression->u.binary_expression.right = pCastExpression;
    }

    if (pCastExpression)
    {
        return pExpression;
    }

    if (IsString(pExpression->u.binary_expression.left->type) && ADD_EXPRESSION == pExpression->kind)
    {
        pCastExpression = CreateToStringCast(pExpression->u.binary_expression.right);

        if (pCastExpression)
        {
            pExpression->u.binary_expression.right = pCastExpression;
        }
    }

    return pExpression;
}

Expression* FixTree::CreateToStringCast(Expression *pExpression)
{
    if (IsBoolean(pExpression->type))
    {
        return AllocCastExpression(BOOLEAN_TO_STRING_CAST, pExpression);
    }
    else if (IsInt(pExpression->type))
    {
        return AllocCastExpression(INT_TO_STRING_CAST, pExpression);
    }
    else if (IsDouble(pExpression->type))
    {
        return AllocCastExpression(DOUBLE_TO_STRING_CAST, pExpression);
    }
    else
    {
        return nullptr;
    }
}

void FixTree::CheckArgument(Block *pBlock, int iLine, ParameterList *pParamList, ArgumentList *pArg, TypeSpecifier *pArrayBase)
{
    ParameterList *pParam = pParamList;

    for (; pParam && pArg; pParam = pParam->next, pArg = pArg->next)
    {
        pArg->expression = FixExpression(pBlock, pArg->expression, nullptr);

        TypeSpecifier *pTypeSpecifier;

        if (DVM_BASE_TYPE == pParam->type->basic_type)
        {
            DBG_assert(pArrayBase, ("pArrayBase == nullptr"));
            pTypeSpecifier = pArrayBase;
        }
        else
        {
            pTypeSpecifier = pParam->type;
        }

        pArg->expression = CreateAssignCast(pArg->expression, pTypeSpecifier);
    }

    if (pParam || pArg)
    {
        m_Error.CompileError(iLine, ARGUMENT_COUNT_MISMATCH_ERR, MESSAGE_ARGUMENT_END);
    }
}

void FixTree::AddLocalVariable(FunctionDefinition *pFunctionDefinition, Declaration *pDeclaration, bool bIsParameter)
{
    pFunctionDefinition->local_variable = (Declaration**)MEM_realloc(pFunctionDefinition->local_variable,
        sizeof(Declaration*) * (pFunctionDefinition->local_variable_count + 1));

    pFunctionDefinition->local_variable[pFunctionDefinition->local_variable_count] = pDeclaration;

    if (pFunctionDefinition->class_definition && !bIsParameter)
    {
        pDeclaration->variable_index = pFunctionDefinition->local_variable_count + 1;
    }
    else
    {
        pDeclaration->variable_index = pFunctionDefinition->local_variable_count;
    }

    pFunctionDefinition->local_variable_count++;
}

void FixTree::AddDeclaration(Block *pBlock, Declaration *pDeclaration, FunctionDefinition *pFunctionDefinition, int iLine, bool bIsParameter)
{
    if (pFunctionDefinition)
    {
        if (m_Util.FunctionSearchDeclaration(pDeclaration->name, pBlock))
        {
            m_Error.CompileError(iLine,
                VARIABLE_MULTIPLE_DEFINE_ERR,
                STRING_MESSAGE_ARGUMENT, "name", pDeclaration->name,
                MESSAGE_ARGUMENT_END);
        }
    }
    else
    {
        if (m_Util.SearchDeclaration(pDeclaration->name, pBlock))
        {
            m_Error.CompileError(iLine,
                VARIABLE_MULTIPLE_DEFINE_ERR,
                STRING_MESSAGE_ARGUMENT, "name", pDeclaration->name,
                MESSAGE_ARGUMENT_END);
        }
    }

    if (pBlock)
    {
        pBlock->declaration_list = m_Create.ChainDeclaration(pBlock->declaration_list, pDeclaration);

        if (nullptr == pFunctionDefinition)
        {
            m_Error.CompileError(iLine, LOCAL_VARIABLE_IN_TOP_ERR,
                STRING_MESSAGE_ARGUMENT, "name", pDeclaration->name,
                MESSAGE_ARGUMENT_END);
        }
    }

    if (pFunctionDefinition)
    {
        pDeclaration->is_local = DVM_TRUE;
        AddLocalVariable(pFunctionDefinition, pDeclaration, bIsParameter);
    }
    else
    {
        DKC_Compiler *pCompiler = m_Interface.GetCurrentCompiler();
        pDeclaration->is_local = DVM_FALSE;
        pCompiler->declaration_list = m_Create.ChainDeclaration(pCompiler->declaration_list, pDeclaration);
    }
}

void FixTree::AddParameterAsDeclaration(FunctionDefinition *pFunctionDefinition)
{
    for (ParameterList *pParam = pFunctionDefinition->parameter; pParam; pParam = pParam->next)
    {
        if (m_Util.FunctionSearchDeclaration(pParam->name, pFunctionDefinition->block))
        {
            m_Error.CompileError(pParam->line_number,
                PARAMETER_MULTIPLE_DEFINE_ERR,
                STRING_MESSAGE_ARGUMENT, "name", pParam->name,
                MESSAGE_ARGUMENT_END);
        }

        FixTypeSpecifier(pParam->type);

        Declaration *pDeclaration = m_Create.AllocDeclaration(pParam->type, pParam->name);

        if (nullptr == pFunctionDefinition || pFunctionDefinition->block)
        {
            AddDeclaration(pFunctionDefinition->block, pDeclaration, pFunctionDefinition, pParam->line_number, true);
        }
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
    pRetStatement->line_number = pFunctionDefinition->end_line_number;

    if (pRetStatement->u.return_s.return_value)
    {
        pRetStatement->u.return_s.return_value->line_number = pFunctionDefinition->end_line_number;
    }

    FixReturnStatement(pFunctionDefinition->block, pRetStatement, pFunctionDefinition);
    *ppLastStatementList = m_Create.CreateStatementList(pRetStatement);
}

void FixTree::CastMismatchError(int iLine, TypeSpecifier *pSrc, TypeSpecifier *pDest)
{
    char *pSrcName = m_Util.GetTypeName(pSrc);
    char *pDestName = m_Util.GetTypeName(pDest);

    m_Error.CompileError(iLine,
        CAST_MISMATCH_ERR,
        STRING_MESSAGE_ARGUMENT, "src", pSrcName,
        STRING_MESSAGE_ARGUMENT, "dest", pDestName,
        MESSAGE_ARGUMENT_END);
}

int FixTree::ReservFunctionIndex(DKC_Compiler *pCompiler, FunctionDefinition *pSrc)
{
    if (pSrc->class_definition && nullptr == pSrc->block)
    {
        return ABSTRACT_METHOD_INDEX;
    }

    char *srcPackageName = m_Util.PackageNameToString(pSrc->package_name);

    for (int i = 0; i < pCompiler->dvm_function_count; i++)
    {
        if (m_Util.ComparePackageName(srcPackageName, pCompiler->dvm_function[i].package_name)
            && std::string(pSrc->name) == pCompiler->dvm_function[i].name)
        {
            MEM_free(srcPackageName);
            return i;
        }
    }

    pCompiler->dvm_function = (DVM_Function*)MEM_realloc(pCompiler->dvm_function, sizeof(DVM_Function) * (pCompiler->dvm_function_count + 1));
    DVM_Function *dest = &pCompiler->dvm_function[pCompiler->dvm_function_count++];
    dest->package_name = srcPackageName;

    if (pSrc->class_definition)
    {
        dest->name = m_Util.CreateMethodFunctionName(pSrc->class_definition->name, pSrc->name);
    }
    else
    {
        dest->name = MEM_strdup(pSrc->name);
    }

    return pCompiler->dvm_function_count - 1;
}

int FixTree::AddClass(ClassDefinition *pSrc)
{
    DKC_Compiler *pCompiler = m_Interface.GetCurrentCompiler();
    char *lpstrPackageName = m_Util.PackageNameToString(pSrc->package_name);

    for (int i = 0; i < pCompiler->dvm_class_count; i++)
    {
        if (m_Util.ComparePackageName(lpstrPackageName, pCompiler->dvm_class[i].package_name)
            && std::string(pSrc->name) == pCompiler->dvm_class[i].name)
        {
            MEM_free(lpstrPackageName);
            return i;
        }
    }

    pCompiler->dvm_class = (DVM_Class*)MEM_realloc(pCompiler->dvm_class, sizeof(DVM_Class) * (pCompiler->dvm_class_count + 1));

    DVM_Class *pDest = &pCompiler->dvm_class[pCompiler->dvm_class_count++];

    pDest->package_name = lpstrPackageName;
    pDest->name = MEM_strdup(pSrc->name);
    pDest->is_implemented = DVM_FALSE;

    for (ExtendsList *pos = pSrc->extends; pos; pos = pos->next)
    {
        int dummy;
        (void)SearchAndAddClass(pSrc->line_number, pos->identifier, &dummy);
    }

    return pCompiler->dvm_class_count - 1;
}

bool FixTree::IsSuperInterface(ClassDefinition *pChild, ClassDefinition *pParent, int *pInterfaceIndexOut)
{
    int iInterfaceIndex = 0;

    for (ExtendsList *pos = pChild->interface_list; pos; pos = pos->next)
    {
        if (pos->class_definition == pParent)
        {
            if (pInterfaceIndexOut)
            {
                *pInterfaceIndexOut = iInterfaceIndex;
            }

            return true;
        }

        iInterfaceIndex++;
    }

    return false;
}

bool FixTree::IsSuperClass(ClassDefinition *pChild, ClassDefinition *pParent, bool *pIsInterface, int *pInterfaceIndex)
{
    for (ClassDefinition *pos = pChild->super_class; pos; pos = pos->super_class)
    {
        if (pos == pParent)
        {
            if (pIsInterface)
            {
                *pIsInterface = false;
            }

            return true;
        }
    }

    if (pIsInterface)
    {
        *pIsInterface = true;
    }

    return IsSuperInterface(pChild, pParent, pInterfaceIndex);
}

bool FixTree::IsInterfaceMethod(ClassDefinition *pClassDefinition, MemberDeclaration *pMemberDeclaration, ClassDefinition **ppTargetInterface, int *pInterfaceIndexOut)
{
    ExtendsList *pExtendPos = pClassDefinition->interface_list;
    int iInterfaceIndex = 0;

    for (; pExtendPos; pExtendPos = pExtendPos->next, iInterfaceIndex++)
    {
        for (MemberDeclaration *pMemberPos = pExtendPos->class_definition->member; pMemberPos; pMemberPos = pMemberPos->next)
        {
            if (pMemberPos->kind != METHOD_MEMBER)
                continue;

            if (std::string(pMemberDeclaration->u.method.function_definition->name) == pMemberPos->u.method.function_definition->name)
            {
                if (ppTargetInterface)
                {
                    *ppTargetInterface = pExtendPos->class_definition;
                }

                if (pInterfaceIndexOut)
                {
                    *pInterfaceIndexOut = iInterfaceIndex;
                }

                return true;
            }
        }
    }

    return false;
}

ClassDefinition* FixTree::SearchAndAddClass(int iLine, char *lpstrName, int *pClassIndex)
{
    ClassDefinition *pClassDefinition = m_Util.SearchClass(lpstrName);

    if (nullptr == pClassDefinition)
    {
        m_Error.CompileError(iLine, CLASS_NOT_FOUND_ERR,
            STRING_MESSAGE_ARGUMENT, "name", lpstrName,
            MESSAGE_ARGUMENT_END);
    }

    *pClassIndex = AddClass(pClassDefinition);

    return pClassDefinition;
}

TypeSpecifier* FixTree::CreateFunctionDeriveType(FunctionDefinition *pFunctionDefinition)
{
    TypeSpecifier *pTypeSpecifier = m_Util.AllocTypeSpecifier(pFunctionDefinition->type->basic_type);

    *pTypeSpecifier = *pFunctionDefinition->type;
    pTypeSpecifier->derive = m_Util.AllocTypeDerive(FUNCTION_DERIVE);
    pTypeSpecifier->derive->u.function_d.parameter_list = pFunctionDefinition->parameter;
    pTypeSpecifier->derive->next = pFunctionDefinition->type->derive;

    return pTypeSpecifier;
}

bool FixTree::CheckTypeCompatibility(TypeSpecifier *pSuper, TypeSpecifier *pSub)
{
    if (!IsClassObject(pSuper))
    {
        return m_Util.CompareType(pSuper, pSub);
    }

    if (!IsClassObject(pSub))
    {
        return false;
    }

    if (pSuper->class_ref.class_definition == pSub->class_ref.class_definition
        || IsSuperClass(pSub->class_ref.class_definition, pSuper->class_ref.class_definition, nullptr, nullptr))
    {
        return true;
    }

    return false;
}

void FixTree::CheckFunctionCompatibility(int iLine, char *lpstrName, TypeSpecifier *pType1, ParameterList *pParam1, TypeSpecifier *pType2, ParameterList *pParam2)
{
    ParameterList *pos1 = pParam1;
    ParameterList *pos2 = pParam2;
    int iIndex = 1;

    for (; pos1 && pos2; pos1 = pos1->next, pos2 = pos2->next)
    {
        if (!CheckTypeCompatibility(pos2->type, pos1->type))
        {
            m_Error.CompileError(iLine, BAD_PARAMETER_TYPE_ERR,
                STRING_MESSAGE_ARGUMENT, "func_name", lpstrName, INT_MESSAGE_ARGUMENT, "index", iIndex, STRING_MESSAGE_ARGUMENT, "param_name", pos2->name,
                MESSAGE_ARGUMENT_END);
        }

        iIndex++;
    }

    if (pos1 || pos2)
    {
        m_Error.CompileError(iLine, BAD_PARAMETER_COUNT_ERR,
            STRING_MESSAGE_ARGUMENT, "name", lpstrName,
            MESSAGE_ARGUMENT_END);
    }

    if (!CheckTypeCompatibility(pType1, pType2))
    {
        m_Error.CompileError(iLine, BAD_RETURN_TYPE_ERR,
            STRING_MESSAGE_ARGUMENT, "name", lpstrName,
            MESSAGE_ARGUMENT_END);
    }
}

void FixTree::CheckFunctionCompatibility(FunctionDefinition *pFunctionDefine1, FunctionDefinition *pFunctionDefine2)
{
    CheckFunctionCompatibility(pFunctionDefine2->end_line_number, pFunctionDefine2->name,
        pFunctionDefine1->type, pFunctionDefine1->parameter,
        pFunctionDefine2->type, pFunctionDefine2->parameter);
}

void FixTree::CheckMemberAccessibility(int iLine, ClassDefinition *pTargetClass, MemberDeclaration *pMember, char *lpstrMemberName)
{
    DKC_Compiler *pCompiler = m_Interface.GetCurrentCompiler();

    if (nullptr == pCompiler->current_class_definition || pCompiler->current_class_definition != pTargetClass)
    {
        if (DVM_PRIVATE_ACCESS == pMember->access_modifier)
        {
            m_Error.CompileError(iLine, PRIVATE_MEMBER_ACCESS_ERR,
                STRING_MESSAGE_ARGUMENT, "member_name", lpstrMemberName,
                MESSAGE_ARGUMENT_END);
        }
    }

    if (!m_Util.ComparePackageName(pCompiler->package_name, pTargetClass->package_name) && pMember->access_modifier != DVM_PUBLIC_ACCESS)
    {
        m_Error.CompileError(iLine, PACKAGE_MEMBER_ACCESS_ERR,
            STRING_MESSAGE_ARGUMENT, "member_name", lpstrMemberName,
            MESSAGE_ARGUMENT_END);
    }
}

void FixTree::AddSuperInterface(ClassDefinition *pClassDefinition)
{
    ExtendsList *pTail = nullptr;

    if (pClassDefinition->interface_list)
    {
        for (pTail = pClassDefinition->interface_list; pTail->next; pTail = pTail->next)
            ;
    }

    for (ClassDefinition *pos = pClassDefinition->super_class; pos; pos = pos->super_class)
    {
        for (ExtendsList *pIfPos = pos->interface_list; pIfPos; pIfPos = pIfPos->next)
        {
            ExtendsList *pNewExtends = (ExtendsList*)dkc_malloc(sizeof(ExtendsList));

            *pNewExtends = *pIfPos;
            pNewExtends->next = nullptr;

            if (pTail)
            {
                pTail->next = pNewExtends;
            }
            else
            {
                pClassDefinition->interface_list = pNewExtends;
            }

            pTail = pNewExtends;
        }
    }
}

void FixTree::AddDefaultConstructor(ClassDefinition *pClassDefinition)
{
    MemberDeclaration *pTail = nullptr;

    for (MemberDeclaration *pos = pClassDefinition->member; pos; pos = pos->next)
    {
        if (METHOD_MEMBER == pos->kind && DVM_TRUE == pos->u.method.is_constructor)
        {
            return;
        }

        pTail = pos;
    }

    TypeSpecifier *pTypeSpecifier = m_Util.AllocTypeSpecifier(DVM_VOID_TYPE);
    Block *pBlock = m_Create.AllocBlock();
    ClassOrMemberModifierList modifier = m_Create.CreateClassOrMemberModifier(VIRTUAL_MODIFIER);

    if (pClassDefinition->super_class)
    {
        Statement *pStatement = m_Create.AllocStatement(EXPRESSION_STATEMENT);
        Expression *pSuperExpression = m_Create.CreateSuperExpression();
        Expression *pMemberExpression = m_Create.CreateMemberExpression(pSuperExpression, DEFAULT_CONSTRUCTOR_NAME);
        Expression *pFunctionCallExpression = m_Create.CreateFunctionCallExpression(pMemberExpression, nullptr);

        pStatement->u.expression_s = pFunctionCallExpression;
        pBlock->statement_list = m_Create.CreateStatementList(pStatement);

        modifier = m_Create.ChainClassOrMemberModifier(modifier, m_Create.CreateClassOrMemberModifier(OVERRIDE_MODIFIER));
        modifier = m_Create.ChainClassOrMemberModifier(modifier, m_Create.CreateClassOrMemberModifier(PUBLIC_MODIFIER));
    }
    else
    {
        pBlock->statement_list = nullptr;
    }

    FunctionDefinition *pFunctionDefinition = m_Create.CreateFunctionDefinition(pTypeSpecifier, DEFAULT_CONSTRUCTOR_NAME, nullptr, pBlock);

    if (pTail)
    {
        pTail->next = m_Create.CreateMethodMember(&modifier, pFunctionDefinition, DVM_TRUE);
    }
    else
    {
        pClassDefinition->member = m_Create.CreateMethodMember(&modifier, pFunctionDefinition, DVM_TRUE);
    }
}

void FixTree::GetSuperFieldMethodCount(ClassDefinition *pClassDefinition, int *pFieldIndexOut, int *pMethodIndexOut)
{
    int iFieldIndex = -1;
    int iMethodIndex = -1;

    for (ClassDefinition *pCdPos = pClassDefinition->super_class; pCdPos; pCdPos = pCdPos->super_class)
    {
        for (MemberDeclaration *pMPos = pCdPos->member; pMPos; pMPos = pMPos->next)
        {
            if (METHOD_MEMBER == pMPos->kind)
            {
                if (pMPos->u.method.method_index > iMethodIndex)
                {
                    iMethodIndex = pMPos->u.method.method_index;
                }
            }
            else
            {
                DBG_assert(FIELD_MEMBER == pMPos->kind, ("pMPos->kind..", pMPos->kind));

                if (pMPos->u.field.field_index > iFieldIndex)
                {
                    iFieldIndex = pMPos->u.field.field_index;
                }
            }
        }
    }

    *pFieldIndexOut = iFieldIndex + 1;
    *pMethodIndexOut = iMethodIndex + 1;
}

MemberDeclaration* FixTree::SearchMemberInSuper(ClassDefinition *pClassDefinition, char *lpstrMemberName)
{
    MemberDeclaration *pMemberDeclaration = nullptr;

    if (pClassDefinition->super_class)
    {
        pMemberDeclaration = m_Util.SearchMember(pClassDefinition->super_class, lpstrMemberName);
    }

    if (pMemberDeclaration)
    {
        return pMemberDeclaration;
    }

    for (ExtendsList *pos = pClassDefinition->interface_list; pos; pos = pos->next)
    {
        pMemberDeclaration = m_Util.SearchMember(pos->class_definition, lpstrMemberName);

        if (pMemberDeclaration)
        {
            return pMemberDeclaration;
        }
    }

    return nullptr;
}

void FixTree::CheckMethodOverride(MemberDeclaration *pSuperMethod, MemberDeclaration *pSubMethod)
{
    if (DVM_PUBLIC_ACCESS == pSuperMethod->access_modifier && pSubMethod->access_modifier != DVM_PUBLIC_ACCESS
        || DVM_FILE_ACCESS == pSuperMethod->access_modifier && DVM_PRIVATE_ACCESS == pSubMethod->access_modifier)
    {
        m_Error.CompileError(pSubMethod->line_number, OVERRIDE_METHOD_ACCESSIBILITY_ERR,
            STRING_MESSAGE_ARGUMENT, "name", pSubMethod->u.method.function_definition->name,
            MESSAGE_ARGUMENT_END);
    }

    if (DVM_FALSE == pSubMethod->u.method.is_constructor)
    {
        CheckFunctionCompatibility(pSuperMethod->u.method.function_definition, pSubMethod->u.method.function_definition);
    }
}