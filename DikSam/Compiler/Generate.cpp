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
    return 0;
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
    return nullptr;
}

DVM_LocalVariable* Generate::CopyLocalVariables(FunctionDefinition *pFunctionDefinition, int iParameterCount)
{
    return nullptr;
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