#pragma once

#include "DikSamc.h"

class Debug;
class Memory;

class Dispose
{
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