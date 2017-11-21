#include "StdAfx.h"
#include "Native.h"
#include "DVM_pri.h"
#include "Debug.h"
#include "Memory.h"

#ifdef DBG_assert
#undef DBG_assert
#endif
#define DBG_assert(expression, arg) ((expression) ? (void)(0) : (m_Debug.Assert(__FILE__, __LINE__, #expression, arg)))

#ifdef MEM_realloc
#undef MEM_realloc
#endif
#define MEM_realloc(ptr, size)              (m_Memory.Realloc(__FILE__, __LINE__, ptr, size))

#ifdef MEM_strdup
#undef MEM_strdup
#endif
#define MEM_strdup(ptr)                     (m_Memory.StrDUP(__FILE__, __LINE__, ptr))

Native::Native(Debug& debug, Memory& memory)
    : m_Debug(debug), m_Memory(memory)
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
    DBG_assert(iArgCount == 1, ("iArgCount..", iArgCount));

    if (pArgs[0].object)
    {
        std::wcout << pArgs[0].object->u.string.string << std::endl;
    }

    DVM_Value ret;
    ret.int_value = 0;

    return ret;
}

DVM_Value Native::Sleep(DVM_VirtualMachine *pVirtualMachine, int iArgCount, DVM_Value *pArgs)
{
    DBG_assert(iArgCount == 1, ("iArgCount..", iArgCount));

    ::Sleep(pArgs[0].int_value);

    DVM_Value ret;
    ret.int_value = 0;

    return ret;
}