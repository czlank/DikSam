#include "StdAfx.h"
#include "Create.h"
#include "DikSam.h"
#include "Debug.h"
#include "Memory.h"
#include "Error.h"
#include "Util.h"
#include "Interface.h"

#include "CreateC.cpp"

Create::Create(Debug& debug, Memory& memory, Error& error, Util& util, Interface& rInterface)
    : m_Debug(debug)
    , m_Memory(memory)
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
    Declaration *pDecl = (Declaration*)dkc_malloc(sizeof(Declaration));

    pDecl->name = lpstrIdentifier;
    pDecl->type = pType;
    pDecl->variable_index = -1;

    return pDecl;
}

DeclarationList* Create::ChainDeclaration(DeclarationList *pList, Declaration *pDecl)
{
    DeclarationList *pNewItem = (DeclarationList*)dkc_malloc(sizeof(DeclarationList));
    
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

    (void)CreateFunctionDefinition(pType, lpstrIdentifier, pParameterList, pBlock);
}

FunctionDefinition* Create::CreateFunctionDefinition(TypeSpecifier *pType, char *lpstrIdentifier, ParameterList *pParameterList, Block *pBlock)
{
    DKC_Compiler *pCompiler = m_Interface.GetCompiler();
    FunctionDefinition *pFD = (FunctionDefinition*)dkc_malloc(sizeof(FunctionDefinition));

    pFD->type = pType;
    pFD->package_name = pCompiler->package_name;
    pFD->name = lpstrIdentifier;
    pFD->parameter = pParameterList;
    pFD->block = pBlock;
    pFD->local_variable_count = 0;
    pFD->local_variable = nullptr;
    pFD->class_definition = nullptr;
    pFD->end_line_number = pCompiler->current_line_number;
    pFD->next = nullptr;

    if (pBlock)
    {
        pBlock->type = FUNCTION_BLOCK;
        pBlock->parent.function.function = pFD;
    }

    AddFunctionToCompiler(pFD);

    return pFD;
}

ParameterList* Create::CreateParameter(TypeSpecifier *pType, char *lpstrIdentifier)
{
    ParameterList *p = (ParameterList*)dkc_malloc(sizeof(ParameterList));

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
    ArgumentList *pArgumentList = (ArgumentList*)dkc_malloc(sizeof(ArgumentList));

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
    ExpressionList *pExpressionList = (ExpressionList*)dkc_malloc(sizeof(ExpressionList));

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
    StatementList *pStatementList = (StatementList*)dkc_malloc(sizeof(StatementList));

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
    TypeSpecifier *pTypeSpecifier = m_Util.AllocTypeSpecifier(enType);
    pTypeSpecifier->line_number = m_Interface.GetCompiler()->current_line_number;

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

TypeSpecifier* Create::CreateClassTypeSpecifier(char *lpstrIdentifier)
{
    TypeSpecifier *pTypeSpecifier = m_Util.AllocTypeSpecifier(DVM_CLASS_TYPE);

    pTypeSpecifier->class_ref.identifier = lpstrIdentifier;
    pTypeSpecifier->class_ref.class_definition = nullptr;
    pTypeSpecifier->line_number = m_Interface.GetCompiler()->current_line_number;

    return pTypeSpecifier;
}

Expression* Create::AllocExpression(ExpressionKind enKind)
{
    Expression *pExpression = (Expression*)dkc_malloc(sizeof(Expression));

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

    Statement *pStatement = (Statement*)dkc_malloc(sizeof(Statement));
    pStatement->type = DECLARATION_STATEMENT;
    pStatement->line_number = pLeft->line_number;
    
    Declaration *pDeclaration = (Declaration*)dkc_malloc(sizeof(Declaration));
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

Expression* Create::CreateMemberExpression(Expression *pMemberExpression, char *lpstrMemberName)
{
    Expression *pExpression = AllocExpression(MEMBER_EXPRESSION);

    pExpression->u.member_expression.expression = pMemberExpression;
    pExpression->u.member_expression.member_name = lpstrMemberName;

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
    Expression *pExpression = AllocExpression(NULL_EXPRESSION);

    return pExpression;
}

Expression* Create::CreateIndexExpression(Expression *pArrayExpression, Expression* pIndex)
{
    Expression *pExpression = AllocExpression(INDEX_EXPRESSION);

    pExpression->u.index_expression.array = pArrayExpression;
    pExpression->u.index_expression.index = pIndex;

    return pExpression;
}

Expression* Create::CreateArrayLiteralExpression(ExpressionList *pList)
{
    Expression *pExpression = AllocExpression(ARRAY_LITERAL_EXPRESSION);

    pExpression->u.array_literal = pList;

    return pExpression;
}

Expression* Create::CreateBasicArrayCreation(DVM_BasicType enType, ArrayDimension *pArrayDimensionExpressionList, ArrayDimension *pArrayDimension)
{
    TypeSpecifier *pTypeSpecifier = CreateTypeSpecifier(enType);
    Expression *pExpression = CreateClassArrayCreation(pTypeSpecifier, pArrayDimensionExpressionList, pArrayDimension);

    return pExpression;
}

Expression* Create::CreateClassArrayCreation(TypeSpecifier *pType, ArrayDimension *pArrayDimensionExpressionList, ArrayDimension *pArrayDimension)
{
    Expression *pExpression = AllocExpression(ARRAY_CREATION_EXPRESSION);

    pExpression->u.array_creation.type = pType;
    pExpression->u.array_creation.dimension = ChainArrayDimension(pArrayDimensionExpressionList, pArrayDimension);

    return pExpression;
}

Expression* Create::CreateInstanceofExpression(Expression *pOperand, TypeSpecifier *pType)
{
    Expression *pExpression = AllocExpression(INSTANCEOF_EXPRESSION);

    pExpression->u.instanceof.operand = pOperand;
    pExpression->u.instanceof.type = pType;

    return pExpression;
}

Expression* Create::CreateDownCastExpression(Expression *pOperand, TypeSpecifier *pType)
{
    Expression *pExpression = AllocExpression(DOWN_CAST_EXPRESSION);

    pExpression->u.down_cast.operand = pOperand;
    pExpression->u.down_cast.type = pType;

    return pExpression;
}

Expression* Create::CreateNewExpression(char *lpstrClassName, char *lpstrMethodName, ArgumentList *pArgument)
{
    Expression *pExpression = AllocExpression(NEW_EXPRESSION);

    pExpression->u.new_e.class_name = lpstrClassName;
    pExpression->u.new_e.class_definition = nullptr;
    pExpression->u.new_e.method_name = lpstrMethodName;
    pExpression->u.new_e.method_declaration = nullptr;
    pExpression->u.new_e.argument = pArgument;

    return pExpression;
}

Expression* Create::CreateThisExpression()
{
    Expression *pExpression = AllocExpression(THIS_EXPRESSION);
    return pExpression;
}

Expression* Create::CreateSuperExpression()
{
    Expression *pExpression = AllocExpression(SUPER_EXPRESSION);
    return pExpression;
}

ArrayDimension* Create::CreateArrayDimension(Expression *pExpression)
{
    ArrayDimension *pArrayDimension = (ArrayDimension*)dkc_malloc(sizeof(ArrayDimension));

    pArrayDimension->expression = pExpression;
    pArrayDimension->next = nullptr;

    return pArrayDimension;
}

ArrayDimension* Create::ChainArrayDimension(ArrayDimension *pList, ArrayDimension *pArrayDimension)
{
    ArrayDimension *pos;

    for (pos = pList; pos->next != nullptr; pos = pos->next)
        ;
    pos->next = pArrayDimension;

    return pList;
}

Statement* Create::AllocStatement(StatementType enType)
{
    Statement *pStatement = (Statement*)dkc_malloc(sizeof(Statement));

    pStatement->type = enType;
    pStatement->line_number = m_Interface.GetCompiler()->current_line_number;

    return pStatement;
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
    Elsif *pElsif = (Elsif*)dkc_malloc(sizeof(Elsif));

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

Statement* Create::CreateDoWhileStatement(char *lpstrLabel, Block *pBlock, Expression *pCondition)
{
    Statement *pStatement = AllocStatement(DO_WHILE_STATEMENT);

    pStatement->u.do_while_s.label = lpstrLabel;
    pStatement->u.do_while_s.block = pBlock;
    pStatement->u.do_while_s.condition = pCondition;

    pBlock->type = DO_WHILE_STATEMENT_BLOCK;
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

Block* Create::AllocBlock()
{
    Block *pBlock = (Block*)dkc_malloc(sizeof(Block));

    pBlock->type = UNDEFINED_BLOCK;
    pBlock->outer_block = nullptr;
    pBlock->statement_list = nullptr;
    pBlock->declaration_list = nullptr;

    return pBlock;
}

Block* Create::OpenBlock()
{
    Block *pBlock = AllocBlock();

    pBlock->outer_block = m_Interface.GetCompiler()->current_block;
    m_Interface.GetCompiler()->current_block = pBlock;

    return pBlock;
}

Block* Create::CloseBlock(Block *pBlock, StatementList *pStatementList)
{
    DBG_assert(pBlock == m_Interface.GetCompiler()->current_block, ("block mismatch."));

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

Statement* Create::CreateTryStatement(Block *pTryBlock, CatchClause *pCatchClause, Block *pFinallyBlock)
{
    // 待实现
    return nullptr;
}

CatchClause* Create::CreateCatchClause(TypeSpecifier *pType, char *lpstrVariableName, Block *pBlock)
{
    // 待实现
    return nullptr;
}

CatchClause* Create::StartCatchClause()
{
    // 待实现
    return nullptr;
}

CatchClause* Create::EndCatchClause(CatchClause *pCatchClause, TypeSpecifier *pType, char *lpstrVariableName, Block *pBlock)
{
    // 待实现
    return nullptr;
}

CatchClause* Create::ChainCatchList(CatchClause *pList, CatchClause *pAdd)
{
    // 待实现
    return nullptr;
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
    Declaration *pDeclaration = AllocDeclaration(pType, lpstrIdentifier);

    pDeclaration->initializer = pInitializer;
    pStatement->u.declaration_s = pDeclaration;

    return pStatement;
}

void Create::StartClassDefinition(ClassOrMemberModifierList *pModifier, DVM_ClassOrInterface enClassOrInterface, char *lpstrIdentifier, ExtendsList *pExtends)
{
    DKC_Compiler *pCompiler = m_Interface.GetCompiler();

    ClassDefinition *pClassDefinition = (ClassDefinition*)dkc_malloc(sizeof(ClassDefinition));

    pClassDefinition->is_abstract = (DVM_INTERFACE_DEFINITION == enClassOrInterface) ? DVM_TRUE : DVM_FALSE;
    pClassDefinition->access_modifier = DVM_FILE_ACCESS;

    if (pModifier)
    {
        if (ABSTRACT_MODIFIER == pModifier->is_abstract)
        {
            pClassDefinition->is_abstract = DVM_TRUE;
        }

        pClassDefinition->access_modifier = ConvertAccessModifier(pModifier->access_modifier);
    }

    pClassDefinition->class_or_interface = enClassOrInterface;
    pClassDefinition->package_name = pCompiler->package_name;
    pClassDefinition->name = lpstrIdentifier;
    pClassDefinition->extends = pExtends;
    pClassDefinition->super_class = nullptr;
    pClassDefinition->interface_list = nullptr;
    pClassDefinition->member = nullptr;
    pClassDefinition->next = nullptr;
    pClassDefinition->line_number = pCompiler->current_line_number;

    DBG_assert(nullptr == pCompiler->current_class_definition, ("current_class_definition is not null."));
    pCompiler->current_class_definition = pClassDefinition;
}

void Create::ClassDefine(MemberDeclaration *pMemberList)
{
    DKC_Compiler *pCompiler = m_Interface.GetCompiler();
    ClassDefinition *pClassDefinition = pCompiler->current_class_definition;

    DBG_assert(pClassDefinition != nullptr, ("current_class_definition is null."));

    if (nullptr == pCompiler->class_definition_list)
    {
        pCompiler->class_definition_list = pClassDefinition;
    }
    else
    {
        ClassDefinition *pos = pCompiler->class_definition_list;

        for (; pos->next; pos = pos->next)
            ;

        pos->next = pClassDefinition;
    }

    pClassDefinition->member = pMemberList;
    pCompiler->current_class_definition = nullptr;
}

ExtendsList* Create::CreateExtendsList(char *lpstrIdentifier)
{
    ExtendsList *pList = (ExtendsList*)dkc_malloc(sizeof(ExtendsList));

    pList->identifier = lpstrIdentifier;
    pList->class_definition = nullptr;
    pList->next = nullptr;

    return pList;
}

ExtendsList* Create::ChainExtendsList(ExtendsList *pList, char *pAdd)
{
    ExtendsList *pos = pList;

    for (; pos->next; pos = pos->next)
        ;

    pos->next = CreateExtendsList(pAdd);

    return pList;
}

ClassOrMemberModifierList Create::CreateClassOrMemberModifier(ClassOrMemberModifierKind Modifier)
{
    ClassOrMemberModifierList modifierList;

    modifierList.is_abstract = NOT_SPECIFIED_MODIFIER;
    modifierList.access_modifier = NOT_SPECIFIED_MODIFIER;
    modifierList.is_override = NOT_SPECIFIED_MODIFIER;
    modifierList.is_virtual = NOT_SPECIFIED_MODIFIER;

    switch (Modifier)
    {
    case ABSTRACT_MODIFIER :
        modifierList.is_abstract = ABSTRACT_MODIFIER;
        break;

    case PUBLIC_MODIFIER :
        modifierList.access_modifier = PUBLIC_MODIFIER;
        break;

    case PRIVATE_MODIFIER :
        modifierList.access_modifier = PRIVATE_MODIFIER;
        break;

    case OVERRIDE_MODIFIER :
        modifierList.is_override = OVERRIDE_MODIFIER;
        break;

    case VIRTUAL_MODIFIER :
        modifierList.is_virtual = VIRTUAL_MODIFIER;
        break;

    case NOT_SPECIFIED_MODIFIER :
    default :
        DBG_assert(0, ("modifier..", Modifier));
    }

    return modifierList;
}

ClassOrMemberModifierList Create::ChainClassOrMemberModifier(ClassOrMemberModifierList List, ClassOrMemberModifierList Add)
{
    if (Add.is_abstract != NOT_SPECIFIED_MODIFIER)
    {
        DBG_assert(ABSTRACT_MODIFIER == Add.is_abstract, ("add.is_abstract..", Add.is_abstract));

        if (List.is_abstract != NOT_SPECIFIED_MODIFIER)
        {
            m_Error.CompileError(m_Interface.GetCompiler()->current_line_number, ABSTRACT_MULTIPILE_SPECIFIED_ERR, MESSAGE_ARGUMENT_END);
        }

        List.is_abstract = ABSTRACT_MODIFIER;
    }
    else if (Add.access_modifier != NOT_SPECIFIED_MODIFIER)
    {
        DBG_assert(PUBLIC_MODIFIER == Add.access_modifier || PRIVATE_MODIFIER == Add.access_modifier, ("add.access_modifier..", Add.access_modifier));

        if (List.access_modifier != NOT_SPECIFIED_MODIFIER)
        {
            m_Error.CompileError(m_Interface.GetCompiler()->current_line_number, ACCESS_MODIFIER_MULTIPLE_SPECIFIED_ERR, MESSAGE_ARGUMENT_END);
        }

        List.access_modifier = Add.access_modifier;
    }
    else if (Add.is_override != NOT_SPECIFIED_MODIFIER)
    {
        DBG_assert(OVERRIDE_MODIFIER == Add.is_override, ("add.is_override..", Add.is_override));

        if (List.is_override != NOT_SPECIFIED_MODIFIER)
        {
            m_Error.CompileError(m_Interface.GetCompiler()->current_line_number, OVERRIDE_MODIFIER_MULTIPLE_SPECIFIED_ERR, MESSAGE_ARGUMENT_END);
        }

        List.is_override = Add.is_override;
    }
    else if (Add.is_virtual != NOT_SPECIFIED_MODIFIER)
    {
        DBG_assert(VIRTUAL_MODIFIER == Add.is_virtual, ("add.is_virtual..", Add.is_virtual));

        if (List.is_virtual != NOT_SPECIFIED_MODIFIER)
        {
            m_Error.CompileError(m_Interface.GetCompiler()->current_line_number, VIRTUAL_MODIFIER_MULTIPLE_SPECIFIED_ERR, MESSAGE_ARGUMENT_END);
        }

        List.is_virtual = Add.is_virtual;
    }

    return List;
}

MemberDeclaration* Create::ChainMemberDeclaration(MemberDeclaration *pList, MemberDeclaration *pAdd)
{
    MemberDeclaration *pos = pList;

    for (; pos->next; pos = pos->next)
        ;

    pos->next = pAdd;

    return pList;
}

MemberDeclaration* Create::CreateMethodMember(ClassOrMemberModifierList *pModifier, FunctionDefinition *pFunctionDefinition, DVM_Boolean IsConstructor)
{
    MemberDeclaration *pMemberDeclaration = AllocMemberDeclaration(METHOD_MEMBER, pModifier);

    pMemberDeclaration->u.method.is_constructor = IsConstructor;
    pMemberDeclaration->u.method.is_abstract = DVM_FALSE;
    pMemberDeclaration->u.method.is_virtual = DVM_FALSE;
    pMemberDeclaration->u.method.is_override = DVM_FALSE;

    if (pModifier)
    {
        if (ABSTRACT_MODIFIER == pModifier->is_abstract)
        {
            pMemberDeclaration->u.method.is_abstract = DVM_TRUE;
        }

        if (VIRTUAL_MODIFIER == pModifier->is_virtual)
        {
            pMemberDeclaration->u.method.is_virtual = DVM_TRUE;
        }

        if (OVERRIDE_MODIFIER == pModifier->is_override)
        {
            pMemberDeclaration->u.method.is_override = DVM_TRUE;
        }
    }

    DKC_Compiler *pCompiler = m_Interface.GetCompiler();

    if (DVM_INTERFACE_DEFINITION == pCompiler->current_class_definition->class_or_interface)
    {
        pMemberDeclaration->u.method.is_abstract = DVM_TRUE;
        pMemberDeclaration->u.method.is_virtual = DVM_TRUE;
    }

    pMemberDeclaration->u.method.function_definition = pFunctionDefinition;

    if (pMemberDeclaration->u.method.is_abstract)
    {
        if (pFunctionDefinition->block)
        {
            m_Error.CompileError(pCompiler->current_line_number, ABSTRACT_METHOD_HAS_BODY_ERR, MESSAGE_ARGUMENT_END);
        }
    }
    else
    {
        if (nullptr == pFunctionDefinition->block)
        {
            m_Error.CompileError(pCompiler->current_line_number, CONCRETE_METHOD_HAS_NO_BODY_ERR, MESSAGE_ARGUMENT_END);
        }
    }

    pFunctionDefinition->class_definition = pCompiler->current_class_definition;

    return pMemberDeclaration;
}

FunctionDefinition* Create::MethodFunctionDefine(TypeSpecifier *pType, char *lpstrIdentifier, ParameterList *pParameterList, Block *pBlock)
{
    FunctionDefinition *pFunctionDefinition = CreateFunctionDefinition(pType, lpstrIdentifier, pParameterList, pBlock);

    return pFunctionDefinition;
}

FunctionDefinition* Create::ConstructorFunctionDefine(char *lpstrIdentifier, ParameterList *pParameterList, Block *pBlock)
{
    TypeSpecifier *pType = CreateTypeSpecifier(DVM_VOID_TYPE);
    FunctionDefinition *pFunctionDefinition = MethodFunctionDefine(pType, lpstrIdentifier, pParameterList, pBlock);

    return pFunctionDefinition;
}

MemberDeclaration* Create::CreateFieldMember(ClassOrMemberModifierList *pModifier, TypeSpecifier *pType, char *lpstrName)
{
    MemberDeclaration *pMemberDeclaration = AllocMemberDeclaration(FIELD_MEMBER, pModifier);

    pMemberDeclaration->u.field.name = lpstrName;
    pMemberDeclaration->u.field.type = pType;

    return pMemberDeclaration;
}

PackageName* Create::CreatePackageName(char *lpstrIdentifier)
{
    PackageName *pPackageName = (PackageName*)dkc_malloc(sizeof(PackageName));

    pPackageName->name = lpstrIdentifier;
    pPackageName->next = nullptr;

    return pPackageName;
}

PackageName* Create::ChainPackageName(PackageName *pPackageNameList, char *lpstrIdentifier)
{
    PackageName *pos = pPackageNameList;

    for (; pos->next; pos = pos->next)
        ;

    pos->next = CreatePackageName(lpstrIdentifier);

    return pPackageNameList;
}

RequireList* Create::CreateRequireList(PackageName *pPackageName)
{
    DKC_Compiler *pCompiler = m_Interface.GetCompiler();
    char *pCurrPackageName = m_Util.PackageNameToString(pCompiler->package_name);
    char *pReqPackageName = m_Util.PackageNameToString(pPackageName);

    if (m_Util.CompareString(pCurrPackageName, pReqPackageName) && DKH_SOURCE == pCompiler->source_suffix)
    {
        m_Error.CompileError(pCompiler->current_line_number, REQUIRE_ITSELF_ERR, MESSAGE_ARGUMENT_END);
    }

    MEM_Free(pCurrPackageName);
    MEM_Free(pReqPackageName);

    RequireList *pRequireList = (RequireList*)dkc_malloc(sizeof(RequireList));

    pRequireList->package_name = pPackageName;
    pRequireList->line_number = pCompiler->current_line_number;
    pRequireList->next = nullptr;

    return pRequireList;
}

RequireList* Create::ChainRequireList(RequireList *pList, RequireList *pAdd)
{
    RequireList *pos = pList;
    for (; pos->next; pos = pos->next)
    {
        if (m_Util.ComparePackageName(pos->package_name, pAdd->package_name))
        {
            char *pPackageName = m_Util.PackageNameToString(pAdd->package_name);
            std::string sPackageName(pPackageName);
            MEM_Free(pPackageName);

            m_Error.CompileError(m_Interface.GetCompiler()->current_line_number,
                REQUIRE_DUPLICATE_ERR, STRING_MESSAGE_ARGUMENT, "package_name", sPackageName.c_str(),
                MESSAGE_ARGUMENT_END);
        }
    }

    pos->next = pAdd;
    return pList;
}

RenameList* Create::CreateRenameList(PackageName *pPackageName, char *lpstrIdentifier)
{
    PackageName *pPreTail = nullptr;
    PackageName *pTail = pPackageName;

    for (; pTail->next; pTail = pTail->next)
    {
        pPreTail = pTail;
    }

    if (nullptr == pPreTail)
    {
        m_Error.CompileError(m_Interface.GetCompiler()->current_line_number, RENAME_HAS_NO_PACKAGED_NAME_ERR, MESSAGE_ARGUMENT_END);
    }

    pPreTail->next = nullptr;

    RenameList *pRenameList = (RenameList*)dkc_malloc(sizeof(RenameList));

    pRenameList->package_name = pPackageName;
    pRenameList->original_name = pTail->name;
    pRenameList->renamed_name = lpstrIdentifier;
    pRenameList->line_number = m_Interface.GetCompiler()->current_line_number;
    pRenameList->next = nullptr;

    return pRenameList;
}

RenameList* Create::ChainRenameList(RenameList *pList, RenameList *pAdd)
{
    RenameList *pos = pList;

    for (; pos->next; pos = pos->next)
        ;

    pos->next = pAdd;

    return pList;
}

void Create::SetRequireAndRenameList(RequireList *pRequireList, RenameList *pRenameList)
{
    DKC_Compiler *pCompiler = m_Interface.GetCompiler();
    char *pCurrentPackageName = m_Util.PackageNameToString(pCompiler->package_name);

    if (!m_Util.CompareString(pCurrentPackageName, DVM_DIKSAM_DEFAULT_PACKAGE))
    {
        pRequireList = AddDefaultPackage(pRequireList);
    }

    MEM_Free(pCurrentPackageName);

    pCompiler->require_list = pRequireList;
    pCompiler->rename_list = pRenameList;
}

RequireList* Create::AddDefaultPackage(RequireList *pRequireList)
{
    bool bDefaultPackageRequired = false;

    for (RequireList *pos = pRequireList; pos; pos = pos->next)
    {
        char *tempName = m_Util.PackageNameToString(pos->package_name);
        if (DVM_DIKSAM_DEFAULT_PACKAGE == std::string(tempName))
        {
            bDefaultPackageRequired = true;
        }
        MEM_Free(tempName);
    }

    if (!bDefaultPackageRequired)
    {
        PackageName *pPackageName = CreatePackageName(DVM_DIKSAM_DEFAULT_PACKAGE_P1);
        pPackageName = ChainPackageName(pPackageName, DVM_DIKSAM_DEFAULT_PACKAGE_P2);

        RequireList *tempReq = pRequireList;
        pRequireList = CreateRequireList(pPackageName);
        pRequireList->next = tempReq;
    }

    return pRequireList;
}

void Create::AddFunctionToCompiler(FunctionDefinition *pFunctionDefinition)
{
    DKC_Compiler *pCompiler = m_Interface.GetCompiler();

    if (pCompiler->function_list)
    {
        FunctionDefinition *pos = pCompiler->function_list;

        for (; pos->next; pos = pos->next)
            ;

        pos->next = pFunctionDefinition;
    }
    else
    {
        pCompiler->function_list = pFunctionDefinition;
    }
}

DVM_AccessModifier Create::ConvertAccessModifier(ClassOrMemberModifierKind src)
{
    if (PUBLIC_MODIFIER == src)
    {
        return DVM_PUBLIC_ACCESS;
    }
    else if (PRIVATE_MODIFIER)
    {
        return DVM_PRIVATE_ACCESS;
    }
    else
    {
        DBG_assert(NOT_SPECIFIED_MODIFIER == src, ("src..", src));
        return DVM_FILE_ACCESS;
    }
}

MemberDeclaration* Create::AllocMemberDeclaration(MemberKind enKind, ClassOrMemberModifierList *pModifierList)
{
    MemberDeclaration *pMemberDeclaration = (MemberDeclaration*)dkc_malloc(sizeof(MemberDeclaration));

    pMemberDeclaration->kind = enKind;

    if (pModifierList)
    {
        pMemberDeclaration->access_modifier = ConvertAccessModifier(pModifierList->access_modifier);
    }
    else
    {
        pMemberDeclaration->access_modifier = DVM_FILE_ACCESS;
    }

    pMemberDeclaration->line_number = m_Interface.GetCompiler()->current_line_number;
    pMemberDeclaration->next = nullptr;

    return pMemberDeclaration;
}