#pragma once

#include "DikSamc.h"
#include "DVM_dev.h"

class Debug;
class Memory;

class Native
{
#define NATIVE_MEM_Realloc(ptr, size)       (m_Memory.Realloc(__FILE__, __LINE__, ptr, size))
#define NATIVE_MEM_StrDUP(str)              (m_Memory.StrDUP(__FILE__, __LINE__, str))
#define NATIVE_DBG_Assert(expression, arg)  ((expression) ? (void)(0) : (m_Debug.Assert(__FILE__, __LINE__, #expression, arg)))

public:
    Native(Debug& debug, Memory& memory);
    ~Native();

    void AddNativeFunctions(DVM_VirtualMachine *pVirtualMachine);

private:
    void AddNativeFunction(DVM_VirtualMachine *pVirtualMachine, const char *lpcstrFuncName, DVM_NativeFunctionProc *pProc, int iArgCount);

    static DVM_Value PrintProc(Native *pThis, DVM_VirtualMachine *pVirtualMachine, int iArgCount, DVM_Value *pArgs);
    static DVM_Value SleepProc(Native *pThis, DVM_VirtualMachine *pVirtualMachine, int iArgCount, DVM_Value *pArgs);

    DVM_Value Print(DVM_VirtualMachine *pVirtualMachine, int iArgCount, DVM_Value *pArgs);
    DVM_Value Sleep(DVM_VirtualMachine *pVirtualMachine, int iArgCount, DVM_Value *pArgs);

private:
    Debug   &m_Debug;
    Memory  &m_Memory;
};