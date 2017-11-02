#include "StdAfx.h"
#include "Generate.h"
#include "Debug.h"
#include "Memory.h"
#include "Util.h"
#include "Error.h"
#include "OpcodeInfo.h"

#ifdef DBG_assert
#undef DBG_assert
#endif
#define DBG_assert(expression, arg) ((expression) ? (void)(0) : (m_Debug.Assert(__FILE__, __LINE__, #expression, arg)))

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

#define OPCODE_ALLOC_SIZE       (256)
#define LABEL_TABLE_ALLOC_SIZE  (256)

Generate::Generate(Debug& debug, Memory& memory, Util& util, Error& error)
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
    DVM_Executable *pExecutable = AllocExecutable();

    AddGlobalVariable(pCompiler, pExecutable);
    AddFunctions(pCompiler, pExecutable);
    AddTopLevel(pCompiler, pExecutable);

    return pExecutable;
}

int Generate::AddConstantPool(DVM_Executable *pExecutable, DVM_ConstantPool *pConstantPool)
{
    pExecutable->constant_pool = (DVM_ConstantPool*)MEM_realloc(pExecutable->constant_pool, sizeof(DVM_ConstantPool) * (pExecutable->constant_pool_count + 1));
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
    pExecutable->global_variable = (DVM_Variable*)MEM_malloc(sizeof(DVM_Variable) * iVariableCount);

    int i = 0;
    for (DeclarationList *pDeclarationList = pCompiler->declaration_list; pDeclarationList; pDeclarationList = pDeclarationList->next, i++)
    {
        pExecutable->global_variable[i].name = MEM_strdup(pDeclarationList->declaration->name);
        pExecutable->global_variable[i].type = CopyTypeSpecifier(pDeclarationList->declaration->type);
    }
}

int Generate::AddTypeSpecifier(TypeSpecifier *pTypeSpecifier, DVM_Executable *pExecutable)
{
    pExecutable->type_specifier = (DVM_TypeSpecifier*)MEM_realloc(pExecutable->type_specifier, sizeof(DVM_TypeSpecifier) * (pExecutable->type_specifier_count + 1));
    CopyTypeSpecifierNoAlloc(pTypeSpecifier, &pExecutable->type_specifier[pExecutable->type_specifier_count]);

    return pExecutable->type_specifier_count++;
}

void Generate::AddLineNumber(OpcodeBuf *pOpcode, int iLine, int iStartPC)
{
    if (nullptr == pOpcode->m_pLineNumber
        || pOpcode->m_pLineNumber[pOpcode->m_iLineNumberSize - 1].line_number != iLine)
    {
        pOpcode->m_pLineNumber = (DVM_LineNumber*)MEM_realloc(pOpcode->m_pLineNumber, sizeof(DVM_LineNumber) * (pOpcode->m_iLineNumberSize + 1));
        pOpcode->m_pLineNumber[pOpcode->m_iLineNumberSize].line_number = iLine;
        pOpcode->m_pLineNumber[pOpcode->m_iLineNumberSize].start_pc = iStartPC;
        pOpcode->m_pLineNumber[pOpcode->m_iLineNumberSize].pc_count = pOpcode->m_iSize - iStartPC;
        pOpcode->m_iLineNumberSize++;
    }
    else
    {
        pOpcode->m_pLineNumber[pOpcode->m_iLineNumberSize - 1].pc_count += pOpcode->m_iSize - iStartPC;
    }
}

void Generate::AddFunctions(DKC_Compiler *pCompiler, DVM_Executable *pExecutable)
{
    FunctionDefinition *pFunctionDefinition = pCompiler->function_list;
    int iCount = 0;

    for (; pFunctionDefinition; pFunctionDefinition = pFunctionDefinition->next)
    {
        iCount++;
    }

    pExecutable->function_count = iCount;
    pExecutable->function = (DVM_Function*)MEM_malloc(sizeof(DVM_Function) * iCount);

    pFunctionDefinition = pCompiler->function_list;
    for (int i = 0; pFunctionDefinition; pFunctionDefinition = pFunctionDefinition->next, i++)
    {
        CopyFunction(pFunctionDefinition, &pExecutable->function[i]);

        if (pFunctionDefinition->block)
        {
            OpcodeBuf opcodeBuf;
            DVM_Function *pFunction = pExecutable->function;

            InitOpcodeBuf(&opcodeBuf);
            GenerateStatementList(pExecutable, pFunctionDefinition->block, pFunctionDefinition->block->statement_list, &opcodeBuf);

            pFunction[i].is_implemented = DVM_TRUE;
            pFunction[i].code_size = opcodeBuf.m_iSize;
            pFunction[i].code = FixOpcodeBuf(&opcodeBuf);
            pFunction[i].line_number_size = opcodeBuf.m_iLineNumberSize;
            pFunction[i].line_number = opcodeBuf.m_pLineNumber;
            pFunction[i].need_stack_size = CalcNeedStackSize(pExecutable->function[i].code, pExecutable->function[i].code_size);
        }
        else
        {
            pExecutable->function[i].is_implemented = DVM_FALSE;
        }
    }
}

void Generate::AddTopLevel(DKC_Compiler *pCompiler, DVM_Executable *pExecutable)
{
    OpcodeBuf opcodeBuf;

    InitOpcodeBuf(&opcodeBuf);
    GenerateStatementList(pExecutable, nullptr, pCompiler->statement_list, &opcodeBuf);

    pExecutable->code_size = opcodeBuf.m_iSize;
    pExecutable->code = FixOpcodeBuf(&opcodeBuf);
    pExecutable->line_number_size = opcodeBuf.m_iLineNumberSize;
    pExecutable->line_number = opcodeBuf.m_pLineNumber;
    pExecutable->need_stack_size = CalcNeedStackSize(pExecutable->code, pExecutable->code_size);
}

void Generate::GenerateCode(OpcodeBuf *pOpcode, int iLine, DVM_Opcode code, ...)
{
    va_list ap;

    va_start(ap, code);

    const char *param = DVMOpcodeInfo::Opcode()[int(code)].parameter;
    int paramLen = param ? std::string(param).length() : 0;

    if (pOpcode->m_iAllocSize < pOpcode->m_iSize + 1 + (paramLen * 2))
    {
        pOpcode->m_pCode = (DVM_Byte*)MEM_realloc(pOpcode->m_pCode, pOpcode->m_iAllocSize + OPCODE_ALLOC_SIZE);
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
            DBG_assert(0, ("param..", param, ", i..", i));
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
    if (0.0 == pExpression->u.double_value)
    {
        GenerateCode(pOpcode, pExpression->line_number, DVM_PUSH_DOUBLE_0);
    }
    else if (1.0 == pExpression->u.double_value)
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
    if (DVM_TRUE == pExpression->u.identifier.is_function)
    {
        GenerateCode(pOpcode, pExpression->line_number, DVM_PUSH_FUNCTION, pExpression->u.identifier.u.function->index);
        return;
    }

    DVM_Opcode code = pExpression->u.identifier.u.declaration->is_local ? DVM_PUSH_STACK_INT : DVM_PUSH_STATIC_INT;

    GenerateCode(pOpcode, pExpression->line_number,
        DVM_Opcode(code + GetOpcodeTypeOffset(pExpression->u.identifier.u.declaration->type)),
        pExpression->u.identifier.u.declaration->variable_index);
}

void Generate::GeneratePopToIdentifier(Declaration *pDeclaration, int iLine, OpcodeBuf *pOpcode)
{
    DVM_Opcode code = DVM_TRUE == pDeclaration->is_local ? DVM_POP_STACK_INT : DVM_POP_STATIC_INT;

    GenerateCode(pOpcode, iLine,
        DVM_Opcode(code + GetOpcodeTypeOffset(pDeclaration->type)),
        pDeclaration->variable_index);
}

void Generate::GeneratePopToLValue(DVM_Executable *pExecutable, Block *pBlock, Expression *pExpression, OpcodeBuf *pOpcode)
{
    if (IDENTIFIER_EXPRESSION == pExpression->kind)
    {
        GeneratePopToIdentifier(pExpression->u.identifier.u.declaration, pExpression->line_number, pOpcode);
    }
    else
    {
        DBG_assert(INDEX_EXPRESSION == pExpression->kind, ("pExpression->kind..", pExpression->kind));

        GenerateExpression(pExecutable, pBlock, pExpression->u.index_expression.array, pOpcode);
        GenerateExpression(pExecutable, pBlock, pExpression->u.index_expression.index, pOpcode);
        GenerateCode(pOpcode, pExpression->line_number, DVM_Opcode(DVM_POP_ARRAY_INT + GetOpcodeTypeOffset(pExpression->type)));
    }
}

void Generate::GenerateAssignExpression(DVM_Executable *pExecutable, Block *pBlock, Expression *pExpression, OpcodeBuf *pOpcode, bool isTopLevel)
{
    if (pExpression->u.assign_expression.op != NORMAL_ASSIGN)
    {
        GenerateExpression(pExecutable, pBlock, pExpression->u.assign_expression.left, pOpcode);
    }

    GenerateExpression(pExecutable, pBlock, pExpression->u.assign_expression.operand, pOpcode);

    switch (pExpression->u.assign_expression.op)
    {
    case NORMAL_ASSIGN :
        break;

    case ADD_ASSIGN :
        GenerateCode(pOpcode, pExpression->line_number, DVM_Opcode(DVM_ADD_INT + GetOpcodeTypeOffset(pExpression->type)));
        break;

    case SUB_ASSIGN :
        GenerateCode(pOpcode, pExpression->line_number, DVM_Opcode(DVM_SUB_INT + GetOpcodeTypeOffset(pExpression->type)));
        break;

    case MUL_ASSIGN :
        GenerateCode(pOpcode, pExpression->line_number, DVM_Opcode(DVM_MUL_INT + GetOpcodeTypeOffset(pExpression->type)));
        break;

    case DIV_ASSIGN :
        GenerateCode(pOpcode, pExpression->line_number, DVM_Opcode(DVM_DIV_INT + GetOpcodeTypeOffset(pExpression->type)));
        break;

    case MOD_ASSIGN :
        GenerateCode(pOpcode, pExpression->line_number, DVM_Opcode(DVM_MOD_INT + GetOpcodeTypeOffset(pExpression->type)));
        break;

    default :
        DBG_assert(0, ("operator..", pExpression->u.assign_expression.op));
    }

    if (!isTopLevel)
    {
        GenerateCode(pOpcode, pExpression->line_number, DVM_DUPLICATE);
    }

    GeneratePopToLValue(pExecutable, pBlock, pExpression->u.assign_expression.left, pOpcode);
}

void Generate::GenerateBinaryExpression(DVM_Executable *pExecutable, Block *pBlock, Expression *pExpression, DVM_Opcode opCode, OpcodeBuf *pOpcode)
{
    Expression *pLeftExpression = pExpression->u.binary_expression.left;
    Expression *pRightExpression = pExpression->u.binary_expression.right;

    GenerateExpression(pExecutable, pBlock, pLeftExpression, pOpcode);
    GenerateExpression(pExecutable, pBlock, pRightExpression, pOpcode);

    int offset;

    if ((NULL_EXPRESSION == pLeftExpression->kind && pRightExpression->kind != NULL_EXPRESSION)
        || (pLeftExpression->kind != NULL_EXPRESSION && NULL_EXPRESSION == pRightExpression->kind))
    {
        // object type
        offset = 2;
    }
    else if ((DVM_EQ_INT == opCode || DVM_NE_INT == opCode) && IsString(pLeftExpression->type))
    {
        // string type
        offset = 3;
    }
    else
    {
        offset = GetOpcodeTypeOffset(pExpression->u.binary_expression.left->type);
    }

    GenerateCode(pOpcode, pExpression->line_number, DVM_Opcode(opCode + offset));
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
        DBG_assert(0, ("pExpression->u.cast.type..", pExpression->u.cast.type));
    }
}

void Generate::GenerateArrayLiteralExpression(DVM_Executable *pExecutable, Block *pBlock, Expression *pExpression, OpcodeBuf *pOpcode)
{
    DBG_assert(pExpression->type->derive && ARRAY_DERIVE == pExpression->type->derive->tag, ("array literal is not array."));

    int count = 0;
    for (ExpressionList *pos = pExpression->u.array_literal; pos; pos = pos->next)
    {
        GenerateExpression(pExecutable, pBlock, pos->expression, pOpcode);
        count++;
    }

    GenerateCode(pOpcode, pExpression->line_number, DVM_Opcode(DVM_NEW_ARRAY_LITERAL_INT + GetOpcodeTypeOffset(pExpression->u.array_literal->expression->type)), count);
}

void Generate::GenerateIndexExpression(DVM_Executable *pExecutable, Block *pBlock, Expression *pExpression, OpcodeBuf *pOpcode)
{
    GenerateExpression(pExecutable, pBlock, pExpression->u.index_expression.array, pOpcode);
    GenerateExpression(pExecutable, pBlock, pExpression->u.index_expression.index, pOpcode);

    GenerateCode(pOpcode, pExpression->line_number, DVM_Opcode(DVM_PUSH_ARRAY_INT + GetOpcodeTypeOffset(pExpression->type)));
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
        DBG_assert(0, ("kind..", kind));
    }

    if (!isTopLevel)
    {
        GenerateCode(pOpcode, pExpression->line_number, DVM_DUPLICATE);
    }

    GeneratePopToLValue(pExecutable, pBlock, pExpression->u.inc_dec.operand, pOpcode);
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

void Generate::GenerateNullExpression(DVM_Executable *pExecutable, Expression *pExpression, OpcodeBuf *pOpcode)
{
    GenerateCode(pOpcode, pExpression->line_number, DVM_PUSH_NULL);
}

void Generate::GenerateArrayCreationExpression(DVM_Executable *pExecutable, Block *pBlock, Expression *pExpression, OpcodeBuf *pOpcode)
{
    int index = AddTypeSpecifier(pExpression->type, pExecutable);

    DBG_assert(ARRAY_DERIVE == pExpression->type->derive->tag, ("pExpression->type->derive->tag", pExpression->type->derive->tag));

    int iDimCount = 0;
    for (ArrayDimension *pos = pExpression->u.array_creation.dimension; pos; pos = pos->next)
    {
        if (nullptr == pos->expression)
            break;

        GenerateExpression(pExecutable, pBlock, pos->expression, pOpcode);
        iDimCount++;
    }

    GenerateCode(pOpcode, pExpression->line_number, DVM_NEW_ARRAY, iDimCount, index);
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
        GenerateStatement(pExecutable, pBlock, pExpression->u.comma.right, pOpcode);
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
        GenerateCode(pOpcode, pExpression->line_number, DVM_Opcode(DVM_MINUS_INT + GetOpcodeTypeOffset(pExpression->type)));
        break;

    case LOGICAL_NOT_EXPRESSION :
        GenerateExpression(pExecutable, pBlock, pExpression->u.logical_not, pOpcode);
        GenerateCode(pOpcode, pExpression->line_number, DVM_LOGICAL_NOT);
        break;

    case FUNCTION_CALL_EXPRESSION :
        GenerateFunctionCallExpression(pExecutable, pBlock, pExpression, pOpcode);
        break;

    case MEMBER_EXPRESSION :
        break;

    case NULL_EXPRESSION :
        GenerateNullExpression(pExecutable, pExpression, pOpcode);
        break;

    case ARRAY_LITERAL_EXPRESSION :
        GenerateArrayLiteralExpression(pExecutable, pBlock, pExpression, pOpcode);
        break;

    case INDEX_EXPRESSION :
        GenerateIndexExpression(pExecutable, pBlock, pExpression, pOpcode);
        break;

    case INCREMENT_EXPRESSION :
    case DECREMENT_EXPRESSION :
        GenerateIncDecExpression(pExecutable, pBlock, pExpression, pExpression->kind, pOpcode, false);
        break;

    case CAST_EXPRESSION :
        GenerateCastExpression(pExecutable, pBlock, pExpression, pOpcode);
        break;

    case ARRAY_CREATION_EXPRESSION :
        GenerateArrayCreationExpression(pExecutable, pBlock, pExpression, pOpcode);
        break;

    default :
        DBG_assert(0, ("pExpression->kind..", pExpression->kind));
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
    DBG_assert(pStatement->u.return_s.return_value, ("return value is null."));

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

void Generate::GenerateStatement(DVM_Executable *pExecutable, Block *pBlock, Statement *pStatement, OpcodeBuf *pOpcode)
{
    switch (pStatement->type)
    {
    case EXPRESSION_STATEMENT :
        GenerateExpressionStatement(pExecutable, pBlock, pStatement->u.expression_s, pOpcode);
        break;

    case IF_STATEMENT :
        GenerateIfStatement(pExecutable, pBlock, pStatement, pOpcode);
        break;

    case WHILE_STATEMENT :
        GenerateWhileStatement(pExecutable, pBlock, pStatement, pOpcode);
        break;

    case FOR_STATEMENT :
        GenerateForStatement(pExecutable, pBlock, pStatement, pOpcode);
        break;

    case RETURN_STATEMENT :
        GenerateReturnStatement(pExecutable, pBlock, pStatement, pOpcode);
        break;

    case BREAK_STATEMENT :
        GenerateBreakStatement(pExecutable, pBlock, pStatement, pOpcode);
        break;

    case CONTINUE_STATEMENT :
        GenerateContinueStatement(pExecutable, pBlock, pStatement, pOpcode);
        break;

    case TRY_STATEMENT :
        break;

    case THROW_STATEMENT :
        break;

    case DECLARATION_STATEMENT :
        GenerateInitializer(pExecutable, pBlock, pStatement, pOpcode);
        break;

    default :
        DBG_assert(0, ("pos->statement->type..", pStatement->type));
    }
}

void Generate::GenerateStatementList(DVM_Executable *pExecutable, Block *pBlock, StatementList *pStatementList, OpcodeBuf *pOpcode)
{
    for (StatementList *pos = pStatementList; pos; pos = pos->next)
    {
        GenerateStatement(pExecutable, pBlock, pos->statement, pOpcode);
    }
}

DVM_Executable* Generate::AllocExecutable(PackageName *pPackageName)
{
    DVM_Executable *pExecutable = (DVM_Executable*)MEM_malloc(sizeof(DVM_Executable));

    pExecutable->package_name = m_Util.PackageNameToString(pPackageName);
    pExecutable->is_required = DVM_FALSE;
    pExecutable->constant_pool_count = 0;
    pExecutable->constant_pool = nullptr;
    pExecutable->global_variable_count = 0;
    pExecutable->global_variable = nullptr;
    pExecutable->function_count = 0;
    pExecutable->function = nullptr;
    pExecutable->type_specifier_count = 0;
    pExecutable->type_specifier = nullptr;
    pExecutable->code_size = 0;
    pExecutable->code = nullptr;

    return pExecutable;
}

DVM_LocalVariable* Generate::CopyParameterList(ParameterList *pParameterList, int *pParameterCount)
{
    int iParamCount = CountParameter(pParameterList);

    *pParameterCount = iParamCount;

    DVM_LocalVariable *pDest = (DVM_LocalVariable*)MEM_malloc(sizeof(DVM_LocalVariable) * iParamCount);

    int i = 0;
    for (ParameterList *pParam = pParameterList; pParam; pParam = pParam->next, i++)
    {
        pDest[i].name = MEM_strdup(pParam->name);
        pDest[i].type = CopyTypeSpecifier(pParam->type);
    }

    return pDest;
}

DVM_LocalVariable* Generate::CopyLocalVariables(FunctionDefinition *pFunctionDefinition, int iParameterCount)
{
    int iLocalVariableCount = pFunctionDefinition->local_variable_count - iParameterCount;
    DVM_LocalVariable *pDest = (DVM_LocalVariable*)MEM_malloc(sizeof(DVM_LocalVariable) * iLocalVariableCount);

    for (int i = 0; i < iLocalVariableCount; i++)
    {
        pDest[i].name = MEM_strdup(pFunctionDefinition->local_variable[iParameterCount + i]->name);
        pDest[i].type = CopyTypeSpecifier(pFunctionDefinition->local_variable[iParameterCount + i]->type);
    }

    return pDest;
}

void Generate::CopyTypeSpecifierNoAlloc(TypeSpecifier *pSrc, DVM_TypeSpecifier *pDest)
{
    pDest->basic_type = pSrc->basic_type;

    if (DVM_CLASS_TYPE == pSrc->basic_type)
    {
        pDest->class_index = pSrc->class_ref.class_index;
    }
    else
    {
        pDest->class_index = -1;
    }

    int iDeriveCount = 0;
    for (TypeDerive *pDerive = pSrc->derive; pDerive; pDerive = pDerive->next)
    {
        iDeriveCount++;
    }

    pDest->derive_count = iDeriveCount;
    pDest->derive = (DVM_TypeDerive*)MEM_malloc(sizeof(DVM_TypeDerive) * iDeriveCount);

    int i = 0;
    for (TypeDerive *pDerive = pSrc->derive; pDerive; pDerive = pDerive->next, i++)
    {
        switch (pDerive->tag)
        {
        case FUNCTION_DERIVE :
            {
                int iParameterCount = 0;

                pDest->derive[i].tag = DVM_FUNCTION_DERIVE;
                pDest->derive[i].u.function_d.parameter = CopyParameterList(pDerive->u.function_d.parameter_list, &iParameterCount);
                pDest->derive[i].u.function_d.parameter_count = iParameterCount;
            }
            break;

        case ARRAY_DERIVE :
            pDest->derive[i].tag = DVM_ARRAY_DERIVE;
            break;

        default :
            DBG_assert(0, ("pDerive->tag..", pDerive->tag));
        }
    }
}

DVM_TypeSpecifier* Generate::CopyTypeSpecifier(TypeSpecifier *pTypeSpecifier)
{
    DVM_TypeSpecifier *pDest = (DVM_TypeSpecifier*)MEM_malloc(sizeof(DVM_TypeSpecifier));
    
    CopyTypeSpecifierNoAlloc(pTypeSpecifier, pDest);
    
    return pDest;
}

int Generate::GetOpcodeTypeOffset(TypeSpecifier *pTypeSpecifier)
{
    if (pTypeSpecifier->derive)
    {
        DBG_assert(pTypeSpecifier->derive->tag == ARRAY_DERIVE, ("pTypeSpecifier->derive->tag..", pTypeSpecifier->derive->tag));
        return 2;
    }

    switch (pTypeSpecifier->basic_type)
    {
    case DVM_BOOLEAN_TYPE :
        return 0;

    case DVM_INT_TYPE :
        return 0;

    case DVM_DOUBLE_TYPE :
        return 1;

    case DVM_STRING_TYPE :
        return 2;

    case DVM_NULL_TYPE :
    default :
        DBG_assert(0, ("basic_type..", pTypeSpecifier->basic_type));
    }

    return 0;
}

int Generate::GetLabel(OpcodeBuf *pOpcode)
{
    if (pOpcode->m_iLabelTableAllocSize < pOpcode->m_iLabelTableSize + 1)
    {
        pOpcode->m_pLabelTable = (LabelTable*)MEM_realloc(pOpcode->m_pLabelTable,
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

void Generate::InitOpcodeBuf(OpcodeBuf *pOpcode)
{
    pOpcode->m_iSize = 0;
    pOpcode->m_iAllocSize = 0;
    pOpcode->m_pCode = nullptr;
    pOpcode->m_iLabelTableSize = 0;
    pOpcode->m_iLabelTableAllocSize = 0;
    pOpcode->m_pLabelTable = nullptr;
    pOpcode->m_iLineNumberSize = 0;
    pOpcode->m_pLineNumber = nullptr;
}

void Generate::FixLabels(OpcodeBuf *pOpcode)
{
    for (int i = 0; i < pOpcode->m_iSize; i++)
    {
        if (DVM_JUMP == pOpcode->m_pCode[i]
            || DVM_JUMP_IF_TRUE == pOpcode->m_pCode[i]
            || DVM_JUMP_IF_FALSE == pOpcode->m_pCode[i])
        {
            int label = (pOpcode->m_pCode[i + 1] << 8) | (pOpcode->m_pCode[i + 2]);
            int address = pOpcode->m_pLabelTable[label].m_iLabelAddress;
            
            pOpcode->m_pCode[i + 1] = DVM_Byte(address >> 8);
            pOpcode->m_pCode[i + 2] = DVM_Byte(address & 0xFF);
        }

        OpcodeInfo &Info = DVMOpcodeInfo::Opcode()[pOpcode->m_pCode[i]];

        for (int idx = 0; Info.parameter[idx] != '\0'; idx++)
        {
            switch (Info.parameter[idx])
            {
            case 'b' :
                i++;
                break;

            case 's' :
            case 'p' :
                i += 2;
                break;

            default :
                DBG_assert(0, ("param..", Info.parameter, ", idx..", idx));
            }
        }
    }
}

DVM_Byte* Generate::FixOpcodeBuf(OpcodeBuf *pOpcode)
{
    FixLabels(pOpcode);

    DVM_Byte *ret = (DVM_Byte*)MEM_realloc(pOpcode->m_pCode, pOpcode->m_iSize);
    MEM_free(pOpcode->m_pLabelTable);

    return ret;
}

int Generate::CalcNeedStackSize(DVM_Byte *pCode, int iCodeSize)
{
    int iStackSize = 0;

    for (int i = 0; i < iCodeSize; i++)
    {
        OpcodeInfo &Info = DVMOpcodeInfo::Opcode()[pCode[i]];

        if (Info.stack_increment > 0)
        {
            iStackSize += Info.stack_increment;
        }

        for (int idx = 0; Info.parameter[idx] != '\0'; idx++)
        {
            switch (Info.parameter[idx])
            {
            case 'b' :
                i++;
                break;

            case 's' :
            case 'p' :
                i += 2;
                break;

            default :
                DBG_assert(0, ("param..", Info.parameter, ", idx..", idx));
            }
        }
    }

    return iStackSize;
}

void Generate::CopyFunction(FunctionDefinition *pFunctionDefinition, DVM_Function *pFunction)
{
    pFunction->type = CopyTypeSpecifier(pFunctionDefinition->type);
    pFunction->name = MEM_strdup(pFunctionDefinition->name);
    pFunction->parameter = CopyParameterList(pFunctionDefinition->parameter, &pFunction->parameter_count);

    if (pFunctionDefinition->block)
    {
        pFunction->local_variable = CopyLocalVariables(pFunctionDefinition, pFunction->parameter_count);
        pFunction->local_variable_count = pFunctionDefinition->local_variable_count - pFunction->parameter_count;
    }
    else
    {
        pFunction->local_variable = nullptr;
        pFunction->local_variable_count = 0;
    }
}

int Generate::CountParameter(ParameterList *pSrc)
{
    int iParamCount = 0;

    for (ParameterList *pos = pSrc; pos; pos = pos->next)
    {
        iParamCount++;
    }

    return iParamCount;
}

void Generate::AddMethod(MemberDeclaration *pMember, DVM_Method *pDest)
{
    pDest->access_modifier = pMember->access_modifier;
    pDest->is_abstract = pMember->u.method.is_abstract;
    pDest->is_virtual = pMember->u.method.is_virtual;
    pDest->is_override = pMember->u.method.is_override;
    pDest->name = MEM_strdup(pMember->u.method.function_definition->name);
}

void Generate::AddField(MemberDeclaration *pMember, DVM_Field *pDest)
{
    pDest->access_modifier = pMember->access_modifier;
    pDest->name = MEM_strdup(pMember->u.field.name);
    pDest->type = CopyTypeSpecifier(pMember->u.field.type);
}

void Generate::SetClassIdentifier(ClassDefinition *pClassDefinition, DVM_ClassIdentifier *pClassIdentifier)
{
    pClassIdentifier = MEM_strdup(pClassDefinition->name);
    pClassIdentifier->package_name = m_Util.PackageNameToString(pClassDefinition->package_name);
}