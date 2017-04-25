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