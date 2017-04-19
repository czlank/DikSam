#pragma once

#include "DikSamc.h"

class Debug;
class Memory;
class Error;

class Generate
{
#define GENERATE_MEM_Malloc(size)               (m_Memory.Malloc(__FILE__, __LINE__, size))
#define GENERATE_MEM_Realloc(ptr, size)         (m_Memory.Realloc(__FILE__, __LINE__, ptr, size))
#define GENERATE_DBG_Assert(expression, arg)    ((expression) ? (void)(0) : (m_Debug.Assert(__FILE__, __LINE__, #expression, arg)))

public:
    Generate(Debug& debug, Memory& memory, Error& error);
    ~Generate();

    DVM_Executable* operator () (DKC_Compiler *pCompiler);

private:
    int AddConstantPool(DVM_Executable *pExecutable, DVM_ConstantPool *pConstantPool);

    DVM_Executable* AllocExecutable();
    DVM_LocalVariable* CopyParameterList(ParameterList *pParameterList, int *pParameterCount);
    DVM_LocalVariable* CopyLocalVariables(FunctionDefinition *pFunctionDefinition, int iParameterCount);
    DVM_TypeSpecifier* CopyTypeSpecifier(TypeSpecifier *pTypeSpecifier);

private:
    Debug   &m_Debug;
    Memory  &m_Memory;
    Error   &m_Error;
};