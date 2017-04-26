#include "StdAfx.h"
#include "Generate.h"
#include "Debug.h"
#include "Memory.h"
#include "Error.h"
#include "OpcodeInfo.h"

Generate::Generate(Debug& debug, Memory& memory, Error& error)
    : m_Debug(debug)
    , m_Memory(memory)
    , m_Error(error)
{

}

Generate::~Generate()
{

}

DVM_Executable* Generate::operator () (DKC_Compiler *pCompiler)
{
    return nullptr;
}

int Generate::AddConstantPool(DVM_Executable *pExecutable, DVM_ConstantPool *pConstantPool)
{
    pExecutable->constant_pool = (DVM_ConstantPool*)GENERATE_MEM_Realloc(pExecutable->constant_pool, sizeof(DVM_ConstantPool) * (pExecutable->constant_pool_count + 1));
    pExecutable->constant_pool[pExecutable->constant_pool_count] = *pConstantPool;

    return pExecutable->constant_pool_count++;
}

void Generate::AddGlobalVariable(DKC_Compiler *pCompiler, DVM_Executable *pExecutable)
{
    int iVariableCount = 0;

    for (DeclarationList *pDeclarationList = pCompiler->declaration_list; pDeclarationList; pDeclarationList = pDeclarationList->next)
    {
        iVariableCount++;
    }

    pExecutable->global_variable_count = iVariableCount;
    pExecutable->global_variable = (DVM_Variable*)GENERATE_MEM_Malloc(sizeof(DVM_Variable) * iVariableCount);

    int i = 0;
    for (DeclarationList *pDeclarationList = pCompiler->declaration_list; pDeclarationList; pDeclarationList = pDeclarationList->next, i++)
    {
        pExecutable->global_variable[i].name = GENERATE_MEM_StrDUP(pDeclarationList->declaration->name);
        pExecutable->global_variable[i].type = CopyTypeSpecifier(pDeclarationList->declaration->type);
    }
}

void Generate::AddLineNumber(OpcodeBuf *pOpcode, int iLine, int iStartPC)
{
    if (nullptr == pOpcode->m_LineNumber
        || pOpcode->m_LineNumber[pOpcode->m_iLineNumberSize - 1].line_number != iLine)
    {
        pOpcode->m_LineNumber = (DVM_LineNumber*)GENERATE_MEM_Realloc(pOpcode->m_LineNumber, sizeof(DVM_LineNumber)* (pOpcode->m_iLineNumberSize + 1));
        pOpcode->m_LineNumber[pOpcode->m_iLineNumberSize].line_number = iLine;
        pOpcode->m_LineNumber[pOpcode->m_iLineNumberSize].start_pc = iStartPC;
        pOpcode->m_LineNumber[pOpcode->m_iLineNumberSize].pc_count = pOpcode->m_iSize - iStartPC;
        pOpcode->m_iLineNumberSize++;
    }
    else
    {
        pOpcode->m_LineNumber[pOpcode->m_iLineNumberSize - 1].pc_count += pOpcode->m_iSize - iStartPC;
    }
}

void Generate::GenerateCode(OpcodeBuf *pOpcode, int iLine, DVM_Opcode code, ...)
{
    va_list ap;

    va_start(ap, code);

    const char *param = DVMOpcodeInfo::Opcode()[int(code)].parameter;
    int paramLen = std::string(param).length();

    if (pOpcode->m_iAllocSize < pOpcode->m_iSize + 1 + (paramLen * 2))
    {
        pOpcode->m_pCode = (DVM_Byte*)GENERATE_MEM_Realloc(pOpcode->m_pCode, pOpcode->m_iAllocSize + OPCODE_ALLOC_SIZE);
        pOpcode->m_iAllocSize += OPCODE_ALLOC_SIZE;
    }

    int iStartPC = pOpcode->m_iSize;

    pOpcode->m_pCode[pOpcode->m_iSize++] = code;

    for (int i = 0; param[i] != '\0'; i++)
    {
        unsigned int value = va_arg(ap, int);

        switch (param[i])
        {
        case 'b' :  // byte
            pOpcode->m_pCode[pOpcode->m_iSize++] = (DVM_Byte)value;
            break;

        case 's' :  // short (2 byte int)
            pOpcode->m_pCode[pOpcode->m_iSize] = (DVM_Byte)(value >> 8);
            pOpcode->m_pCode[pOpcode->m_iSize + 1] = (DVM_Byte)(value & 0xFF);
            pOpcode->m_iSize += 2;
            break;

        case 'p' :  // constant pool index
            pOpcode->m_pCode[pOpcode->m_iSize] = (DVM_Byte)(value >> 8);
            pOpcode->m_pCode[pOpcode->m_iSize + 1] = (DVM_Byte)(value & 0xFF);
            pOpcode->m_iSize += 2;
            break;

        default :
            GENERATE_DBG_Assert(0, ("param..", param, ", i..", i));
        }
    }

    AddLineNumber(pOpcode, iLine, iStartPC);

    va_end(ap);
}

void Generate::GenerateBooleanExpression(DVM_Executable *pExecutable, Expression *pExpression, OpcodeBuf *pOpcode)
{
    GenerateCode(pOpcode, pExpression->line_number, DVM_PUSH_INT_1BYTE, (DVM_TRUE == pExpression->u.boolean_value ? 1 : 0));
}

void Generate::GenerateIntExpression(DVM_Executable *pExecutable, Expression *pExpression, OpcodeBuf *pOpcode)
{
    if (pExpression->u.int_value >= 0 && pExpression->u.int_value < 256)
    {
        GenerateCode(pOpcode, pExpression->line_number, DVM_PUSH_INT_1BYTE, pExpression->u.int_value);
    }
    else if (pExpression->u.int_value >= 0 && pExpression->u.int_value < 65536)
    {
        GenerateCode(pOpcode, pExpression->line_number, DVM_PUSH_INT_2BYTE, pExpression->u.int_value);
    }
    else
    {
        DVM_ConstantPool cPool;

        cPool.tag = DVM_CONSTANT_INT;
        cPool.u.c_int = pExpression->u.int_value;
        
        int cpIdx = AddConstantPool(pExecutable, &cPool);
        GenerateCode(pOpcode, pExpression->line_number, DVM_PUSH_INT, cpIdx);
    }
}

void Generate::GenerateDoubleExpression(DVM_Executable *pExecutable, Expression *pExpression, OpcodeBuf *pOpcode)
{
    if (double(std::abs(pExpression->u.double_value - 0.0)) < eps)
    {
        GenerateCode(pOpcode, pExpression->line_number, DVM_PUSH_DOUBLE_0);
    }
    else if (double(std::abs(pExpression->u.double_value - 1.0)) < eps)
    {
        GenerateCode(pOpcode, pExpression->line_number, DVM_PUSH_DOUBLE_1);
    }
    else
    {
        DVM_ConstantPool cPool;

        cPool.tag = DVM_CONSTANT_DOUBLE;
        cPool.u.c_double = pExpression->u.double_value;

        int cpIdx = AddConstantPool(pExecutable, &cPool);
        GenerateCode(pOpcode, pExpression->line_number, DVM_PUSH_DOUBLE, cpIdx);
    }
}

void Generate::GenerateStringExpression(DVM_Executable *pExecutable, Expression *pExpression, OpcodeBuf *pOpcode)
{
    DVM_ConstantPool cPool;

    cPool.tag = DVM_CONSTANT_STRING;
    cPool.u.c_string = pExpression->u.string_value;

    int cpIdx = AddConstantPool(pExecutable, &cPool);
    GenerateCode(pOpcode, pExpression->line_number, DVM_PUSH_STRING, cpIdx);
}

void Generate::GenerateIdentifierExpression(DVM_Executable *pExecutable, Block *pBlock, Expression *pExpression, OpcodeBuf *pOpcode)
{
    if (pExpression->u.identifier.is_function)
    {
        GenerateCode(pOpcode, pExpression->line_number, DVM_PUSH_FUNCTION, pExpression->u.identifier.u.function->index);
        return;
    }

    DVM_Opcode code = pExpression->u.identifier.u.declaration->is_local ? DVM_PUSH_STACK_INT : DVM_PUSH_STATIC_INT;

    GenerateCode(pOpcode, pExpression->line_number,
        DVM_Opcode(code + GetOpcodeTypeOffset(pExpression->u.identifier.u.declaration->type->basic_type)),
        pExpression->u.identifier.u.declaration->variable_index);
}

void Generate::GeneratePopToIdentifier(Declaration *pDeclaration, int iLine, OpcodeBuf *pOpcode)
{
    DVM_Opcode code = pDeclaration->is_local ? DVM_POP_STACK_INT : DVM_POP_STATIC_INT;

    GenerateCode(pOpcode, iLine,
        DVM_Opcode(code + GetOpcodeTypeOffset(pDeclaration->type->basic_type)),
        pDeclaration->variable_index);
}

void Generate::GenerateAssignExpression(DVM_Executable *pExecutable, Block *pBlock, Expression *pExpression, OpcodeBuf *pOpcode, bool isTopLevel)
{
    if (pExpression->u.assign_expression.op != NORMAL_ASSIGN)
    {
        GenerateIdentifierExpression(pExecutable, pBlock, pExpression->u.assign_expression.left, pOpcode);
    }

    GenerateExpression(pExecutable, pBlock, pExpression->u.assign_expression.operand, pOpcode);

    DVM_Opcode code;
    switch (pExpression->u.assign_expression.op)
    {
    case NORMAL_ASSIGN :
        code = DVM_ADD_INT;
        break;

    case SUB_ASSIGN :
        code = DVM_SUB_INT;
        break;

    case MUL_ASSIGN :
        code = DVM_MUL_INT;
        break;

    case DIV_ASSIGN :
        code = DVM_DIV_INT;
        break;

    case MOD_ASSIGN :
        code = DVM_MOD_INT;
        break;

    default :
        GENERATE_DBG_Assert(0, ("operator..", pExpression->u.assign_expression.op));
    }

    GenerateCode(pOpcode, pExpression->line_number, DVM_Opcode(code + GetOpcodeTypeOffset(pExpression->type->basic_type)));

    if (!isTopLevel)
    {
        GenerateCode(pOpcode, pExpression->line_number, DVM_DUPLICATE);
    }

    GeneratePopToIdentifier(pExpression->u.assign_expression.left->u.identifier.u.declaration,
        pExpression->line_number, pOpcode);
}

void Generate::GenerateBinaryExpression(DVM_Executable *pExecutable, Block *pBlock, Expression *pExpression, DVM_Opcode opCode, OpcodeBuf *pOpcode)
{
    GENERATE_DBG_Assert(pExpression->u.binary_expression.left->type->basic_type
        == pExpression->u.binary_expression.right->type->basic_type,
        ("left..", pExpression->u.binary_expression.left->type->basic_type,
        ", right..", pExpression->u.binary_expression.right->type->basic_type));

    GenerateExpression(pExecutable, pBlock, pExpression->u.binary_expression.left, pOpcode);
    GenerateExpression(pExecutable, pBlock, pExpression->u.binary_expression.right, pOpcode);
    GenerateCode(pOpcode, pExpression->line_number,
        DVM_Opcode(opCode + GetOpcodeTypeOffset(pExpression->u.binary_expression.left->type->basic_type)));
}

void Generate::GenerateLogicalAndExpression(DVM_Executable *pExecutable, Block *pBlock, Expression *pExpression, OpcodeBuf *pOpcode)
{
    int falseLabel = GetLabel(pOpcode);

    GenerateExpression(pExecutable, pBlock, pExpression->u.binary_expression.left, pOpcode);
    GenerateCode(pOpcode, pExpression->line_number, DVM_DUPLICATE);
    GenerateCode(pOpcode, pExpression->line_number, DVM_JUMP_IF_FALSE, falseLabel);
    GenerateExpression(pExecutable, pBlock, pExpression->u.binary_expression.right, pOpcode);
    GenerateCode(pOpcode, pExpression->line_number, DVM_LOGICAL_AND);
    SetLabel(pOpcode, falseLabel);
}

void Generate::GenerateLogicalOrExpression(DVM_Executable *pExecutable, Block *pBlock, Expression *pExpression, OpcodeBuf *pOpcode)
{
    int trueLabel = GetLabel(pOpcode);

    GenerateExpression(pExecutable, pBlock, pExpression->u.binary_expression.left, pOpcode);
    GenerateCode(pOpcode, pExpression->line_number, DVM_DUPLICATE);
    GenerateCode(pOpcode, pExpression->line_number, DVM_JUMP_IF_TRUE, trueLabel);
    GenerateExpression(pExecutable, pBlock, pExpression->u.binary_expression.right, pOpcode);
    GenerateCode(pOpcode, pExpression->line_number, DVM_LOGICAL_OR);
    SetLabel(pOpcode, trueLabel);
}

void Generate::GenerateCastExpression(DVM_Executable *pExecutable, Block *pBlock, Expression *pExpression, OpcodeBuf *pOpcode)
{
    GenerateExpression(pExecutable, pBlock, pExpression->u.cast.operand, pOpcode);

    switch (pExpression->u.cast.type)
    {
    case INT_TO_DOUBLE_CAST :
        GenerateCode(pOpcode, pExpression->line_number, DVM_CAST_INT_TO_DOUBLE);
        break;

    case DOUBLE_TO_INT_CAST :
        GenerateCode(pOpcode, pExpression->line_number, DVM_CAST_DOUBLE_TO_INT);
        break;

    case BOOLEAN_TO_STRING_CAST :
        GenerateCode(pOpcode, pExpression->line_number, DVM_CAST_BOOLEAN_TO_STRING);
        break;

    case INT_TO_STRING_CAST :
        GenerateCode(pOpcode, pExpression->line_number, DVM_CAST_INT_TO_STRING);
        break;

    case DOUBLE_TO_STRING_CAST :
        GenerateCode(pOpcode, pExpression->line_number, DVM_CAST_DOUBLE_TO_STRING);
        break;

    default :
        GENERATE_DBG_Assert(0, ("pExpression->u.cast.type..", pExpression->u.cast.type));
    }
}

void Generate::GenerateIncDecExpression(DVM_Executable *pExecutable, Block *pBlock, Expression *pExpression, ExpressionKind kind, OpcodeBuf *pOpcode, bool isTopLevel)
{
    GenerateExpression(pExecutable, pBlock, pExpression->u.inc_dec.operand, pOpcode);

    switch (kind)
    {
    case INCREMENT_EXPRESSION :
        GenerateCode(pOpcode, pExpression->line_number, DVM_INCREMENT);
        break;

    case DECREMENT_EXPRESSION :
        GenerateCode(pOpcode, pExpression->line_number, DVM_DECREMENT);
        break;

    default :
        GENERATE_DBG_Assert(0, ("kind..", kind));
    }

    if (!isTopLevel)
    {
        GenerateCode(pOpcode, pExpression->line_number, DVM_DUPLICATE);
    }

    GeneratePopToIdentifier(pExpression->u.inc_dec.operand->u.identifier.u.declaration,
        pExpression->line_number, pOpcode);
}

void Generate::GenerateFunctionCallExpression(DVM_Executable *pExecutable, Block *pBlock, Expression *pExpression, OpcodeBuf *pOpcode)
{
    FunctionCallExpression *pFunctionCallExpression = &pExpression->u.function_call_expression;

    for (ArgumentList *pArgList = pFunctionCallExpression->argument; pArgList; pArgList = pArgList->next)
    {
        GenerateExpression(pExecutable, pBlock, pArgList->expression, pOpcode);
    }

    GenerateExpression(pExecutable, pBlock, pFunctionCallExpression->function, pOpcode);
    GenerateCode(pOpcode, pExpression->line_number, DVM_INVOKE);
}

void Generate::GenerateExpression(DVM_Executable *pExecutable, Block *pBlock, Expression *pExpression, OpcodeBuf *pOpcode)
{
    switch (pExpression->kind)
    {
    case BOOLEAN_EXPRESSION :
        GenerateBooleanExpression(pExecutable, pExpression, pOpcode);
        break;

    case INT_EXPRESSION :
        GenerateIntExpression(pExecutable, pExpression, pOpcode);
        break;

    case DOUBLE_EXPRESSION :
        GenerateDoubleExpression(pExecutable, pExpression, pOpcode);
        break;

    case STRING_EXPRESSION :
        GenerateStringExpression(pExecutable, pExpression, pOpcode);
        break;

    case IDENTIFIER_EXPRESSION :
        GenerateIdentifierExpression(pExecutable, pBlock, pExpression, pOpcode);
        break;

    case COMMA_EXPRESSION :
        GenerateExpression(pExecutable, pBlock, pExpression->u.comma.left, pOpcode);
        GenerateExpression(pExecutable, pBlock, pExpression->u.comma.right, pOpcode);
        break;

    case ASSIGN_EXPRESSION :
        GenerateAssignExpression(pExecutable, pBlock, pExpression, pOpcode, false);
        break;

    case ADD_EXPRESSION :
        GenerateBinaryExpression(pExecutable, pBlock, pExpression, DVM_ADD_INT, pOpcode);
        break;

    case SUB_EXPRESSION :
        GenerateBinaryExpression(pExecutable, pBlock, pExpression, DVM_SUB_INT, pOpcode);
        break;

    case MUL_EXPRESSION :
        GenerateBinaryExpression(pExecutable, pBlock, pExpression, DVM_MUL_INT, pOpcode);
        break;

    case DIV_EXPRESSION :
        GenerateBinaryExpression(pExecutable, pBlock, pExpression, DVM_DIV_INT, pOpcode);
        break;

    case MOD_EXPRESSION :
        GenerateBinaryExpression(pExecutable, pBlock, pExpression, DVM_MOD_INT, pOpcode);
        break;

    case EQ_EXPRESSION :
        GenerateBinaryExpression(pExecutable, pBlock, pExpression, DVM_EQ_INT, pOpcode);
        break;

    case NE_EXPRESSION :
        GenerateBinaryExpression(pExecutable, pBlock, pExpression, DVM_NE_INT, pOpcode);
        break;

    case GT_EXPRESSION :
        GenerateBinaryExpression(pExecutable, pBlock, pExpression, DVM_GT_INT, pOpcode);
        break;

    case GE_EXPRESSION :
        GenerateBinaryExpression(pExecutable, pBlock, pExpression, DVM_GE_INT, pOpcode);
        break;

    case LT_EXPRESSION :
        GenerateBinaryExpression(pExecutable, pBlock, pExpression, DVM_LT_INT, pOpcode);
        break;

    case LE_EXPRESSION :
        GenerateBinaryExpression(pExecutable, pBlock, pExpression, DVM_LE_INT, pOpcode);
        break;

    case LOGICAL_AND_EXPRESSION :
        GenerateLogicalAndExpression(pExecutable, pBlock, pExpression, pOpcode);
        break;

    case LOGICAL_OR_EXPRESSION :
        GenerateLogicalOrExpression(pExecutable, pBlock, pExpression, pOpcode);
        break;

    case MINUS_EXPRESSION :
        GenerateExpression(pExecutable, pBlock, pExpression->u.minus_expression, pOpcode);
        GenerateCode(pOpcode, pExpression->line_number,
            DVM_Opcode(DVM_MINUS_INT + GetOpcodeTypeOffset(pExpression->type->basic_type)));
        break;

    case LOGICAL_NOT_EXPRESSION :
        GenerateExpression(pExecutable, pBlock, pExpression->u.logical_not, pOpcode);
        GenerateCode(pOpcode, pExpression->line_number, DVM_LOGICAL_NOT);
        break;

    case FUNCTION_CALL_EXPRESSION :
        GenerateFunctionCallExpression(pExecutable, pBlock, pExpression, pOpcode);
        break;

    case INCREMENT_EXPRESSION :
    case DECREMENT_EXPRESSION :
        GenerateIncDecExpression(pExecutable, pBlock, pExpression, pExpression->kind, pOpcode, false);
        break;

    case CAST_EXPRESSION :
        GenerateCastExpression(pExecutable, pBlock, pExpression, pOpcode);
        break;

    default :
        GENERATE_DBG_Assert(0, ("pExpression->kind..", pExpression->kind));
    }
}

void Generate::GenerateExpressionStatement(DVM_Executable *pExecutable, Block *pBlock, Expression *pExpression, OpcodeBuf *pOpcode)
{
    switch (pExpression->kind)
    {
    case ASSIGN_EXPRESSION :
        GenerateAssignExpression(pExecutable, pBlock, pExpression, pOpcode, true);
        break;

    case INCREMENT_EXPRESSION :
    case DECREMENT_EXPRESSION :
        GenerateIncDecExpression(pExecutable, pBlock, pExpression, pExpression->kind, pOpcode, true);
        break;

    default :
        GenerateExpression(pExecutable, pBlock, pExpression, pOpcode);
        GenerateCode(pOpcode, pExpression->line_number, DVM_POP);
        break;
    }
}

void Generate::GenerateIfStatement(DVM_Executable *pExecutable, Block *pBlock, Statement *pStatement, OpcodeBuf *pOpcode)
{
    IfStatement *pIfStatement = &pStatement->u.if_s;

    GenerateExpression(pExecutable, pBlock, pIfStatement->condition, pOpcode);

    int ifFalseLabel = GetLabel(pOpcode);
    GenerateCode(pOpcode, pStatement->line_number, DVM_JUMP_IF_FALSE, ifFalseLabel);
    GenerateStatementList(pExecutable, pIfStatement->then_block, pIfStatement->then_block->statement_list, pOpcode);

    int endLabel = GetLabel(pOpcode);
    GenerateCode(pOpcode, pStatement->line_number, DVM_JUMP, endLabel);
    SetLabel(pOpcode, ifFalseLabel);

    for (Elsif *pElsIf = pIfStatement->elsif_list; pElsIf; pElsIf = pElsIf->next)
    {
        GenerateExpression(pExecutable, pBlock, pElsIf->condition, pOpcode);

        ifFalseLabel = GetLabel(pOpcode);
        GenerateCode(pOpcode, pStatement->line_number, DVM_JUMP_IF_FALSE, ifFalseLabel);
        GenerateStatementList(pExecutable, pElsIf->block, pElsIf->block->statement_list, pOpcode);
        GenerateCode(pOpcode, pStatement->line_number, DVM_JUMP, endLabel);
        SetLabel(pOpcode, ifFalseLabel);
    }

    if (pIfStatement->else_block)
    {
        GenerateStatementList(pExecutable, pIfStatement->else_block, pIfStatement->else_block->statement_list, pOpcode);
    }

    SetLabel(pOpcode, endLabel);
}

void Generate::GenerateWhileStatement(DVM_Executable *pExecutable, Block *pBlock, Statement *pStatement, OpcodeBuf *pOpcode)
{
    WhileStatement *pWhileStatement = &pStatement->u.while_s;

    int loopLabel = GetLabel(pOpcode);

    SetLabel(pOpcode, loopLabel);
    GenerateExpression(pExecutable, pBlock, pWhileStatement->condition, pOpcode);

    pWhileStatement->block->parent.statement.break_label = GetLabel(pOpcode);
    pWhileStatement->block->parent.statement.continue_label = GetLabel(pOpcode);

    GenerateCode(pOpcode, pStatement->line_number, DVM_JUMP_IF_FALSE, pWhileStatement->block->parent.statement.break_label);
    GenerateStatementList(pExecutable, pWhileStatement->block, pWhileStatement->block->statement_list, pOpcode);

    SetLabel(pOpcode, pWhileStatement->block->parent.statement.continue_label);
    GenerateCode(pOpcode, pStatement->line_number, DVM_JUMP, loopLabel);
    SetLabel(pOpcode, pWhileStatement->block->parent.statement.break_label);
}

void Generate::GenerateForStatement(DVM_Executable *pExecutable, Block *pBlock, Statement *pStatement, OpcodeBuf *pOpcode)
{
    ForStatement *pForStatement = &pStatement->u.for_s;

    if (pForStatement->init)
    {
        GenerateExpressionStatement(pExecutable, pBlock, pForStatement->init, pOpcode);
    }

    int loopLabel = GetLabel(pOpcode);

    SetLabel(pOpcode, loopLabel);

    if (pForStatement->condition)
    {
        GenerateExpression(pExecutable, pBlock, pForStatement->condition, pOpcode);
    }

    pForStatement->block->parent.statement.break_label = GetLabel(pOpcode);
    pForStatement->block->parent.statement.continue_label = GetLabel(pOpcode);

    if (pForStatement->condition)
    {
        GenerateCode(pOpcode, pStatement->line_number, DVM_JUMP_IF_FALSE,
            pForStatement->block->parent.statement.break_label);
    }

    GenerateStatementList(pExecutable, pForStatement->block, pForStatement->block->statement_list, pOpcode);

    SetLabel(pOpcode, pForStatement->block->parent.statement.continue_label);

    if (pForStatement->post)
    {
        GenerateExpressionStatement(pExecutable, pBlock, pForStatement->post, pOpcode);
    }

    GenerateCode(pOpcode, pStatement->line_number, DVM_JUMP, loopLabel);
    SetLabel(pOpcode, pForStatement->block->parent.statement.break_label);
}

void Generate::GenerateReturnStatement(DVM_Executable *pExecutable, Block *pBlock, Statement *pStatement, OpcodeBuf *pOpcode)
{
    GENERATE_DBG_Assert(pStatement->u.return_s.return_value, ("return value is null."));

    GenerateExpression(pExecutable, pBlock, pStatement->u.return_s.return_value, pOpcode);
    GenerateCode(pOpcode, pStatement->line_number, DVM_RETURN);
}

void Generate::GenerateBreakStatement(DVM_Executable *pExecutable, Block *pBlock, Statement *pStatement, OpcodeBuf *pOpcode)
{
    BreakStatement *pBreakStatement = &pStatement->u.break_s;
    Block *pCurrBlock = nullptr;

    for (pCurrBlock = pBlock; pCurrBlock; pCurrBlock = pCurrBlock->outer_block)
    {
        if (pCurrBlock->type != WHILE_STATEMENT_BLOCK && pCurrBlock->type != FOR_STATEMENT_BLOCK)
            continue;

        if (nullptr == pBreakStatement->label)
            break;

        if (WHILE_STATEMENT_BLOCK == pCurrBlock->type)
        {
            if (nullptr == pCurrBlock->parent.statement.statement->u.while_s.label)
                continue;

            if (std::string(pBreakStatement->label) == pCurrBlock->parent.statement.statement->u.while_s.label)
                break;
        }
        else if (FOR_STATEMENT_BLOCK == pCurrBlock->type)
        {
            if (nullptr == pCurrBlock->parent.statement.statement->u.for_s.label)
                continue;

            if (std::string(pBreakStatement->label) == pCurrBlock->parent.statement.statement->u.for_s.label)
                break;
        }
    }

    if (nullptr == pCurrBlock)
    {
        m_Error.CompileError(pStatement->line_number,
            LABEL_NOT_FOUND_ERR, STRING_MESSAGE_ARGUMENT, "label", pBreakStatement->label,
            MESSAGE_ARGUMENT_END);
    }

    GenerateCode(pOpcode, pStatement->line_number, DVM_JUMP, pCurrBlock->parent.statement.break_label);
}

void Generate::GenerateContinueStatement(DVM_Executable *pExecutable, Block *pBlock, Statement *pStatement, OpcodeBuf *pOpcode)
{
    ContinueStatement *pContinueStatement = &pStatement->u.continue_s;
    Block *pCurrBlock = nullptr;

    for (pCurrBlock = pBlock; pCurrBlock; pCurrBlock = pCurrBlock->outer_block)
    {
        if (pCurrBlock->type != WHILE_STATEMENT_BLOCK && pCurrBlock->type != FOR_STATEMENT_BLOCK)
            continue;

        if (nullptr == pContinueStatement->label)
            break;

        if (WHILE_STATEMENT_BLOCK == pCurrBlock->type)
        {
            if (nullptr == pCurrBlock->parent.statement.statement->u.while_s.label)
                continue;

            if (std::string(pContinueStatement->label) == pCurrBlock->parent.statement.statement->u.while_s.label)
                break;
        }
        else if (FOR_STATEMENT_BLOCK == pCurrBlock->type)
        {
            if (nullptr == pCurrBlock->parent.statement.statement->u.for_s.label)
                continue;

            if (std::string(pContinueStatement->label) == pCurrBlock->parent.statement.statement->u.for_s.label)
                break;
        }
    }

    if (nullptr == pCurrBlock)
    {
        m_Error.CompileError(pStatement->line_number,
            LABEL_NOT_FOUND_ERR, STRING_MESSAGE_ARGUMENT, "label", pContinueStatement->label,
            MESSAGE_ARGUMENT_END);
    }

    GenerateCode(pOpcode, pStatement->line_number, DVM_JUMP, pCurrBlock->parent.statement.continue_label);
}

void Generate::GenerateInitializer(DVM_Executable *pExecutable, Block *pBlock, Statement *pStatement, OpcodeBuf *pOpcode)
{
    Declaration *pDeclaration = pStatement->u.declaration_s;
    if (nullptr == pDeclaration->initializer)
        return;

    GenerateExpression(pExecutable, pBlock, pDeclaration->initializer, pOpcode);
    GeneratePopToIdentifier(pDeclaration, pStatement->line_number, pOpcode);
}

void Generate::GenerateStatementList(DVM_Executable *pExecutable, Block *pBlock, StatementList *pStatementList, OpcodeBuf *pOpcode)
{
    StatementList *pos = pStatementList;

    for (; pos; pos = pos->next)
    {
        switch (pos->statement->type)
        {
        case EXPRESSION_STATEMENT :
            GenerateExpressionStatement(pExecutable, pBlock, pos->statement->u.expression_s, pOpcode);
            break;

        case IF_STATEMENT :
            GenerateIfStatement(pExecutable, pBlock, pos->statement, pOpcode);
            break;

        case WHILE_STATEMENT :
            GenerateWhileStatement(pExecutable, pBlock, pos->statement, pOpcode);
            break;

        case FOR_STATEMENT :
            GenerateForStatement(pExecutable, pBlock, pos->statement, pOpcode);
            break;

        case RETURN_STATEMENT :
            GenerateReturnStatement(pExecutable, pBlock, pos->statement, pOpcode);
            break;

        case BREAK_STATEMENT :
            GenerateBreakStatement(pExecutable, pBlock, pos->statement, pOpcode);
            break;

        case CONTINUE_STATEMENT :
            GenerateContinueStatement(pExecutable, pBlock, pos->statement, pOpcode);
            break;

        case TRY_STATEMENT :
            break;

        case THROW_STATEMENT :
            break;

        case DECLARATION_STATEMENT :
            GenerateInitializer(pExecutable, pBlock, pos->statement, pOpcode);
            break;

        default :
            GENERATE_DBG_Assert(0, ("pos->statement->type..", pos->statement->type));
        }
    }
}

DVM_Executable* Generate::AllocExecutable()
{
    DVM_Executable *pExecutable = (DVM_Executable*)GENERATE_MEM_Malloc(sizeof(DVM_Executable));

    pExecutable->constant_pool_count = 0;
    pExecutable->constant_pool = nullptr;
    pExecutable->global_variable_count = 0;
    pExecutable->global_variable = nullptr;
    pExecutable->function_count = 0;
    pExecutable->function = nullptr;
    pExecutable->code_size = 0;
    pExecutable->code = nullptr;

    return pExecutable;
}

DVM_LocalVariable* Generate::CopyParameterList(ParameterList *pParameterList, int *pParameterCount)
{
    int iParamCount = 0;

    for (ParameterList *pParam = pParameterList; pParam; pParam = pParam->next)
    {
        iParamCount++;
    }

    *pParameterCount = iParamCount;

    DVM_LocalVariable *pDest = (DVM_LocalVariable*)GENERATE_MEM_Malloc(sizeof(DVM_LocalVariable)* iParamCount);

    int i = 0;
    for (ParameterList *pParam = pParameterList; pParam; pParam = pParam->next, i++)
    {
        pDest[i].name = GENERATE_MEM_StrDUP(pParam->name);
        pDest[i].type = CopyTypeSpecifier(pParam->type);
    }

    return pDest;
}

DVM_LocalVariable* Generate::CopyLocalVariables(FunctionDefinition *pFunctionDefinition, int iParameterCount)
{
    int iLocalVariableCount = pFunctionDefinition->local_variable_count - iParameterCount;
    DVM_LocalVariable *pDest = (DVM_LocalVariable*)GENERATE_MEM_Malloc(sizeof(DVM_LocalVariable) * iLocalVariableCount);

    for (int i = 0; i < iLocalVariableCount; i++)
    {
        pDest[i].name = GENERATE_MEM_StrDUP(pFunctionDefinition->local_variable[iParameterCount + i]->name);
        pDest[i].type = CopyTypeSpecifier(pFunctionDefinition->local_variable[iParameterCount + i]->type);
    }

    return pDest;
}

DVM_TypeSpecifier* Generate::CopyTypeSpecifier(TypeSpecifier *pTypeSpecifier)
{
    DVM_TypeSpecifier *pDestTypeSpecifier = (DVM_TypeSpecifier*)GENERATE_MEM_Malloc(sizeof(DVM_TypeSpecifier));

    pDestTypeSpecifier->basic_type = pTypeSpecifier->basic_type;

    int iDeriveCount = 0;
    for (TypeDerive *pDerive = pTypeSpecifier->derive; pDerive; pDerive = pDerive->next)
    {
        iDeriveCount++;
    }

    pDestTypeSpecifier->derive_count = iDeriveCount;
    pDestTypeSpecifier->derive = (DVM_TypeDerive*)GENERATE_MEM_Malloc(sizeof(DVM_TypeDerive)* iDeriveCount);

    int i = 0;
    for (TypeDerive *pDerive = pTypeSpecifier->derive; pDerive; pDerive = pDerive->next, i++)
    {
        switch (pDerive->tag)
        {
        case DVM_FUNCTION_DERIVE:
            {
                int iParameterCount = 0;

                pDestTypeSpecifier->derive[i].tag = DVM_FUNCTION_DERIVE;
                pDestTypeSpecifier->derive[i].u.function_d.parameter =
                    CopyParameterList(pDerive->u.function_d.parameter_list, &iParameterCount);
                pDestTypeSpecifier->derive[i].u.function_d.parameter_count = iParameterCount;
            }
            break;

        default :
            GENERATE_DBG_Assert(0, ("derive->tag..", pDerive->tag));
        }
    }

    return pDestTypeSpecifier;
}

int Generate::GetOpcodeTypeOffset(DVM_BasicType enType)
{
    switch (enType)
    {
    case DVM_BOOLEAN_TYPE :
        return 0;

    case DVM_INT_TYPE :
        return 0;

    case DVM_DOUBLE_TYPE :
        return 1;

    case DVM_STRING_TYPE :
        return 2;

    default :
        GENERATE_DBG_Assert(0, ("basic_type..", enType));
    }

    return 0;
}

int Generate::GetLabel(OpcodeBuf *pOpcode)
{
    if (pOpcode->m_iLabelTableAllocSize < pOpcode->m_iLabelTableSize + 1)
    {
        pOpcode->m_pLabelTable = (LabelTable*)GENERATE_MEM_Realloc(pOpcode->m_pLabelTable,
            (pOpcode->m_iLabelTableAllocSize + LABEL_TABLE_ALLOC_SIZE) * sizeof(LabelTable));
        pOpcode->m_iLabelTableAllocSize += LABEL_TABLE_ALLOC_SIZE;
    }

    int ret = pOpcode->m_iLabelTableSize++;
    return ret;
}

void Generate::SetLabel(OpcodeBuf *pOpcode, int ilabel)
{
    pOpcode->m_pLabelTable[ilabel].m_iLabelAddress = pOpcode->m_iSize;
}