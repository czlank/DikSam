#pragma once

#include "DikSamc.h"
#include "Native.h"
#include "GarbageCollect.h"

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

#define STI(sp)     ((m_pVirtualMachine)->stack.stack[(m_pVirtualMachine)->stack.stack_pointer + (sp)].int_value)
#define STD(sp)     ((m_pVirtualMachine)->stack.stack[(m_pVirtualMachine)->stack.stack_pointer + (sp)].double_value)
#define STO(sp)     ((m_pVirtualMachine)->stack.stack[(m_pVirtualMachine)->stack.stack_pointer + (sp)].object)

#define STI_I(sp)   ((m_pVirtualMachine)->stack.stack[(sp)].int_value)
#define STD_I(sp)   ((m_pVirtualMachine)->stack.stack[(sp)].double_value)
#define STO_I(sp)   ((m_pVirtualMachine)->stack.stack[(sp)].object)

#define STI_WRITE(sp, r) \
    ((m_pVirtualMachine)->stack.stack[(m_pVirtualMachine)->stack.stack_pointer + (sp)].int_value = r, \
    (m_pVirtualMachine)->stack.pointer_flags[(m_pVirtualMachine)->stack.stack_pointer + (sp)] = DVM_FALSE)
#define STD_WRITE(sp, r) \
    ((m_pVirtualMachine)->stack.stack[(m_pVirtualMachine)->stack.stack_pointer + (sp)].double_value = r, \
    (m_pVirtualMachine)->stack.pointer_flags[(m_pVirtualMachine)->stack.stack_pointer + (sp)] = DVM_FALSE)
#define STO_WRITE(sp, r) \
    ((m_pVirtualMachine)->stack.stack[(m_pVirtualMachine)->stack.stack_pointer + (sp)].object = r, \
    (m_pVirtualMachine)->stack.pointer_flags[(m_pVirtualMachine)->stack.stack_pointer + (sp)] = DVM_TRUE)

#define STI_WRITE_I(sp, r) \
    ((m_pVirtualMachine)->stack.stack[(sp)].int_value = r, \
    (m_pVirtualMachine)->stack.pointer_flags[(sp)] = DVM_FALSE)
#define STD_WRITE_I(sp, r) \
    ((m_pVirtualMachine)->stack.stack[(sp)].double_value = r, \
    (m_pVirtualMachine)->stack.pointer_flags[(sp)] = DVM_FALSE)
#define STO_WRITE_I(sp, r) \
    ((m_pVirtualMachine)->stack.stack[(sp)].object = r, \
    (m_pVirtualMachine)->stack.pointer_flags[(sp)] = DVM_TRUE)

public:
    Execute(Debug& debug, Memory& memory, Error& error);
    ~Execute();

    DVM_Value operator () (DVM_Executable* pExecutable);

private:
    void AddExecutable(DVM_Executable *pExecutable);
    void AddFunctions(DVM_Executable *pExecutable);
    int SearchFunction(char *lpstrName);
    void ConvertCode(DVM_Executable *pExecutable, DVM_Byte *pCode, int iCodeSize, DVM_Function *pFunction);
    void InitializeValue(DVM_BasicType enType, DVM_Value *pValue);
    void AddStaticVariables(DVM_Executable *pExecutable);
    DVM_Object* ChainString(DVM_Object *str1, DVM_Object *str2);
    void InitializeLocalVariables(DVM_Function *pFunction, int iFromSP);
    void ExpandStack(int iNeedStackSize);
    void InvokeNativeFunction(Function *pFunction, int *pSP);

    DVM_Value ExecuteCode(Function *pFunction, DVM_Byte *pCode, int iCodeSize);

    void CreateVirtualMachine();
    void DisposeVirtualMachine();

private:
    Debug   &m_Debug;
    Memory  &m_Memory;
    Error   &m_Error;

    Native              m_Native;
    GarbageCollect      m_GarbageCollect;

    DVM_VirtualMachine  *m_pVirtualMachine;
};