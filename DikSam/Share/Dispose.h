#pragma once

#include "DikSamc.h"

class Debug;
class Memory;

class Dispose
{
#define DISPOSE_MEM_Free(ptr)                  (m_Memory.Free(ptr))
#define DISPOSE_DBG_Assert(expression, arg)    ((expression) ? (void)(0) : (m_Debug.Assert(__FILE__, __LINE__, #expression, arg)))

public:
    Dispose(Debug& debug, Memory& memory);
    ~Dispose();

    void operator () (DVM_Executable *pExecutable);

private:
    void DisposeTypeDerive(DVM_TypeSpecifier *pType);
    void DisposeTypeSpecifier(DVM_TypeSpecifier *pType);
    void DisposeLocalVariable(int iCount, DVM_LocalVariable *pLocalVariable);

private:
    Debug   &m_Debug;
    Memory  &m_Memory;
};