#include "StdAfx.h"
#include "Dispose.h"
#include "Debug.h"
#include "Memory.h"

#ifdef DBG_assert
#undef DBG_assert
#endif
#define DBG_assert(expression, arg) ((expression) ? (void)(0) : (m_Debug.Assert(__FILE__, __LINE__, #expression, arg)))

#ifdef MEM_free
#undef MEM_free
#endif
#define MEM_free(ptr)                       (m_Memory.Free(ptr))

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
            MEM_free(pExecutable->constant_pool[i].u.c_string);
    }

    MEM_free(pExecutable->constant_pool);

    for (int i = 0; i < pExecutable->global_variable_count; i++)
    {
        MEM_free(pExecutable->global_variable[i].name);
        DisposeTypeSpecifier(pExecutable->global_variable[i].type);
    }

    MEM_free(pExecutable->global_variable);

    for (int i = 0; i < pExecutable->function_count; i++)
    {
        DisposeTypeSpecifier(pExecutable->function[i].type);
        MEM_free(pExecutable->function[i].name);
        DisposeLocalVariable(pExecutable->function[i].parameter_count, pExecutable->function[i].parameter);

        if (DVM_TRUE == pExecutable->function[i].is_implemented)
        {
            DisposeLocalVariable(pExecutable->function[i].local_variable_count, pExecutable->function[i].local_variable);
            MEM_free(pExecutable->function[i].code);
            MEM_free(pExecutable->function[i].line_number);
        }
    }

    MEM_free(pExecutable->function);
    
    for (int i = 0; i < pExecutable->type_specifier_count; i++)
    {
        DisposeTypeDerive(&pExecutable->type_specifier[i]);
    }

    MEM_free(pExecutable->type_specifier);

    MEM_free(pExecutable->code);
    MEM_free(pExecutable->line_number);
    MEM_free(pExecutable);
}

void Dispose::DisposeTypeDerive(DVM_TypeSpecifier *pType)
{
    for (int i = 0; i < pType->derive_count; i++)
    {
        switch (pType->derive[i].tag)
        {
        case DVM_FUNCTION_DERIVE :
            DisposeLocalVariable(pType->derive[i].u.function_d.parameter_count, pType->derive[i].u.function_d.parameter);
            break;

        case DVM_ARRAY_DERIVE :
            break;

        default:
            DBG_assert(0, ("derive->tag..", pType->derive[i].tag));
        }
    }

    MEM_free(pType->derive);
}

void Dispose::DisposeTypeSpecifier(DVM_TypeSpecifier *pType)
{
    DisposeTypeDerive(pType);

    MEM_free(pType);
}

void Dispose::DisposeLocalVariable(int iCount, DVM_LocalVariable *pLocalVariable)
{
    for (int i = 0; i < iCount; i++)
    {
        MEM_free(pLocalVariable[i].name);
        DisposeTypeSpecifier(pLocalVariable[i].type);
    }

    MEM_free(pLocalVariable);
}