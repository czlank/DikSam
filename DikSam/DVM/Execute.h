#pragma once

#include "DikSamc.h"

class Debug;
class Memory;
class Error;

class Execute
{
#define EXECUTE_MEM_Malloc(size)               (m_Memory.Malloc(__FILE__, __LINE__, size))
#define EXECUTE_MEM_Realloc(ptr, size)         (m_Memory.Realloc(__FILE__, __LINE__, ptr, size))
#define EXECUTE_MEM_StrDUP(str)                (m_Memory.StrDUP(__FILE__, __LINE__, str))
#define EXECUTE_MEM_Free(ptr)                  (m_Memory.Free(ptr))
#define EXECUTE_DBG_Assert(expression, arg)    ((expression) ? (void)(0) : (m_Debug.Assert(__FILE__, __LINE__, #expression, arg)))

#define GET_2BYTE_INT(p)            (((p)[0] << 8) + (p)[1])
#define SET_2BYTE_INT(p, value)     (((p)[0] = (value) >> 8), ((p)[1] = value & 0xff))

public:
    Execute(Debug& debug, Memory& memory, Error& error);
    ~Execute();

    DVM_Value operator () (DVM_Executable* pExecutable);

private:
    void AddExecutable(DVM_VirtualMachine *pVirtualMachine, DVM_Executable *pExecutable);
    void AddFunctions(DVM_VirtualMachine *pVirtualMachine, DVM_Executable *pExecutable);
    int SearchFunction(DVM_VirtualMachine *pVirtualMachine, char *lpstrName);
    void ConvertCode(DVM_VirtualMachine *pVirtualMachine, DVM_Executable *pExecutable, DVM_Byte *pCode, int iCodeSize, DVM_Function *pFunction);
    void InitializeValue(DVM_VirtualMachine *pVirtualMachine, DVM_BasicType enType, DVM_Value *pValue);
    void AddStaticVariables(DVM_VirtualMachine *pVirtualMachine, DVM_Executable *pExecutable);

    void CreateVirtualMachine();
    void DisposeVirtualMachine();

private:
    Debug   &m_Debug;
    Memory  &m_Memory;
    Error   &m_Error;

    DVM_Executable      *m_pExecutable;
    DVM_VirtualMachine  *m_pVirtualMachine;
};