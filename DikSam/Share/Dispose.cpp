#include "StdAfx.h"
#include "Dispose.h"
#include "Debug.h"
#include "Memory.h"

Dispose::Dispose(Debug& debug, Memory& memory)
    : m_Debug(debug)
    , m_Memory(memory)
{

}

Dispose::~Dispose()
{

}

void Dispose::operator () (DVM_Executable *pExecutable)
{
    for (int i = 0; i < pExecutable->constant_pool_count; i++)
    {
        if (DVM_CONSTANT_STRING == pExecutable->constant_pool[i].tag)
            DISPOSE_MEM_Free(pExecutable->constant_pool[i].u.c_string);
    }

    DISPOSE_MEM_Free(pExecutable->constant_pool);

    for (int i = 0; i < pExecutable->global_variable_count; i++)
    {
        DISPOSE_MEM_Free(pExecutable->global_variable[i].name);
        DisposeTypeSpecifier(pExecutable->global_variable[i].type);
    }

    DISPOSE_MEM_Free(pExecutable->global_variable);

    for (int i = 0; i < pExecutable->function_count; i++)
    {
        DisposeTypeSpecifier(pExecutable->function[i].type);
        DISPOSE_MEM_Free(pExecutable->function[i].name);
        DisposeLocalVariable(pExecutable->function[i].parameter_count, pExecutable->function[i].parameter);

        if (DVM_TRUE == pExecutable->function[i].is_implemented)
        {
            DisposeLocalVariable(pExecutable->function[i].local_variable_count, pExecutable->function[i].local_variable);
            DISPOSE_MEM_Free(pExecutable->function[i].code);
            DISPOSE_MEM_Free(pExecutable->function[i].line_number);
        }
    }

    DISPOSE_MEM_Free(pExecutable->function);
    DISPOSE_MEM_Free(pExecutable->code);
    DISPOSE_MEM_Free(pExecutable->line_number);
    DISPOSE_MEM_Free(pExecutable);
}

void Dispose::DisposeTypeSpecifier(DVM_TypeSpecifier *pType)
{
    for (int i = 0; i < pType->derive_count; i++)
    {
        switch (pType->derive[i].tag)
        {
        case DVM_FUNCTION_DERIVE :
            DisposeLocalVariable(pType->derive[i].u.function_d.parameter_count,
                pType->derive[i].u.function_d.parameter);
            break;

        default:
            DISPOSE_DBG_Assert(0, ("derive->tag..", pType->derive[i].tag));
        }
    }

    DISPOSE_MEM_Free(pType->derive);
    DISPOSE_MEM_Free(pType);
}

void Dispose::DisposeLocalVariable(int iCount, DVM_LocalVariable *pLocalVariable)
{
    for (int i = 0; i < iCount; i++)
    {
        DISPOSE_MEM_Free(pLocalVariable[i].name);
        DisposeTypeSpecifier(pLocalVariable[i].type);
    }

    DISPOSE_MEM_Free(pLocalVariable);
}