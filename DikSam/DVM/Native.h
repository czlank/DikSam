#pragma once

#include "DikSamc.h"
#include "DVM_dev.h"

class Debug;
class Memory;

class Native
{
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