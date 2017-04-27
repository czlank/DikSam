#pragma once

#include "DikSamc.h"

class Debug;
class Memory;
class Error;

class Execute
{
#define EXECUTE_MEM_Malloc(size)               (m_Memory.Malloc(__FILE__, __LINE__, size))
#define EXECUTE_MEM_Realloc(ptr, size)         (m_Memory.Realloc(__FILE__, __LINE__, ptr, size))
#define EXECUTE_MEM_Free(ptr)                  (m_Memory.Free(ptr))
#define EXECUTE_DBG_Assert(expression, arg)    ((expression) ? (void)(0) : (m_Debug.Assert(__FILE__, __LINE__, #expression, arg)))

public:
    Execute(Debug& debug, Memory& memory, Error& error);
    ~Execute();

    DVM_Value operator () (DVM_Executable* pExecutable);    // --> DVM_execute

private:
    void AddExecutable(DVM_VirtualMachine *pVirtualMachine, DVM_Executable *pExecutable);   // --> DVM_add_executable

    void CreateVirtualMachine();
    void DisposeVirtualMachine();

private:
    Debug   &m_Debug;
    Memory  &m_Memory;
    Error   &m_Error;

    DVM_Executable      *m_pExecutable;
    DVM_VirtualMachine  *m_pVirtualMachine;
};