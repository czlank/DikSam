#include "StdAfx.h"
#include "Generate.h"
#include "Debug.h"
#include "Memory.h"
#include "Error.h"

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

void Generate::AddLineNumber(OpcodeBuf *pOpcodeBuf, int iLine, int iStartPC)
{
    if (nullptr == pOpcodeBuf->m_LineNumber
        || pOpcodeBuf->m_LineNumber[pOpcodeBuf->m_iLineNumberSize - 1].line_number != iLine)
    {
        pOpcodeBuf->m_LineNumber = (DVM_LineNumber*)GENERATE_MEM_Realloc(pOpcodeBuf->m_LineNumber, sizeof(DVM_LineNumber) * (pOpcodeBuf->m_iLineNumberSize + 1));
        pOpcodeBuf->m_LineNumber[pOpcodeBuf->m_iLineNumberSize].line_number = iLine;
        pOpcodeBuf->m_LineNumber[pOpcodeBuf->m_iLineNumberSize].start_pc = iStartPC;
        pOpcodeBuf->m_LineNumber[pOpcodeBuf->m_iLineNumberSize].pc_count = pOpcodeBuf->m_iSize - iStartPC;
        pOpcodeBuf->m_iLineNumberSize++;
    }
    else
    {
        pOpcodeBuf->m_LineNumber[pOpcodeBuf->m_iLineNumberSize - 1].pc_count += pOpcodeBuf->m_iSize - iStartPC;
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