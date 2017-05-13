#pragma once

#include "DikSamc.h"
#include "DVM_dev.h"

class Memory;

class Native
{
#define NATIVE_MEM_Realloc(ptr, size)       (m_Memory.Realloc(__FILE__, __LINE__, ptr, size))
#define NATIVE_MEM_StrDUP(str)              (m_Memory.StrDUP(__FILE__, __LINE__, str))

public:
    Native(Memory& memory);
    ~Native();

    void AddNativeFunctions(DVM_VirtualMachine *pVirtualMachine);

private:
    void AddNativeFunction(DVM_VirtualMachine *pVirtualMachine, const char *lpcstrFuncName, DVM_NativeFunctionProc *pProc, int iArgCount);

    static DVM_Value PrintProc(DVM_VirtualMachine *pVirtualMachine, int iArgCount, DVM_Value *pArgs);
    static DVM_Value SleepProc(DVM_VirtualMachine *pVirtualMachine, int iArgCount, DVM_Value *pArgs);

private:
    Memory  &m_Memory;
};