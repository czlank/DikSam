#pragma once

#include "DikSamc.h"
#include "GarbageCollect.h"

class Debug;
class Memory;
class Util;
class Error;
class Interface;

class Execute
{
public:
    Execute(Debug& debug, Memory& memory, Util& uitl, Error& error, Interface& interfaceRef);
    ~Execute();

    DVM_Value operator () (DVM_Executable* pExecutable);

private:
    int ArrayGetInt(DVM_Object *pArray, int iIndex);
    double ArrayGetDouble(DVM_Object *pArray, int iIndex);
    DVM_Object* ArrayGetObject(DVM_Object *pArray, int iIndex);
    void ArraySetInt(DVM_Object* pArray, int iIndex, int value);
    void ArraySetDouble(DVM_Object* pArray, int iIndex, double value);
    void ArraySetObject(DVM_Object* pArray, int iIndex, DVM_Object *value);
    void CheckArray(DVM_Object *pArray, int iIndex, DVM_Executable *pExecutable, Function *pFunction, int iPC);
    DVM_ObjectRef ChainString(DVM_VirtualMachine *pVirtualMachine, DVM_ObjectRef str1, DVM_ObjectRef str2);
    void InitializeLocalVariables(DVM_VirtualMachine *pVirtualMachine, DVM_Function *pFunction, int iFromSP);
    void ExpandStack(DVM_VirtualMachine *pVirtualMachine, int iNeedStackSize);
    void InvokeNativeFunction(DVM_VirtualMachine *pVirtualMachine, Function *pCaller, Function *pCallee, int iPC, int *pSP, int iBase);
    void InvokeDikSamFunction(DVM_VirtualMachine *pVirtualMachine, Function **ppCaller, Function *pCallee, DVM_Byte **ppCode, int *pCodeSize, int *pPC, int *pSP, int *pBase, ExecutableEntry **ppExeEntry, DVM_Executable **ppExe);
    DVM_Boolean DoReturn(DVM_VirtualMachine *pVirtualMachine, Function **ppFunction, DVM_Byte **ppCode, int *pCodeSize, int *pPC, int *pBase, ExecutableEntry **ppExeEntry, DVM_Executable **ppExe);
    DVM_Boolean ReturnFunction(DVM_VirtualMachine *pVirtualMachine, Function **ppFunction, DVM_Byte **ppCode, int *pCodeSize, int *pPC, int *pBase, ExecutableEntry **ppExeEntry, DVM_Executable **ppExe);
    DVM_ObjectRef CreateArraySub(DVM_VirtualMachine *pVirtualMachine, int iDim, int iDimIndex, DVM_TypeSpecifier *pType);
    DVM_ObjectRef CreateArray(DVM_VirtualMachine *pVirtualMachine, int iDim, DVM_TypeSpecifier *pType);
    DVM_ObjectRef CreateArrayLiteralInt(DVM_VirtualMachine *pVirtualMachine, int iSize);
    DVM_ObjectRef CreateArrayLiteralDouble(DVM_VirtualMachine *pVirtualMachine, int iSize);
    DVM_ObjectRef CreateArrayLiteralObject(DVM_VirtualMachine *pVirtualMachine, int iSize);
    void RestorePC(DVM_VirtualMachine *pVirtualMachine, ExecutableEntry *pExecutableEntry, Function *pFunction, int iPC);
    DVM_Value ExecuteCode(Function *pFunction, DVM_Byte *pCode, int iCodeSize);

    void DisposeVirtualMachine();

    inline bool IsPointerType(DVM_TypeSpecifier *pType);

private:
    Debug       &m_Debug;
    Memory      &m_Memory;
    Util        &m_Util;
    Error       &m_Error;
    Interface   &m_Interface;

    GarbageCollect      m_GarbageCollect;
};