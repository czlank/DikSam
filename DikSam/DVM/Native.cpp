#include "StdAfx.h"
#include "Native.h"
#include "DVM_pri.h"
#include "Memory.h"

Native::Native(Memory& memory)
    : m_Memory(memory)
{

}

Native::~Native()
{

}

void Native::AddNativeFunctions(DVM_VirtualMachine *pVirtualMachine)
{
    AddNativeFunction(pVirtualMachine, "print", &Native::PrintProc, 1);
    AddNativeFunction(pVirtualMachine, "sleep", &Native::SleepProc, 1);
}

void Native::AddNativeFunction(DVM_VirtualMachine *pVirtualMachine, const char *lpcstrFuncName, DVM_NativeFunctionProc *pProc, int iArgCount)
{
    pVirtualMachine->function = (Function*)NATIVE_MEM_Realloc(pVirtualMachine->function, sizeof(Function) * (pVirtualMachine->function_count + 1));
    pVirtualMachine->function[pVirtualMachine->function_count].name = NATIVE_MEM_StrDUP(lpcstrFuncName);
    pVirtualMachine->function[pVirtualMachine->function_count].kind = NATIVE_FUNCTION;
    pVirtualMachine->function[pVirtualMachine->function_count].u.native_f.proc = pProc;
    pVirtualMachine->function[pVirtualMachine->function_count].u.native_f.pThis = this;
    pVirtualMachine->function[pVirtualMachine->function_count].u.native_f.arg_count = iArgCount;
    pVirtualMachine->function_count++;
}

DVM_Value Native::PrintProc(Native *pThis, DVM_VirtualMachine *pVirtualMachine, int iArgCount, DVM_Value *pArgs)
{
    return pThis->Print(pVirtualMachine, iArgCount, pArgs);
}

DVM_Value Native::SleepProc(Native *pThis, DVM_VirtualMachine *pVirtualMachine, int iArgCount, DVM_Value *pArgs)
{
    return pThis->Sleep(pVirtualMachine, iArgCount, pArgs);
}

DVM_Value Native::Print(DVM_VirtualMachine *pVirtualMachine, int iArgCount, DVM_Value *pArgs)
{
    DVM_Value ret;

    ret.int_value = -1;

    if (iArgCount != 1)
    {
        return ret;
    }

    std::wcout << pArgs[0].object->u.string.string << std::endl;

    ret.int_value = 0;

    return ret;
}

DVM_Value Native::Sleep(DVM_VirtualMachine *pVirtualMachine, int iArgCount, DVM_Value *pArgs)
{
    DVM_Value ret;

    ret.int_value = -1;

    if (iArgCount != 1)
    {
        return ret;
    }

    ::Sleep(pArgs[0].int_value);

    ret.int_value = 0;

    return ret;
}